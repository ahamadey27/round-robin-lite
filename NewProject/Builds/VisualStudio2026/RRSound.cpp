#include "RRSound.h"

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
    // Validate the pair index is in range
    if (pairIndex < 0 || pairIndex > 9)
    {
        DBG("Invalid key pair index: " + juce::String(pairIndex));
        keyPairIndex = -1;
        return;
    }

    keyPairIndex = pairIndex;

    // Update root note based on the pair index offset
    // Root is C2 (48), and each pair has a semitone offset
    const int semitoneOffsets[] = { -7, -6, -5, -4, -3, -2, -1, 0, +1, +2 };
    rootNote = 48 + semitoneOffsets[pairIndex];

    DBG("Sound assigned to key pair " + juce::String(pairIndex) +
        " (root note: " + juce::String(rootNote) + ")");
}

//==============================================================================
// Helper Methods
void RRSound::getMidiNotesForPair(int pairIndex, int& note1, int& note2) const
{
    // Map key pair index to the two MIDI note numbers
    // These are the white key pairs in Round Robin Lite's mapping

    switch (pairIndex)
    {
    case 0:  note1 = 36;  note2 = 38;  break;  // C0/D0
    case 1:  note1 = 40;  note2 = 41;  break;  // E0/F0
    case 2:  note1 = 43;  note2 = 45;  break;  // G0/A0
    case 3:  note1 = 47;  note2 = 48;  break;  // B0/C1
    case 4:  note1 = 52;  note2 = 53;  break;  // E1/F1
    case 5:  note1 = 55;  note2 = 57;  break;  // G1/A1
    case 6:  note1 = 59;  note2 = 60;  break;  // B1/C2
    case 7:  note1 = 48;  note2 = 50;  break;  // C2/D2 (ROOT)
    case 8:  note1 = 64;  note2 = 65;  break;  // E2/F2
    case 9:  note1 = 67;  note2 = 69;  break;  // G2/A2
    default: note1 = -1;  note2 = -1;  break;  // Invalid
    }
}