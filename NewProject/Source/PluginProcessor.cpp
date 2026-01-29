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

    DBG("Round Robin Lite initialized - Synthesiser ready");
    DBG("Number of voices: " + juce::String(synthesiser.getNumVoices()));

    // Print mapping info
    MidiMapper::printMappingInfo();

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

    // ============================================================
// TEST: Load single sample
// ============================================================

    juce::File testFile("C:\\Users\\hamad\\OneDrive\\Desktop\\snd_surf_hard_dirt_01.wav");

    DBG("\n=== LOADING TEST SAMPLE ===");

    if (!testFile.existsAsFile())
    {
        DBG("ERROR: Test file not found at: " + testFile.getFullPathName());
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
        DBG("Press C2 (MIDI 36) or D2 (MIDI 38) to hear the sample");
        DBG("Both keys play at original pitch");
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
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
    // This is called before playback starts, giving us a chance to prepare

    DBG("prepareToPlay called - Sample Rate: " + juce::String(sampleRate) +
        " Hz, Buffer Size: " + juce::String(samplesPerBlock) + " samples");

    // Tell the synthesiser what sample rate we're running at
    // This is CRITICAL for correct pitch calculation
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

    DBG("releaseResources called - Playback stopped");
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
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

    // CRITICAL: Clear the output buffer first
    // The synthesiser uses addSample(), so we need to start with silence
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // For a synth, we also clear the input channels since we're generating audio
    for (int i = 0; i < totalNumInputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // THIS IS WHERE THE MAGIC HAPPENS!
    // The synthesiser processes all MIDI messages and renders audio from active voices
    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Optional: Log MIDI activity for debugging
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
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor(*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// PARAMETER LAYOUT CREATION

juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //==============================================================================
    // GLOBAL PITCH CONTROLS

    // Semitone: -12 to +12 semitones, step 1, default 0
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::semitone, 1),  // ID and version
        "Semitone",                                      // Name shown in DAW
        -12,                                             // Min
        12,                                              // Max
        0,                                               // Default
        juce::String(),                                  // Label (empty for now)
        [](int value, int) { return juce::String(value) + " st"; }  // Value to text
    ));

    // Fine Tune: -100 to +100 cents, step 1, default 0
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::fineTune, 1),
        "Fine Tune",
        -100,                                            // Min
        100,                                             // Max
        0,                                               // Default
        juce::String(),
        [](int value, int) { return juce::String(value) + " cents"; }
    ));

    //==============================================================================
    // VOLUME

    // Volume: 0.0 to 1.0, default 0.75 (approx -2.5dB)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::volume, 1),
        "Volume",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),  // Min, Max, Step
        0.75f,                                                // Default
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

    // Envelope Attack: 0 to 1000ms, default 0ms (instant)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envAttack, 1),
        "Env Attack",
        juce::NormalisableRange<float>(0.0f, 1000.0f, 0.1f, 0.3f),  // Min, Max, Step, Skew (slight curve)
        0.0f,                                                         // Default
        "ms",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1) + " ms"; }
    ));

    // Envelope Decay: 0 to 5000ms, default 100ms
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::envDecay, 1),
        "Env Decay",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 1.0f, 0.4f),  // Slight curve for better control
        100.0f,                                                       // Default
        "ms",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1) + " ms"; }
    ));

    //==============================================================================
    // 3-BAND EQ

    // Low Band Gain: -24dB to +24dB, default 0dB
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowGain, 1),
        "Low Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        "dB"
    ));

    // Low Band Frequency: 20Hz to 500Hz, default 100Hz (logarithmic)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::lowFreq, 1),
        "Low Freq",
        juce::NormalisableRange<float>(20.0f, 500.0f, 1.0f, 0.25f),  // 0.25 skew = logarithmic
        100.0f,
        "Hz"
    ));

    // Mid Band Gain: -24dB to +24dB, default 0dB
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midGain, 1),
        "Mid Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        "dB"
    ));

    // Mid Band Frequency: 200Hz to 5000Hz, default 1000Hz (logarithmic)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::midFreq, 1),
        "Mid Freq",
        juce::NormalisableRange<float>(200.0f, 5000.0f, 1.0f, 0.3f),
        1000.0f,
        "Hz"
    ));

    // High Band Gain: -24dB to +24dB, default 0dB
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highGain, 1),
        "High Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f),
        0.0f,
        "dB"
    ));

    // High Band Frequency: 2000Hz to 20000Hz, default 5000Hz (logarithmic)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID(ParameterIDs::highFreq, 1),
        "High Freq",
        juce::NormalisableRange<float>(2000.0f, 20000.0f, 1.0f, 0.3f),
        5000.0f,
        "Hz"
    ));

    //==============================================================================
    // TRANSIENT MASTER

    // Transient Attack: -127 to +127, default 0
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientAttack, 1),
        "Transient Attack",
        -127,
        127,
        0
    ));

    // Transient Decay: -127 to +127, default 0
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(ParameterIDs::transientDecay, 1),
        "Transient Decay",
        -127,
        127,
        0
    ));

    //==============================================================================

    DBG("Parameter layout created with " + juce::String(ParameterIDs::totalParameters) + " parameters");

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}

