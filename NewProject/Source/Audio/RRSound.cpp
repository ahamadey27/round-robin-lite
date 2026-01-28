#include "RRSound.h"
#include "MidiMapper.h"

//==============================================================================
// Constructor
RRSound::RRSound()
{
    // Initialize with default values
    // audioBuffer starts empty (0 channels, 0 samples)
}

// Destructor
RRSound::~RRSound()
{
    // audioBuffer will clean itself up automatically
}

//==============================================================================
// JUCE SynthesiserSound Required Methods

bool RRSound::appliesToNote(int midiNoteNumber)
{
    // All loaded sounds respond to C2 (36) and D2 (38)
    return MidiMapper::isTriggerNote(midiNoteNumber);
}

bool RRSound::appliesToChannel(int midiChannel)
{
    // Respond to all MIDI channels (1-16)
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

    // Try to create a reader for the audio file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader == nullptr)
    {
        DBG("Failed to create reader for file: " + file.getFullPathName());
        return false;
    }

    // Store the original sample rate
    originalSampleRate = reader->sampleRate;

    // Prepare buffer for mono audio
    const int numSamples = static_cast<int>(reader->lengthInSamples);
    audioBuffer.setSize(1, numSamples);  // 1 channel (mono)

    // Read audio file into buffer
    if (reader->numChannels == 1)
    {
        // Mono source - read directly
        reader->read(&audioBuffer, 0, numSamples, 0, true, false);
    }
    else
    {
        // Stereo (or more) - mix to mono
        juce::AudioBuffer<float> tempBuffer(reader->numChannels, numSamples);
        reader->read(&tempBuffer, 0, numSamples, 0, true, true);

        // Mix all channels to mono (simple average)
        audioBuffer.clear();
        for (int channel = 0; channel < reader->numChannels; ++channel)
        {
            audioBuffer.addFrom(0, 0, tempBuffer, channel, 0, numSamples, 1.0f / reader->numChannels);
        }
    }

    // Set display name from filename (without extension)
    displayName = file.getFileNameWithoutExtension();

    DBG("Successfully loaded: " + displayName +
        " (" + juce::String(numSamples) + " samples at " +
        juce::String(originalSampleRate) + " Hz)");

    return true;
}

//==============================================================================
// Helper Methods

void RRSound::clearSample()
{
    audioBuffer.setSize(0, 0);
    displayName = "";
}