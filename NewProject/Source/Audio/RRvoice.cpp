#include "RRVoice.h"

//==============================================================================
// Constructor
RRVoice::RRVoice()
{
    // Set up default ADSR envelope parameters
    envelopeParams.attack = 0.01f;   // 10ms attack
    envelopeParams.decay = 0.1f;     // 100ms decay
    envelopeParams.sustain = 1.0f;   // Full sustain level
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
        // No valid sound to play
        isPlaying = false;
        return;
    }

    // UNPITCHED PLAYBACK: Always play at original speed (no pitch shifting)
    // Round Robin Lite plays all samples at their natural pitch
    pitchRatio = 1.0;

    DBG("Starting note " + juce::String(midiNoteNumber) +
        " - UNPITCHED playback (ratio: 1.0)");

    // Reset playback to beginning of sample
    sourceSamplePosition = 0.0;

    // Trigger the ADSR envelope's attack phase
    envelope.noteOn();

    // Mark voice as playing
    isPlaying = true;
}

//==============================================================================
// Stop playing a note
void RRVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        // Let the envelope fade out naturally (release phase)
        envelope.noteOff();
    }
    else
    {
        // Stop immediately (hard cut)
        clearCurrentNote();
        envelope.reset();
        isPlaying = false;
    }
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
    const float* sampleData = sampleBuffer.getReadPointer(0); // Channel 0 (mono)
    const int sampleLength = sampleBuffer.getNumSamples();

    // Process each sample in this block
    for (int i = 0; i < numSamples; ++i)
    {
        // Check if we've reached the end of the sample
        if (sourceSamplePosition >= sampleLength)
        {
            // Sample finished playing
            clearCurrentNote();
            isPlaying = false;
            break;
        }

        // Get the current envelope level (0.0 to 1.0)
        const float envelopeLevel = envelope.getNextSample();

        // Linear interpolation for smooth playback
        const int index0 = static_cast<int>(sourceSamplePosition);
        const int index1 = index0 + 1;

        if (index1 >= sampleLength)
        {
            // Near end of sample, no interpolation needed
            clearCurrentNote();
            isPlaying = false;
            break;
        }

        // Linear interpolation formula: y = y0 + (y1 - y0) * fraction
        const float fraction = static_cast<float>(sourceSamplePosition - index0);
        const float sample0 = sampleData[index0];
        const float sample1 = sampleData[index1];
        const float interpolatedSample = sample0 + (sample1 - sample0) * fraction;

        // Apply envelope to the sample
        const float outputSample = interpolatedSample * envelopeLevel;

        // Write to output buffer (add, don't replace - allows multiple voices)
        // For stereo output, write to both channels
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample + i, outputSample);
        }

        // Advance the playback position by the pitch ratio
        // (For now, always 1.0 = original speed/pitch)
        sourceSamplePosition += pitchRatio;

        // Check if envelope has finished (release phase complete)
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