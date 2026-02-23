/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Audio/MidiMapper.h" 
#include "Parameters/ParametersIDs.h"
#include "DSP/TransientShaper.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    // Initialize the synthesiser with one voice for monophonic playback
    synthesiser.addVoice(new RRVoice());

    // Register basic audio formats for sample loading
    formatManager.registerBasicFormats();

        DBG("=== Round Robin Lite Initialized ===");
        DBG("Synthesiser ready with " + juce::String(synthesiser.getNumVoices()) + " voice(s)");
        DBG("UNPITCHED PLAYBACK MODE - All samples play at original pitch");
        DBG("Root reference note: C1 (MIDI 36)");
        DBG("===================================");

        // TEST: Verify parameter IDs are accessible
        DBG("\n=== Testing Parameter IDs ===");
        DBG("Semitone ID: " + juce::String(ParameterIDs::semitone));
        DBG("Volume ID: " + juce::String(ParameterIDs::volume));
        DBG("Total parameters: " + juce::String(ParameterIDs::totalParameters));
        DBG("============================\n");

        // TEST: Verify APVTS is initialized
        DBG("\n=== Testing APVTS ===");
        DBG("APVTS initialized: " + juce::String(apvts.state.isValid() ? "YES" : "NO"));
        DBG("Current parameter count: " + juce::String(apvts.state.getNumChildren()));
        DBG("=====================\n");

        DBG("=== Sample Slots Initialized ===");
        DBG("Slots available: " + juce::String(NUM_SAMPLE_SLOTS));
        DBG("All slots empty on startup — load via UI");
        DBG("================================");

        
    }

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
    // Synthesiser will clean up voices and sounds automatically
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName(int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    DBG("prepareToPlay called - Sample Rate: " + juce::String(sampleRate) +
        " Hz, Buffer Size: " + juce::String(samplesPerBlock) + " samples");

    // Tell the synthesiser what sample rate we're running at
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);

    // Resample any loaded samples if sample rate changed
    sampleLoader.setSampleRate(sampleRate);

    //==============================================================================
   // INITIALIZE 3-BAND EQ

    threeBandEQ.prepareToPlay(sampleRate, samplesPerBlock);

    //==============================================================================
    // INITIALIZE TRANSIENT SHAPER

    transientShaper.prepareToPlay(sampleRate, samplesPerBlock);

    //==============================================================================
    // INITIALIZE PARAMETER SMOOTHING

    const double rampTimeSeconds = 0.05; // 50 milliseconds

    // Pitch Controls
    smoothedSemitone.reset(sampleRate, rampTimeSeconds);
    smoothedSemitone.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::semitone)->load());

    smoothedFineTune.reset(sampleRate, rampTimeSeconds);
    smoothedFineTune.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::fineTune)->load());

    // Volume
    smoothedVolume.reset(sampleRate, rampTimeSeconds);
    smoothedVolume.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::volume)->load());

    // Amplitude Envelope
    smoothedEnvAttack.reset(sampleRate, rampTimeSeconds);
    smoothedEnvAttack.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::envAttack)->load());

    smoothedEnvDecay.reset(sampleRate, rampTimeSeconds);
    smoothedEnvDecay.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::envDecay)->load());

    // Transient Master
    smoothedTransientAttack.reset(sampleRate, rampTimeSeconds);
    smoothedTransientAttack.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::transientAttack)->load());

    smoothedTransientDecay.reset(sampleRate, rampTimeSeconds);
    smoothedTransientDecay.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::transientDecay)->load());

    DBG("All parameter smoothing initialized");
}

void NewProjectAudioProcessor::releaseResources()
{
    DBG("releaseResources called");

    // Reset EQ filter states
    threeBandEQ.reset();
    transientShaper.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void NewProjectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //==============================================================================
    // UPDATE SMOOTHED PARAMETER VALUES

    smoothedSemitone.setTargetValue(apvts.getRawParameterValue(ParameterIDs::semitone)->load());
    smoothedFineTune.setTargetValue(apvts.getRawParameterValue(ParameterIDs::fineTune)->load());
    smoothedVolume.setTargetValue(apvts.getRawParameterValue(ParameterIDs::volume)->load());
    smoothedEnvAttack.setTargetValue(apvts.getRawParameterValue(ParameterIDs::envAttack)->load());
    smoothedEnvDecay.setTargetValue(apvts.getRawParameterValue(ParameterIDs::envDecay)->load());
    smoothedTransientAttack.setTargetValue(apvts.getRawParameterValue(ParameterIDs::transientAttack)->load());
    smoothedTransientDecay.setTargetValue(apvts.getRawParameterValue(ParameterIDs::transientDecay)->load());

    //==============================================================================
    // CLEAR OUTPUT BUFFERS

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int i = 0; i < totalNumInputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    //==============================================================================
   // STORE GLOBAL PARAMETER VALUES FOR VOICES

   // These values will be accessed by voices when they start
    globalSemitones.store(smoothedSemitone.getCurrentValue());
    globalCents.store(smoothedFineTune.getCurrentValue());
    globalAttackMs.store(smoothedEnvAttack.getCurrentValue());
    globalDecayMs.store(smoothedEnvDecay.getCurrentValue());

    //==============================================================================
    // UPDATE ALL VOICES WITH CURRENT GLOBAL PARAMETERS

   // Get INSTANT parameter values (no smoothing for note-start parameters)
    float semitones = static_cast<float>(apvts.getRawParameterValue(ParameterIDs::semitone)->load());
    float cents = static_cast<float>(apvts.getRawParameterValue(ParameterIDs::fineTune)->load());
    float attackMs = apvts.getRawParameterValue(ParameterIDs::envAttack)->load();
    float decayMs = apvts.getRawParameterValue(ParameterIDs::envDecay)->load();

    for (int i = 0; i < synthesiser.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<RRVoice*>(synthesiser.getVoice(i)))
        {
            voice->updateGlobalParameters(semitones, cents, attackMs, decayMs);
        }
    }

    for (int i = 0; i < synthesiser.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<RRVoice*>(synthesiser.getVoice(i)))
        {
            voice->setRandomizationReferences(&randomizationEngine, &apvts);
        }
    }

    //==============================================================================
    // RENDER AUDIO FROM SYNTHESISER

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    //==============================================================================
   // APPLY 3-BAND EQ (AFTER SYNTH, BEFORE VOLUME)

   // Get smoothed EQ parameter values
    float eqLowGain = apvts.getRawParameterValue(ParameterIDs::lowGain)->load();
    float eqLowFreq = apvts.getRawParameterValue(ParameterIDs::lowFreq)->load();
    float eqMidGain = apvts.getRawParameterValue(ParameterIDs::midGain)->load();
    float eqMidFreq = apvts.getRawParameterValue(ParameterIDs::midFreq)->load();
    float eqHighGain = apvts.getRawParameterValue(ParameterIDs::highGain)->load();
    float eqHighFreq = apvts.getRawParameterValue(ParameterIDs::highFreq)->load();
    float transAtk = smoothedTransientAttack.getNextValue();
    float transDec = smoothedTransientDecay.getNextValue();

    // Check if a voice is active and use its randomized values instead
    for (int i = 0; i < synthesiser.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<RRVoice*>(synthesiser.getVoice(i)))
        {
            if (voice->isVoiceActive())
            {
                eqLowGain = voice->getRandomizedLowGain();
                eqLowFreq = voice->getRandomizedLowFreq();
                eqMidGain = voice->getRandomizedMidGain();
                eqMidFreq = voice->getRandomizedMidFreq();
                eqHighGain = voice->getRandomizedHighGain();
                eqHighFreq = voice->getRandomizedHighFreq();
                transAtk = voice->getRandomizedTransientAttack();
                transDec = voice->getRandomizedTransientDecay();
                break;
            }
        }
    }

    // Update EQ filters with current parameter values
    threeBandEQ.updateFilters(eqLowGain, eqLowFreq, eqMidGain, eqMidFreq, eqHighGain, eqHighFreq);

    // Process the buffer through the EQ
    threeBandEQ.processBlock(buffer);

    //==============================================================================
    // APPLY TRANSIENT SHAPING (AFTER EQ, BEFORE VOLUME)

    // Get transient parameter values (these are integers -127 to +127)
    float attackAmount = static_cast<float>(apvts.getRawParameterValue(ParameterIDs::transientAttack)->load());
    float decayAmount = static_cast<float>(apvts.getRawParameterValue(ParameterIDs::transientDecay)->load());

    // Process transient shaping
    transientShaper.processBlock(buffer, transAtk, transDec);

    //==============================================================================
    // APPLY VOLUME CONTROL

    // Get the number of samples in this block
    const int numSamples = buffer.getNumSamples();

    // Apply volume to each sample with smoothing
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Get the next smoothed volume value
            float volumeGain = smoothedVolume.getNextValue();

            // Apply gain to this sample
            channelData[sample] *= volumeGain;
        }
    }

    //==============================================================================
    // LOG MIDI ACTIVITY (for debugging)

    if (!midiMessages.isEmpty())
    {
        DBG("Processing " + juce::String(midiMessages.getNumEvents()) + " MIDI events");

        for (const auto metadata : midiMessages)
        {
            const auto msg = metadata.getMessage();
            if (msg.isNoteOn())
                DBG("MIDI Note ON: " + juce::String(msg.getNoteNumber()) +
                    " Velocity: " + juce::String(msg.getVelocity()));
            else if (msg.isNoteOff())
                DBG("MIDI Note OFF: " + juce::String(msg.getNoteNumber()));
        }
    }
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor(*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // SAVE PLUGIN STATE TO PRESET
    // This is called by the DAW when saving a project or preset

    DBG("=== SAVING PLUGIN STATE ===");

    // Step 1: Get the current parameter state from APVTS as a ValueTree
    auto state = apvts.copyState();

    // Step 2: Add custom data (for future use - sample paths will go here)
    // For now, we'll create a placeholder section
    juce::ValueTree customData("CustomData");
    customData.setProperty("version", 1, nullptr);  // Track preset format version

    // TODO (Phase 4): Add sample file paths here
    // Example structure for future:
    // juce::ValueTree samples("Samples");
    // for (int i = 0; i < 20; ++i)
    // {
    //     juce::ValueTree sample("Sample");
    //     sample.setProperty("slot", i, nullptr);
    //     sample.setProperty("path", samplePaths[i], nullptr);
    //     samples.appendChild(sample, nullptr);
    // }
    // customData.appendChild(samples, nullptr);

    state.appendChild(customData, nullptr);

    // Step 3: Convert the ValueTree to XML
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    if (xml != nullptr)
    {
        // Step 4: Convert XML to binary and store in destData
        copyXmlToBinary(*xml, destData);

        DBG("State saved successfully");
        DBG("  Parameters saved: " + juce::String(apvts.state.getNumChildren()));
        DBG("========================");
    }
    else
    {
        DBG("ERROR: Failed to create XML from state");
    }
}

void NewProjectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // LOAD PLUGIN STATE FROM PRESET
    // This is called by the DAW when loading a project or preset

    DBG("=== LOADING PLUGIN STATE ===");

    // Step 1: Parse the binary data back to XML
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState == nullptr)
    {
        DBG("ERROR: Failed to parse XML from binary data");
        return;
    }

    // Step 2: Check if the XML is valid and matches our state format
    if (!xmlState->hasTagName(apvts.state.getType()))
    {
        DBG("ERROR: XML tag name doesn't match APVTS state type");
        return;
    }

    // Step 3: Convert XML back to ValueTree
    juce::ValueTree state = juce::ValueTree::fromXml(*xmlState);

    if (!state.isValid())
    {
        DBG("ERROR: Failed to create ValueTree from XML");
        return;
    }

    // Step 4: Restore APVTS parameter values from the loaded state
    apvts.replaceState(state);

    DBG("State loaded successfully");
    DBG("  Parameters restored: " + juce::String(apvts.state.getNumChildren()));

    // Step 5: Restore custom data (sample paths - for future implementation)
    juce::ValueTree customData = state.getChildWithName("CustomData");
    if (customData.isValid())
    {
        int version = customData.getProperty("version", 0);
        DBG("  Custom data version: " + juce::String(version));

        // TODO (Phase 4): Load sample file paths here
        // juce::ValueTree samples = customData.getChildWithName("Samples");
        // if (samples.isValid())
        // {
        //     for (int i = 0; i < samples.getNumChildren(); ++i)
        //     {
        //         juce::ValueTree sample = samples.getChild(i);
        //         int slot = sample.getProperty("slot");
        //         juce::String path = sample.getProperty("path").toString();
        //         // Load sample from path...
        //     }
        // }
    }

    DBG("========================");
}

//==============================================================================
// PARAMETER LAYOUT CREATION

juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //==============================================================================
    // GLOBAL PITCH CONTROLS

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::semitone, 1),
        "Semitone",
        -12, 12, 0,
        juce::String(),
        [](int value, int) { return juce::String(value) + " st"; }
    ));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::fineTune, 1),
        "Fine Tune",
        -100, 100, 0,
        juce::String(),
        [](int value, int) { return juce::String(value) + " cents"; }
    ));

    //==============================================================================
    // VOLUME

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::volume, 1),
        "Volume",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.75f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {
            return juce::String(juce::Decibels::gainToDecibels(value), 1) + " dB";
        },
        [](const juce::String& text) {
            return juce::Decibels::decibelsToGain(text.dropLastCharacters(3).getFloatValue());
        }
    ));

    //==============================================================================
    // Pan
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::pan, 1),
        "Pan",
        juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f),
        0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {
            if (value < -0.01f) return juce::String(int(std::abs(value) * 100)) + "% L";
            if (value > 0.01f) return juce::String(int(value * 100)) + "% R";
            return juce::String("Center");
        }
    ));

    // ADD PAN RANDOMIZATION (near other randomization params)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::panRndNeg, 1),
        "Pan Rnd Neg", 0.0f, 1.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::panRndPos, 1),
        "Pan Rnd Pos", 0.0f, 1.0f, 0.0f));

    //==============================================================================
    // AMPLITUDE ENVELOPE

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envAttack, 1),
        "Env Attack",
        juce::NormalisableRange<float>(0.0f, 1000.0f, 0.1f, 0.3f),
        0.0f,
        "ms",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1) + " ms"; }
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envDecay, 1),
        "Env Decay",
        juce::NormalisableRange<float>(20.0f, 5000.0f, 1.0f, 0.4f),
        820.0f, //default amount
        "ms",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1) + " ms"; }
    ));

    //==============================================================================
    // 3-BAND EQ

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowGain, 1),
        "Low Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        "dB"
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowFreq, 1),
        "Low Freq",
        juce::NormalisableRange<float>(20.0f, 500.0f, 1.0f, 0.25f),
        100.0f,
        "Hz"
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midGain, 1),
        "Mid Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        "dB"
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midFreq, 1),
        "Mid Freq",
        juce::NormalisableRange<float>(200.0f, 5000.0f, 1.0f, 0.3f),
        1000.0f,
        "Hz"
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highGain, 1),
        "High Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        "dB"
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highFreq, 1),
        "High Freq",
        juce::NormalisableRange<float>(2000.0f, 20000.0f, 1.0f, 0.3f),
        5000.0f,
        "Hz"
    ));

    // Envelope Attack Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envAttackRndNeg, 1),
        "Env Attack Rnd Neg", 0.0f, 1000.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envAttackRndPos, 1),
        "Env Attack Rnd Pos", 0.0f, 1000.0f, 0.0f));

    // Envelope Decay Randomization  
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envDecayRndNeg, 1),
        "Env Decay Rnd Neg", 0.0f, 5000.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envDecayRndPos, 1),
        "Env Decay Rnd Pos", 0.0f, 5000.0f, 0.0f));

    // Transient Attack Randomization (existing code below)

    //==============================================================================
    // TRANSIENT MASTER

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientAttack, 1),
        "Transient Attack",
        -127, 127, 0
    ));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientDecay, 1),
        "Transient Decay",
        -127, 127, 0
    ));

    DBG("Parameter layout created with " + juce::String(ParameterIDs::totalParameters) + " parameters");

    //==============================================================================
    // RANDOMIZATION PARAMETERS (22 total: 2 per randomizable parameter)

    // Semitone Randomization
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::semitoneRndNeg, 1),
        "Semitone Rnd Neg", 0, 12, 0));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::semitoneRndPos, 1),
        "Semitone Rnd Pos", 0, 12, 0));

    // Fine Tune Randomization
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::fineTuneRndNeg, 1),
        "Fine Tune Rnd Neg", 0, 100, 0));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::fineTuneRndPos, 1),
        "Fine Tune Rnd Pos", 0, 100, 0));

    // Volume Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::volumeRndNeg, 1),
        "Volume Rnd Neg", 0.0f, 1.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::volumeRndPos, 1),
        "Volume Rnd Pos", 0.0f, 1.0f, 0.0f));

    // Low EQ Gain Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowGainRndNeg, 1),
        "Low Gain Rnd Neg", 0.0f, 24.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowGainRndPos, 1),
        "Low Gain Rnd Pos", 0.0f, 24.0f, 0.0f));

    // Low EQ Frequency Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowFreqRndNeg, 1),
        "Low Freq Rnd Neg", 0.0f, 480.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowFreqRndPos, 1),
        "Low Freq Rnd Pos", 0.0f, 480.0f, 0.0f));

    // Mid EQ Gain Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midGainRndNeg, 1),
        "Mid Gain Rnd Neg", 0.0f, 24.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midGainRndPos, 1),
        "Mid Gain Rnd Pos", 0.0f, 24.0f, 0.0f));

    // Mid EQ Frequency Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midFreqRndNeg, 1),
        "Mid Freq Rnd Neg", 0.0f, 4800.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midFreqRndPos, 1),
        "Mid Freq Rnd Pos", 0.0f, 4800.0f, 0.0f));

    // High EQ Gain Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highGainRndNeg, 1),
        "High Gain Rnd Neg", 0.0f, 24.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highGainRndPos, 1),
        "High Gain Rnd Pos", 0.0f, 24.0f, 0.0f));

    // High EQ Frequency Randomization
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highFreqRndNeg, 1),
        "High Freq Rnd Neg", 0.0f, 18000.0f, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highFreqRndPos, 1),
        "High Freq Rnd Pos", 0.0f, 18000.0f, 0.0f));

    // Transient Attack Randomization
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientAttackRndNeg, 1),
        "Trans Atk Rnd Neg", 0, 127, 0));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientAttackRndPos, 1),
        "Trans Atk Rnd Pos", 0, 127, 0));

    // Transient Decay Randomization
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientDecayRndNeg, 1),
        "Trans Dec Rnd Neg", 0, 127, 0));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientDecayRndPos, 1),
        "Trans Dec Rnd Pos", 0, 127, 0));

    return layout;

    
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}