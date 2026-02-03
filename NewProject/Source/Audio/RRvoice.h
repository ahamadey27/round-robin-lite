/*
  ==============================================================================

    Custom SynthesiserVoice class for Round Robin Lite.
    Handles playback of RRSound samples with pitch shifting and ADSR envelope.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "RRSound.h"

//==============================================================================
/**
 * RRVoice - Plays back RRSound samples when triggered by MIDI notes.
 *
 * This voice handles:
 * - Sample playback with pitch shifting
 * - ADSR envelope application
 * - Monophonic behavior (one note at a time)
 * - Smooth voice termination
 */
class RRVoice : public juce::SynthesiserVoice
{
public:
    // Constructor & Destructor
    RRVoice();
    ~RRVoice() override;

    void updateGlobalParameters(float semitones, float cents, float attackMs, float decayMs);

    //==============================================================================
    // JUCE SynthesiserVoice Required Methods

    /**
     * Checks if this voice can play the given sound.
     * We only want to play RRSound objects, not other sound types.
     * @param sound The sound to check
     * @return true if sound is an RRSound
     */
    bool canPlaySound(juce::SynthesiserSound* sound) override;

    /**
     * Called when a MIDI note-on event occurs.
     * Sets up the voice to start playing a sample.
     * @param midiNoteNumber The MIDI note (0-127)
     * @param velocity The note velocity (0.0-1.0)
     * @param sound The RRSound to play
     * @param currentPitchWheelPosition Current pitch wheel value (-1.0 to 1.0)
     */
    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound* sound,
        int currentPitchWheelPosition) override;

    /**
     * Called when a MIDI note-off event occurs.
     * Triggers the release phase of the envelope.
     * @param velocity Note-off velocity (usually ignored)
     * @param allowTailOff If true, let envelope release naturally
     */
    void stopNote(float velocity, bool allowTailOff) override;

    /**
     * Renders the next block of audio for this voice.
     * This is where the actual sample playback and processing happens.
     * @param outputBuffer The buffer to write audio into
     * @param startSample Which sample in the buffer to start at
     * @param numSamples How many samples to render
     */
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
        int startSample, int numSamples) override;

    /**
     * Called when pitch wheel is moved (not implemented for MVP).
     */
    void pitchWheelMoved(int newPitchWheelValue) override;

    /**
     * Called when controller data is received (not implemented for MVP).
     */
    void controllerMoved(int controllerNumber, int newControllerValue) override;

private:
    //==============================================================================
    // Member Variables

    // Playback state
    double sourceSamplePosition = 0.0;
    double pitchRatio = 1.0;
    bool isPlaying = false;

    // Global parameters (updated before each note)
    float currentSemitones = 0.0f;
    float currentCents = 0.0f;
    float currentAttackMs = 0.0f;
    float currentDecayMs = 100.0f;

    // ADSR Envelope
    juce::ADSR envelope;
    juce::ADSR::Parameters envelopeParams;

    // Reference to current sound
    RRSound* currentSound = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RRVoice)
};