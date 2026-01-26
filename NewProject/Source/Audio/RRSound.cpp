#include "RRSound.h"
#include "MidiMapper.h"  

//==============================================================================
// Constructor
RRSound::RRSound()
{
    // Initialize with default values
    // audioBuffer starts empty (0 channels, 0 samples)
    // Other members already initialized in header with default values
}

// Destructor
RRSound::~RRSound()
{
    // audioBuffer will clean itself up automatically (JUCE handles this)
}

//==============================================================================
// JUCE SynthesiserSound Required Methods
bool RRSound::appliesToNote(int midiNoteNumber)
{
    // If not assigned to a key pair yet, don't respond to any notes
    if (keyPairIndex < 0 || keyPairIndex > 9)
        return false;

    // Get the two MIDI notes for our assigned key pair
    int note1, note2;
    getMidiNotesForPair(keyPairIndex, note1, note2);

    // Return true if the incoming MIDI note matches either of our pair
    return (midiNoteNumber == note1) || (midiNoteNumber == note2);
}

bool RRSound::appliesToChannel(int midiChannel)
{
    // Respond to all MIDI channels (1-16)
    // This is typical for most instruments
    return true;
}

//==============================================================================
// Audio Loading
bool RRSound::loadFromFile(const juce::File& file, juce::AudioFormatManager& formatManager)
{
    if (!file.existsAsFile())
    {
        DBG("File does not exist: " + file.getFullPathName());
        return false;
    }

    // Step 1: Try to create a reader for the audio file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader == nullptr)
    {
        // File couldn't be read (wrong format, corrupted, doesn't exist, etc.)
        DBG("Failed to create reader for file: " + file.getFullPathName());
        return false;
    }

    // Step 2: Store the original sample rate (we'll need this for pitch shifting)
    originalSampleRate = reader->sampleRate;

    // Step 3: Prepare our buffer to hold the audio data
   // We want MONO audio, so we'll either take the left channel or mix stereo to mono
    const int numSamples = static_cast<int>(reader->lengthInSamples);
    audioBuffer.setSize(1, numSamples);  // 1 channel (mono), numSamples long

    // Step 4: Read the audio file into our buffer
    if (reader->numChannels == 1)
    {
        // Mono source - read directly into our buffer
        reader->read(&audioBuffer, 0, numSamples, 0, true, false);
    }
    else
    {
        // Stereo (or more channels) - read both channels then mix to mono
        juce::AudioBuffer<float> tempBuffer(reader->numChannels, numSamples);
        reader->read(&tempBuffer, 0, numSamples, 0, true, true);

        // Mix all channels to mono (simple average)
        audioBuffer.clear();
        for (int channel = 0; channel < reader->numChannels; ++channel)
        {
            audioBuffer.addFrom(0, 0, tempBuffer, channel, 0, numSamples, 1.0f / reader->numChannels);
        }
    }

    // Step 5: Set display name from filename (without extension)
    displayName = file.getFileNameWithoutExtension();

    DBG("Successfully loaded: " + displayName +
        " (" + juce::String(numSamples) + " samples at " +
        juce::String(originalSampleRate) + " Hz)");

    return true;
}

//==============================================================================
// MIDI Note Mapping
void RRSound::setKeyPairIndex(int pairIndex)
{
    // Validate using MidiMapper
    if (!MidiMapper::isValidPairIndex(pairIndex))
    {
        DBG("Invalid key pair index: " + juce::String(pairIndex));
        keyPairIndex = -1;
        return;
    }

    keyPairIndex = pairIndex;

    // Get semitone offset from MidiMapper
    int offset = MidiMapper::getSemitoneOffsetForPair(pairIndex);
    rootNote = MidiMapper::ROOT_MIDI_NOTE + offset;

    DBG("Sound assigned to key pair " + juce::String(pairIndex) +
        " (" + MidiMapper::getKeyPairName(pairIndex) + ")" +
        " | Root note: " + juce::String(rootNote));
}

//==============================================================================
// Helper Methods
void RRSound::getMidiNotesForPair(int pairIndex, int& note1, int& note2) const
{
    // Delegate to the MidiMapper utility
    MidiMapper::getMidiNotesForPair(pairIndex, note1, note2);
}

void RRSound::clearSample()
{
    audioBuffer.setSize(0, 0);
    displayName = "";
    keyPairIndex = -1;
    rootNote = 48;
}