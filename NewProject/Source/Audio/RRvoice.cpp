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

        // Volume randomization
        float baseVolume = apvts->getRawParameterValue(ParameterIDs::volume)->load();
        float volumeNeg = apvts->getRawParameterValue(ParameterIDs::volumeRndNeg)->load();
        float volumePos = apvts->getRawParameterValue(ParameterIDs::volumeRndPos)->load();
        randomizedVolume = randEngine->generateRandomValue(baseVolume, volumeNeg, volumePos);
        randomizedVolume = juce::jlimit(0.0f, 1.0f, randomizedVolume); // Clamp

        // Low EQ randomization
        float baseLowGain = apvts->getRawParameterValue(ParameterIDs::lowGain)->load();
        float lowGainNeg = apvts->getRawParameterValue(ParameterIDs::lowGainRndNeg)->load();
        float lowGainPos = apvts->getRawParameterValue(ParameterIDs::lowGainRndPos)->load();
        randomizedLowGain = randEngine->generateRandomValue(baseLowGain, lowGainNeg, lowGainPos);
        randomizedLowGain = juce::jlimit(-24.0f, 24.0f, randomizedLowGain);

        float baseLowFreq = apvts->getRawParameterValue(ParameterIDs::lowFreq)->load();
        float lowFreqNeg = apvts->getRawParameterValue(ParameterIDs::lowFreqRndNeg)->load();
        float lowFreqPos = apvts->getRawParameterValue(ParameterIDs::lowFreqRndPos)->load();
        randomizedLowFreq = randEngine->generateRandomValue(baseLowFreq, lowFreqNeg, lowFreqPos);
        randomizedLowFreq = juce::jlimit(20.0f, 500.0f, randomizedLowFreq);

        // Mid EQ randomization
        float baseMidGain = apvts->getRawParameterValue(ParameterIDs::midGain)->load();
        float midGainNeg = apvts->getRawParameterValue(ParameterIDs::midGainRndNeg)->load();
        float midGainPos = apvts->getRawParameterValue(ParameterIDs::midGainRndPos)->load();
        randomizedMidGain = randEngine->generateRandomValue(baseMidGain, midGainNeg, midGainPos);
        randomizedMidGain = juce::jlimit(-24.0f, 24.0f, randomizedMidGain);

        float baseMidFreq = apvts->getRawParameterValue(ParameterIDs::midFreq)->load();
        float midFreqNeg = apvts->getRawParameterValue(ParameterIDs::midFreqRndNeg)->load();
        float midFreqPos = apvts->getRawParameterValue(ParameterIDs::midFreqRndPos)->load();
        randomizedMidFreq = randEngine->generateRandomValue(baseMidFreq, midFreqNeg, midFreqPos);
        randomizedMidFreq = juce::jlimit(200.0f, 5000.0f, randomizedMidFreq);

        // High EQ randomization
        float baseHighGain = apvts->getRawParameterValue(ParameterIDs::highGain)->load();
        float highGainNeg = apvts->getRawParameterValue(ParameterIDs::highGainRndNeg)->load();
        float highGainPos = apvts->getRawParameterValue(ParameterIDs::highGainRndPos)->load();
        randomizedHighGain = randEngine->generateRandomValue(baseHighGain, highGainNeg, highGainPos);
        randomizedHighGain = juce::jlimit(-24.0f, 24.0f, randomizedHighGain);

        float baseHighFreq = apvts->getRawParameterValue(ParameterIDs::highFreq)->load();
        float highFreqNeg = apvts->getRawParameterValue(ParameterIDs::highFreqRndNeg)->load();
        float highFreqPos = apvts->getRawParameterValue(ParameterIDs::highFreqRndPos)->load();
        randomizedHighFreq = randEngine->generateRandomValue(baseHighFreq, highFreqNeg, highFreqPos);
        randomizedHighFreq = juce::jlimit(2000.0f, 20000.0f, randomizedHighFreq);

        // Transient randomization
        float baseTransAtk = static_cast<float>(apvts->getRawParameterValue(ParameterIDs::transientAttack)->load());
        float transAtkNeg = static_cast<float>(apvts->getRawParameterValue(ParameterIDs::transientAttackRndNeg)->load());
        float transAtkPos = static_cast<float>(apvts->getRawParameterValue(ParameterIDs::transientAttackRndPos)->load());
        randomizedTransientAttack = randEngine->generateRandomValue(baseTransAtk, transAtkNeg, transAtkPos);
        randomizedTransientAttack = juce::jlimit(-127.0f, 127.0f, randomizedTransientAttack);

        float baseTransDec = static_cast<float>(apvts->getRawParameterValue(ParameterIDs::transientDecay)->load());
        float transDecNeg = static_cast<float>(apvts->getRawParameterValue(ParameterIDs::transientDecayRndNeg)->load());
        float transDecPos = static_cast<float>(apvts->getRawParameterValue(ParameterIDs::transientDecayRndPos)->load());
        randomizedTransientDecay = randEngine->generateRandomValue(baseTransDec, transDecNeg, transDecPos);
        randomizedTransientDecay = juce::jlimit(-127.0f, 127.0f, randomizedTransientDecay);
    }
    else
    {
        randomizedVolume = 0.75f;
        randomizedLowGain = 0.0f;
        randomizedLowFreq = 100.0f;
        randomizedMidGain = 0.0f;
        randomizedMidFreq = 1000.0f;
        randomizedHighGain = 0.0f;
        randomizedHighFreq = 5000.0f;
        randomizedTransientAttack = 0.0f;
        randomizedTransientDecay = 0.0f;
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

        // Apply envelope AND per-voice volume to the sample
        const float outputSample = interpolatedSample * envelopeLevel * randomizedVolume;

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