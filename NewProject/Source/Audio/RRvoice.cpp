#include "RRVoice.h"

//==============================================================================
// Constructor
RRVoice::RRVoice()
{
    // Set up default ADSR envelope parameters
    envelopeParams.attack = 0.01f;   // 10ms attack
    envelopeParams.decay = 0.001f;   // 1ms decay
    envelopeParams.sustain = 1.0f;   // Full sustain
    envelopeParams.release = 0.1f;   // 100ms release

    envelope.setParameters(envelopeParams);
}

// Destructor
RRVoice::~RRVoice()
{
    // Nothing to clean up - JUCE handles it
}

//==============================================================================
// Check if this voice can play the given sound
bool RRVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    // We can only play RRSound objects
    return dynamic_cast<RRSound*>(sound) != nullptr;
}

void RRVoice::updateGlobalParameters(float semitones, float cents, float attackMs, float decayMs)
{
    currentSemitones = semitones;
    currentCents = cents;
    currentAttackMs = attackMs;
    currentDecayMs = decayMs;
}

//==============================================================================
// Start playing a note
void RRVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound* sound,
    int /*currentPitchWheelPosition*/)
{
    // Cast the generic sound to our specific RRSound type
    currentSound = dynamic_cast<RRSound*>(sound);

    if (currentSound == nullptr || !currentSound->isLoaded())
    {
        isPlaying = false;
        return;
    }

    // CALCULATE GLOBAL PITCH SHIFT
    double semitonePitch = std::pow(2.0, currentSemitones / 12.0);
    double centsPitch = std::pow(2.0, currentCents / 1200.0);
    pitchRatio = semitonePitch * centsPitch;

    DBG("Starting note " + juce::String(midiNoteNumber) +
        " - Pitch: " + juce::String(currentSemitones) + " st + " +
        juce::String(currentCents) + " cents (ratio: " + juce::String(pitchRatio) +
        ") | Attack: " + juce::String(currentAttackMs) + "ms, Decay: " + juce::String(currentDecayMs) + "ms");

    // ONE-SHOT ENVELOPE: Attack → Decay → Silent
    // We use decay to fade out, and sustain at 0 so it stops naturally

    envelopeParams.attack = currentAttackMs / 1000.0f;   // Fade in time
    envelopeParams.decay = currentDecayMs / 1000.0f;     // Fade out time  
    envelopeParams.sustain = 0.0f;                       // Fade to silence
    envelopeParams.release = 0.001f;                     // Very short release (1ms)

    envelope.setParameters(envelopeParams);

    // Reset playback position
    sourceSamplePosition = 0.0;

    // Reset and start envelope from clean state
    envelope.reset();      // ← IMPORTANT: Reset first!
    envelope.noteOn();     // Start attack phase

    // DON'T call noteOff() here - we'll handle it differently
    // The envelope will play: Attack → Decay → Sustain (0) → wait

    // Mark voice as playing
    isPlaying = true;
}

//==============================================================================
// Stop playing a note
void RRVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    // ONE-SHOT BEHAVIOR: Ignore key releases
    // The envelope will play through regardless of when the key is released
    // This gives us drum machine/one-shot behavior

    if (!allowTailOff)
    {
        // Hard stop (only for emergency voice stealing)
        clearCurrentNote();
        envelope.reset();
        isPlaying = false;
    }

    // If allowTailOff is true, do NOTHING - let the envelope play through!
    // The voice will stop automatically when envelope.isActive() returns false
}

//==============================================================================
// Render the next block of audio
void RRVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
    int startSample, int numSamples)
{
    // If we're not playing, nothing to do
    if (!isPlaying || currentSound == nullptr)
        return;

    // Get the sample data from the RRSound
    const juce::AudioBuffer<float>& sampleBuffer = currentSound->getAudioBuffer();
    const float* sampleData = sampleBuffer.getReadPointer(0);
    const int sampleLength = sampleBuffer.getNumSamples();

    // Process each sample in this block
    for (int i = 0; i < numSamples; ++i)
    {
        // Check if we've reached the end of the sample
        if (sourceSamplePosition >= sampleLength)
        {
            clearCurrentNote();
            isPlaying = false;
            break;
        }

        // Get the current envelope level (0.0 to 1.0)
        const float envelopeLevel = envelope.getNextSample();

        // ONE-SHOT BEHAVIOR: If envelope has faded to (near) silence, trigger release to finish
        if (envelopeLevel < 0.0001f && envelope.isActive())
        {
            envelope.noteOff();  // Trigger short release to cleanly finish
        }

        // Linear interpolation for smooth playback
        const int index0 = static_cast<int>(sourceSamplePosition);
        const int index1 = index0 + 1;

        if (index1 >= sampleLength)
        {
            clearCurrentNote();
            isPlaying = false;
            break;
        }

        const float fraction = static_cast<float>(sourceSamplePosition - index0);
        const float sample0 = sampleData[index0];
        const float sample1 = sampleData[index1];
        const float interpolatedSample = sample0 + (sample1 - sample0) * fraction;

        // Apply envelope to the sample
        const float outputSample = interpolatedSample * envelopeLevel;

        // Write to both channels
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample + i, outputSample);
        }

        // Advance playback position
        sourceSamplePosition += pitchRatio;

        // Stop when envelope completely finishes
        if (!envelope.isActive())
        {
            clearCurrentNote();
            isPlaying = false;
            break;
        }
    }
}

//==============================================================================
// Pitch wheel moved (not implemented for MVP)
void RRVoice::pitchWheelMoved(int /*newPitchWheelValue*/)
{
    // Could implement pitch bend here in future
}

//==============================================================================
// Controller moved (not implemented for MVP)
void RRVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/)
{
    // Could implement mod wheel, expression, etc. here in future
}