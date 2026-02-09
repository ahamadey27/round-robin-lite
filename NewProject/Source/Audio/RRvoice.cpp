#include "RRvoice.h"
#include "../DSP/RandomizationEngine.h"
#include "../Parameters/ParametersIDs.h"


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

void RRVoice::setRandomizationReferences(RandomizationEngine* engine,
                                         juce::AudioProcessorValueTreeState* params)
{
    randEngine = engine;
    apvts = params;
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
    juce::SynthesiserSound* sound, int /*currentPitchWheelPosition*/)
{
    currentSound = dynamic_cast<RRSound*>(sound);
    
    if (currentSound == nullptr)
        return;

    //==========================================================================
    // GENERATE RANDOMIZED PARAMETER VALUES
    
    if (randEngine != nullptr && apvts != nullptr)
    {
        // Semitone randomization
        float semitoneNeg = apvts->getRawParameterValue(ParameterIDs::semitoneRndNeg)->load();
        float semitonePos = apvts->getRawParameterValue(ParameterIDs::semitoneRndPos)->load();
        randomizedSemitones = randEngine->generateRandomValue(globalSemitones, semitoneNeg, semitonePos);
        
        // Fine tune randomization
        float fineNeg = apvts->getRawParameterValue(ParameterIDs::fineTuneRndNeg)->load();
        float finePos = apvts->getRawParameterValue(ParameterIDs::fineTuneRndPos)->load();
        randomizedCents = randEngine->generateRandomValue(globalCents, fineNeg, finePos);
        
        // Envelope attack randomization
        float attackNeg = apvts->getRawParameterValue(ParameterIDs::envAttackRndNeg)->load();
        float attackPos = apvts->getRawParameterValue(ParameterIDs::envAttackRndPos)->load();
        randomizedAttackMs = randEngine->generateRandomValue(globalAttackMs, attackNeg, attackPos);
        
        // Envelope decay randomization
        float decayNeg = apvts->getRawParameterValue(ParameterIDs::envDecayRndNeg)->load();
        float decayPos = apvts->getRawParameterValue(ParameterIDs::envDecayRndPos)->load();
        randomizedDecayMs = randEngine->generateRandomValue(globalDecayMs, decayNeg, decayPos);
    }
    else
    {
        // No randomization - use base values
        randomizedSemitones = globalSemitones;
        randomizedCents = globalCents;
        randomizedAttackMs = globalAttackMs;
        randomizedDecayMs = globalDecayMs;
    }
    
    //==========================================================================
    // CALCULATE PITCH RATIO (using randomized values)
    
    float semitoneShift = std::pow(2.0f, randomizedSemitones / 12.0f);
    float centShift = std::pow(2.0f, randomizedCents / 1200.0f);
    pitchRatio = semitoneShift * centShift;

    //==========================================================================
    // CONFIGURE ENVELOPE (using randomized values)
    
    juce::ADSR::Parameters envParams;
    envParams.attack = randomizedAttackMs / 1000.0f;
    envParams.decay = 0.0f;
    envParams.sustain = 1.0f;
    envParams.release = randomizedDecayMs / 1000.0f;
    envelope.setParameters(envParams);
    envelope.noteOn();

    //==========================================================================
    // START PLAYBACK
    
    sourceSamplePosition = 0.0;
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