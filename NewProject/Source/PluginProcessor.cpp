/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Audio/MidiMapper.h" 

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
    )
#endif
{
    // Initialize the synthesiser with one voice for monophonic playback
    synthesiser.addVoice(new RRVoice());

    // Register basic audio formats for sample loading
    formatManager.registerBasicFormats();

    DBG("Round Robin Lite initialized - Synthesiser ready");
    DBG("Number of voices: " + juce::String(synthesiser.getNumVoices()));

    // ============================================================
    // TEST: Load a single sample and assign to root key pair
    // ============================================================

    // IMPORTANT: Change this path to point to your test WAV file
    juce::File testFile("C:\\Users\\hamad\\OneDrive\\Desktop\\snd_surf_hard_dirt_01.wav");

    DBG("\n=== LOADING TEST SAMPLE ===");
    DBG("Test file path: " + testFile.getFullPathName());

    if (!testFile.existsAsFile())
    {
        DBG("ERROR: Test file not found!");
        DBG("Please update the file path in PluginProcessor constructor");
        DBG("Current path: " + testFile.getFullPathName());
        return;
    }

    DBG("File exists! Size: " + juce::String(testFile.getSize()) + " bytes");

    // Create a new RRSound object
    RRSound* testSound = new RRSound();

    // Try to load the audio file
    if (testSound->loadFromFile(testFile, formatManager))
    {
        DBG("✓ Sample loaded successfully!");
        DBG("  - Display name: " + testSound->getDisplayName());
        DBG("  - Sample count: " + juce::String(testSound->getNumSamples()));
        DBG("  - Sample rate: " + juce::String(testSound->getOriginalSampleRate()) + " Hz");

        // Assign to pair 7 (C4/D4 - MIDI 60/62) which is the ROOT pair
        // At root pair, the sample plays at its original pitch (no pitch shift)
        testSound->setKeyPairIndex(7);
        DBG("  - Assigned to pair 7: " + MidiMapper::getKeyPairName(7));

        // Verify which MIDI notes this sound will respond to
        int note1, note2;
        MidiMapper::getMidiNotesForPair(7, note1, note2);
        DBG("  - MIDI notes: " + juce::String(note1) + " and " + juce::String(note2));
        DBG("  - Root note: " + juce::String(testSound->getRootNote()));

        // Add the sound to the synthesiser
        synthesiser.addSound(testSound);
        DBG("✓ Sound added to synthesiser");
        DBG("Total sounds in synthesiser: " + juce::String(synthesiser.getNumSounds()));

        DBG("\n=== READY TO PLAY ===");
        DBG("Press MIDI note 60 (C4/Middle C) to hear the sample at original pitch");
        DBG("Press MIDI note 62 (D4) to hear the sample at original pitch");
        DBG("Try other notes to hear pitch shifting!");
    }
    else
    {
        DBG("✗ ERROR: Failed to load sample from file");
        delete testSound;
    }

    DBG("=============================\n");
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
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}