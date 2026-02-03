/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Audio/MidiMapper.h" 
#include "Parameters/ParametersIDs.h"

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

    // ============================================================
    // TEST: Load single sample
    // ============================================================

    juce::File testFile("C:\\Users\\hamad\\OneDrive\\Desktop\\snd_surf_hard_dirt_01.wav");

    DBG("\n=== LOADING TEST SAMPLE ===");

    if (!testFile.existsAsFile())
    {
        DBG("ERROR: Test file not found at: " + testFile.getFullPathName());
        DBG("Please update the file path in PluginProcessor.cpp constructor");
        return;
    }

    // Create and load sound
    RRSound* testSound = new RRSound();

    if (testSound->loadFromFile(testFile, formatManager))
    {
        DBG("Sample loaded: " + testSound->getDisplayName());
        DBG("  Samples: " + juce::String(testSound->getNumSamples()));
        DBG("  Rate: " + juce::String(testSound->getOriginalSampleRate()) + " Hz");

        // Add to synthesiser
        synthesiser.addSound(testSound);

        DBG("\n=== READY TO PLAY ===");
        DBG("Press ANY MIDI key to hear the sample");
        DBG("All keys play at original pitch (unpitched mode)");
        DBG("=============================\n");
    }
    else
    {
        DBG("Failed to load sample");
        delete testSound;
    }
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

    // EQ Parameters
    smoothedLowGain.reset(sampleRate, rampTimeSeconds);
    smoothedLowGain.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::lowGain)->load());

    smoothedLowFreq.reset(sampleRate, rampTimeSeconds);
    smoothedLowFreq.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::lowFreq)->load());

    smoothedMidGain.reset(sampleRate, rampTimeSeconds);
    smoothedMidGain.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::midGain)->load());

    smoothedMidFreq.reset(sampleRate, rampTimeSeconds);
    smoothedMidFreq.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::midFreq)->load());

    smoothedHighGain.reset(sampleRate, rampTimeSeconds);
    smoothedHighGain.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::highGain)->load());

    smoothedHighFreq.reset(sampleRate, rampTimeSeconds);
    smoothedHighFreq.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::highFreq)->load());

    // Transient Master
    smoothedTransientAttack.reset(sampleRate, rampTimeSeconds);
    smoothedTransientAttack.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::transientAttack)->load());

    smoothedTransientDecay.reset(sampleRate, rampTimeSeconds);
    smoothedTransientDecay.setCurrentAndTargetValue(apvts.getRawParameterValue(ParameterIDs::transientDecay)->load());

    DBG("All parameter smoothing initialized");
}

void NewProjectAudioProcessor::releaseResources()
{
    DBG("releaseResources called - Playback stopped");
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
    smoothedLowGain.setTargetValue(apvts.getRawParameterValue(ParameterIDs::lowGain)->load());
    smoothedLowFreq.setTargetValue(apvts.getRawParameterValue(ParameterIDs::lowFreq)->load());
    smoothedMidGain.setTargetValue(apvts.getRawParameterValue(ParameterIDs::midGain)->load());
    smoothedMidFreq.setTargetValue(apvts.getRawParameterValue(ParameterIDs::midFreq)->load());
    smoothedHighGain.setTargetValue(apvts.getRawParameterValue(ParameterIDs::highGain)->load());
    smoothedHighFreq.setTargetValue(apvts.getRawParameterValue(ParameterIDs::highFreq)->load());
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

    float semitones = smoothedSemitone.getCurrentValue();
    float cents = smoothedFineTune.getCurrentValue();
    float attackMs = smoothedEnvAttack.getCurrentValue();
    float decayMs = smoothedEnvDecay.getCurrentValue();

    for (int i = 0; i < synthesiser.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<RRVoice*>(synthesiser.getVoice(i)))
        {
            voice->updateGlobalParameters(semitones, cents, attackMs, decayMs);
        }
    }

    //==============================================================================
    // RENDER AUDIO FROM SYNTHESISER

    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

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
}

void NewProjectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
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
        juce::NormalisableRange<float>(0.0f, 5000.0f, 1.0f, 0.4f),
        100.0f,
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

    return layout;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}