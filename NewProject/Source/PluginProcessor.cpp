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
    // TEST: Load sample and assign to ALL pairs for testing
    // ============================================================

    juce::File testFile("C:\\Users\\hamad\\OneDrive\\Desktop\\snd_surf_hard_dirt_01.wav");

    DBG("\n=== LOADING TEST SAMPLE ===");
    DBG("Test file path: " + testFile.getFullPathName());

    if (!testFile.existsAsFile())
    {
        DBG("ERROR: Test file not found!");
        return;
    }

    DBG("File exists! Size: " + juce::String(testFile.getSize()) + " bytes");

    // Load the sample ONCE into a temporary sound
    RRSound* templateSound = new RRSound();

    if (templateSound->loadFromFile(testFile, formatManager))
    {
        DBG("✓ Sample loaded successfully!");
        DBG("  - Display name: " + templateSound->getDisplayName());
        DBG("  - Sample count: " + juce::String(templateSound->getNumSamples()));
        DBG("  - Sample rate: " + juce::String(templateSound->getOriginalSampleRate()) + " Hz");

        // Now assign this sample to ALL 10 key pairs for testing
        DBG("\n=== ASSIGNING TO ALL KEY PAIRS ===");

        for (int pairIndex = 0; pairIndex < 10; ++pairIndex)
        {
            // Create a new RRSound for each pair
            RRSound* sound = new RRSound();

            // Load the same file
            if (sound->loadFromFile(testFile, formatManager))
            {
                // Assign to this pair
                sound->setKeyPairIndex(pairIndex);

                // Add to synthesiser
                synthesiser.addSound(sound);

                // Get MIDI notes for logging
                int note1, note2;
                MidiMapper::getMidiNotesForPair(pairIndex, note1, note2);

                DBG("  Pair " + juce::String(pairIndex) + ": " +
                    MidiMapper::getKeyPairName(pairIndex) +
                    " (MIDI " + juce::String(note1) + "/" + juce::String(note2) + ")");
            }
        }

        delete templateSound;  // Clean up the template

        DBG("\n✓ Sample assigned to all 10 key pairs!");
        DBG("Total sounds in synthesiser: " + juce::String(synthesiser.getNumSounds()));

        DBG("\n=== READY TO PLAY ===");
        DBG("Try playing across your entire keyboard!");
        DBG("Each pair should pitch-shift the sample:");
        DBG("  - Lower pairs (C2-G2) = Lower pitch");
        DBG("  - Pair 7 (C3/D3 on your keyboard) = Original pitch");
        DBG("  - Higher pairs (E3-G3) = Higher pitch");
    }
    else
    {
        DBG("✗ ERROR: Failed to load sample");
        delete templateSound;
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