/*
  ==============================================================================

    Custom SynthesiserSound class for Round Robin Lite.
    Represents a single loaded audio sample.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
 * RRSound - Represents a loaded audio sample for Round Robin Lite.
 * Each RRSound stores:
 * - The actual audio data (mono buffer)
 * - The original sample rate
 * - Display information for the UI
 */

class RRSound : public juce::SynthesiserSound
{
public:
    // Constructor & Destructor
    RRSound();
    ~RRSound() override;

    /**
     * Determines if this sound should play when the given MIDI note is triggered.
     * For Round Robin Lite, all sounds respond to C2 (36) and D2 (38).
     * @param midiNoteNumber The MIDI note number (0-127)
     * @return true if this is C2 or D2
     */
    bool appliesToNote(int midiNoteNumber) override;

    /**
     * Determines if this sound responds to the given MIDI channel.
     * We respond to all channels (1-16).
     * @param midiChannel The MIDI channel (1-16)
     * @return true (always responds to all channels)
     */
    bool appliesToChannel(int midiChannel) override;

    //==============================================================================
    // Audio Data Management

    /**
     * Loads an audio file into this sound's buffer.
     *
     * This method:
     * - Reads the audio file using JUCE's AudioFormatManager
     * - Converts stereo to mono (if needed)
     * - Stores the original sample rate
     * - Stores the audio data in our buffer
     * @param file The audio file to load
     * @param formatManager Reference to the audio format manager
     * @return true if loading was successful, false otherwise
     */
    bool loadFromFile(const juce::File& file, juce::AudioFormatManager& formatManager);

    /**
     * Gets a reference to the audio buffer containing the sample data.
     * @return Const reference to the audio buffer
     */
    const juce::AudioBuffer<float>& getAudioBuffer() const { return audioBuffer; }

    /**
     * Gets the number of samples in the loaded audio buffer.
     * @return Number of samples, or 0 if no audio is loaded
     */
    int getNumSamples() const { return audioBuffer.getNumSamples(); }

    /**
     * Gets the original sample rate of the loaded audio file.
     * @return The sample rate (e.g., 44100.0)
     */
    double getOriginalSampleRate() const { return originalSampleRate; }

    //==============================================================================
    // Display Information

    /**
     * Sets the display name for this sample (shown in UI).
     * @param name The display name (e.g., "FootstepLeft.wav")
     */
    void setDisplayName(const juce::String& name) { displayName = name; }

    /**
     * Gets the display name.
     * @return The display name
     */
    juce::String getDisplayName() const { return displayName; }

    /**
     * Checks if this sound has audio data loaded.
     * @return true if audio buffer contains samples
     */
    bool isLoaded() const { return audioBuffer.getNumSamples() > 0; }

    /**
     * Clears all sample data and resets to empty state.
     */
    void clearSample();

private:
    //==============================================================================
    // Member Variables

    juce::AudioBuffer<float> audioBuffer;    // The actual audio sample data (mono)
    double originalSampleRate = 44100.0;     // Sample rate of the loaded file
    juce::String displayName;                // Name to show in UI

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RRSound)
};