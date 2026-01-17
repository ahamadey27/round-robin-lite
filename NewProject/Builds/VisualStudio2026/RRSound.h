/*
  ==============================================================================

    Custom SynthesiserSound class for Round Robin Lite.
    Represents a single loaded audio sample with MIDI note mapping.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
 * RRSound - Represents a loaded audio sample for the Round Robin Lite sampler.
 * Each RRSound stores:
 * - The actual audio data (mono buffer)
 * - Which MIDI notes trigger it (key pair)
 * - The root pitch reference point
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
     * For Round Robin Lite, each sound responds to TWO white keys (a pair).
     * Example: If keyPairIndex = 7 (root pair), this responds to C2 (48) and D2 (50)
     * @param midiNoteNumber The MIDI note number (0-127)
     * @return true if this sound should play for this note
     */
    bool appliesToNote(int midiNoteNumber) override;

    /**
     * Determines if this sound responds to the given MIDI channel.
     * For Round Robin Lite, we respond to all channels (1-16).
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
     * @param formatManager Reference to the audio format manager (for reading files)
     * @return true if loading was successful, false otherwise
     */
    bool loadFromFile(const juce::File& file, juce::AudioFormatManager& formatManager);

    /**
    * Gets a reference to the audio buffer containing the sample data.
    * This is used by RRVoice to actually play the sound.
    * @return Const reference to the audio buffer
    */
    const juce::AudioBuffer<float>& getAudioBuffer() const { return audioBuffer; }

    /**
     * Gets the original sample rate of the loaded audio file.
     * Used for pitch shift calculations.
     * @return The sample rate (e.g., 44100.0)
     */
    double getOriginalSampleRate() const { return originalSampleRate; }

    //==============================================================================
    // MIDI Note Mapping

    /**
     * Sets which key pair this sound is assigned to (0-9).
     * Key Pair Index → MIDI Notes:
     * 0 → C0/D0 (36/38) = -7 semitones
     * 1 → E0/F0 (40/41) = -6 semitones
     * 2 → G0/A0 (43/45) = -5 semitones
     * 3 → B0/C1 (47/48) = -4 semitones
     * 4 → E1/F1 (52/53) = -3 semitones
     * 5 → G1/A1 (55/57) = -2 semitones
     * 6 → B1/C2 (59/60) = -1 semitone
     * 7 → C2/D2 (48/50) = 0 (ROOT - no pitch shift)
     * 8 → E2/F2 (64/65) = +1 semitone
     * 9 → G2/A2 (67/69) = +2 semitones
     * @param pairIndex The key pair index (0-9)
     */
    void setKeyPairIndex(int pairIndex);

    /**
     * Gets the current key pair index.
     * @return The key pair index (0-9, or -1 if not assigned)
     */
    int getKeyPairIndex() const { return keyPairIndex; }
};