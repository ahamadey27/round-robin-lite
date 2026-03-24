#include "RRvoice.h"
#include "../DSP/RandomizationEngine.h"
#include "../Parameters/ParametersIDs.h"

void RRSound::setFromSlot(const SampleSlot& slot)
{
    audioBuffer = slot.audioBuffer;     // copies the buffer
    originalSampleRate = slot.sampleRate;
    displayName = slot.displayName;
}

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

    rndPtrs.semitone = params->getRawParameterValue(ParameterIDs::semitone);
    rndPtrs.fineTune = params->getRawParameterValue(ParameterIDs::fineTune);
    rndPtrs.volume = params->getRawParameterValue(ParameterIDs::volume);
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //rndPtrs.envAttack = params->getRawParameterValue(ParameterIDs::envAttack);
    //rndPtrs.envDecay = params->getRawParameterValue(ParameterIDs::envDecay);
    //rndPtrs.lowGain = params->getRawParameterValue(ParameterIDs::lowGain);
    //rndPtrs.lowFreq = params->getRawParameterValue(ParameterIDs::lowFreq);
    //rndPtrs.midGain = params->getRawParameterValue(ParameterIDs::midGain);
    //rndPtrs.midFreq = params->getRawParameterValue(ParameterIDs::midFreq);
    //rndPtrs.highGain = params->getRawParameterValue(ParameterIDs::highGain);
    //rndPtrs.highFreq = params->getRawParameterValue(ParameterIDs::highFreq);
    //rndPtrs.transAtk = params->getRawParameterValue(ParameterIDs::transientAttack);
    //rndPtrs.transDec = params->getRawParameterValue(ParameterIDs::transientDecay);

    rndPtrs.semitoneNeg = params->getRawParameterValue(ParameterIDs::semitoneRndNeg);
    rndPtrs.semitonePos = params->getRawParameterValue(ParameterIDs::semitoneRndPos);
    rndPtrs.fineNeg = params->getRawParameterValue(ParameterIDs::fineTuneRndNeg);
    rndPtrs.finePos = params->getRawParameterValue(ParameterIDs::fineTuneRndPos);
    rndPtrs.volumeNeg = params->getRawParameterValue(ParameterIDs::volumeRndNeg);
    rndPtrs.volumePos = params->getRawParameterValue(ParameterIDs::volumeRndPos);
    rndPtrs.pan = params->getRawParameterValue(ParameterIDs::pan);
    rndPtrs.panNeg = params->getRawParameterValue(ParameterIDs::panRndNeg);
    rndPtrs.panPos = params->getRawParameterValue(ParameterIDs::panRndPos);
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //rndPtrs.atkNeg = params->getRawParameterValue(ParameterIDs::envAttackRndNeg);
    //rndPtrs.atkPos = params->getRawParameterValue(ParameterIDs::envAttackRndPos);
    //rndPtrs.decNeg = params->getRawParameterValue(ParameterIDs::envDecayRndNeg);
    //rndPtrs.decPos = params->getRawParameterValue(ParameterIDs::envDecayRndPos);
    //rndPtrs.lowGainNeg = params->getRawParameterValue(ParameterIDs::lowGainRndNeg);
    //rndPtrs.lowGainPos = params->getRawParameterValue(ParameterIDs::lowGainRndPos);
    //rndPtrs.lowFreqNeg = params->getRawParameterValue(ParameterIDs::lowFreqRndNeg);
    //rndPtrs.lowFreqPos = params->getRawParameterValue(ParameterIDs::lowFreqRndPos);
    //rndPtrs.midGainNeg = params->getRawParameterValue(ParameterIDs::midGainRndNeg);
    //rndPtrs.midGainPos = params->getRawParameterValue(ParameterIDs::midGainRndPos);
    //rndPtrs.midFreqNeg = params->getRawParameterValue(ParameterIDs::midFreqRndNeg);
    //rndPtrs.midFreqPos = params->getRawParameterValue(ParameterIDs::midFreqRndPos);
    //rndPtrs.highGainNeg = params->getRawParameterValue(ParameterIDs::highGainRndNeg);
    //rndPtrs.highGainPos = params->getRawParameterValue(ParameterIDs::highGainRndPos);
    //rndPtrs.highFreqNeg = params->getRawParameterValue(ParameterIDs::highFreqRndNeg);
    //rndPtrs.highFreqPos = params->getRawParameterValue(ParameterIDs::highFreqRndPos);
    //rndPtrs.transAtkNeg = params->getRawParameterValue(ParameterIDs::transientAttackRndNeg);
    //rndPtrs.transAtkPos = params->getRawParameterValue(ParameterIDs::transientAttackRndPos);
    //rndPtrs.transDecNeg = params->getRawParameterValue(ParameterIDs::transientDecayRndNeg);
    //rndPtrs.transDecPos = params->getRawParameterValue(ParameterIDs::transientDecayRndPos);
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
        randomizedSemitones = randEngine->generateRandomValue(
            rndPtrs.semitone->load(),
            rndPtrs.semitoneNeg->load(),
            rndPtrs.semitonePos->load());

        // Fine tune randomization
        randomizedCents = randEngine->generateRandomValue(
            rndPtrs.fineTune->load(),
            rndPtrs.fineNeg->load(),
            rndPtrs.finePos->load());

        // Volume Randomization 
        float baseVolumeLinear = rndPtrs.volume->load();
        float baseVolumedB = juce::Decibels::gainToDecibels(baseVolumeLinear, -60.0f);
        float volNegdB = rndPtrs.volumeNeg->load() * 24.0f;  // scale 0-1 → 0-24dB
        float volPosdB = rndPtrs.volumePos->load() * 24.0f;  // scale 0-1 → 0-24dB
        float randomizeddB = randEngine->generateRandomValue(baseVolumedB, volNegdB, volPosdB);
        randomizeddB = juce::jlimit(-60.0f, 12.0f, randomizeddB);
        randomizedVolume = juce::Decibels::decibelsToGain(randomizeddB);

        //Pan Randomization
        randomizedPan = randEngine->generateRandomValue(
            rndPtrs.pan->load(),
            rndPtrs.panNeg->load(),
            rndPtrs.panPos->load());
        randomizedPan = juce::jlimit(-1.0f, 1.0f, randomizedPan);

        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        // Envelope attack randomization
        //randomizedAttackMs = randEngine->generateRandomValue(
        //    rndPtrs.envAttack->load(),
        //    rndPtrs.atkNeg->load(),
        //    rndPtrs.atkPos->load());

        // Envelope decay randomization
        //randomizedDecayMs = randEngine->generateRandomValue(
        //    rndPtrs.envDecay->load(),
        //    rndPtrs.decNeg->load(),
        //    rndPtrs.decPos->load());

        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        // Low EQ randomization
        //randomizedLowGain = randEngine->generateRandomValue(
        //    rndPtrs.lowGain->load(),
        //    rndPtrs.lowGainNeg->load(),
        //    rndPtrs.lowGainPos->load());
        //randomizedLowGain = juce::jlimit(-24.0f, 24.0f, randomizedLowGain);

        //randomizedLowFreq = randEngine->generateRandomValue(
        //    rndPtrs.lowFreq->load(),
        //    rndPtrs.lowFreqNeg->load(),
        //    rndPtrs.lowFreqPos->load());
        //randomizedLowFreq = juce::jlimit(20.0f, 500.0f, randomizedLowFreq);

        // Mid EQ randomization
        //randomizedMidGain = randEngine->generateRandomValue(
        //    rndPtrs.midGain->load(),
        //    rndPtrs.midGainNeg->load(),
        //    rndPtrs.midGainPos->load());
        //randomizedMidGain = juce::jlimit(-24.0f, 24.0f, randomizedMidGain);

        //randomizedMidFreq = randEngine->generateRandomValue(
        //    rndPtrs.midFreq->load(),
        //    rndPtrs.midFreqNeg->load(),
        //    rndPtrs.midFreqPos->load());
        //randomizedMidFreq = juce::jlimit(200.0f, 5000.0f, randomizedMidFreq);

        // High EQ randomization
        //randomizedHighGain = randEngine->generateRandomValue(
        //    rndPtrs.highGain->load(),
        //    rndPtrs.highGainNeg->load(),
        //    rndPtrs.highGainPos->load());
        //randomizedHighGain = juce::jlimit(-24.0f, 24.0f, randomizedHighGain);

        //randomizedHighFreq = randEngine->generateRandomValue(
        //    rndPtrs.highFreq->load(),
        //    rndPtrs.highFreqNeg->load(),
        //    rndPtrs.highFreqPos->load());
        //randomizedHighFreq = juce::jlimit(2000.0f, 20000.0f, randomizedHighFreq);

        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        // Transient randomization
        //randomizedTransientAttack = randEngine->generateRandomValue(
        //    rndPtrs.transAtk->load(),
        //    rndPtrs.transAtkNeg->load(),
        //    rndPtrs.transAtkPos->load());
        //randomizedTransientAttack = juce::jlimit(-127.0f, 127.0f, randomizedTransientAttack);

        //randomizedTransientDecay = randEngine->generateRandomValue(
        //    rndPtrs.transDec->load(),
        //    rndPtrs.transDecNeg->load(),
        //    rndPtrs.transDecPos->load());
        //randomizedTransientDecay = juce::jlimit(-127.0f, 127.0f, randomizedTransientDecay);
    }
    else
    {
        randomizedVolume = 0.75f;
        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        //randomizedLowGain = 0.0f;
        //randomizedLowFreq = 100.0f;
        //randomizedMidGain = 0.0f;
        //randomizedMidFreq = 1000.0f;
        //randomizedHighGain = 0.0f;
        //randomizedHighFreq = 5000.0f;
        //randomizedTransientAttack = 0.0f;
        //randomizedTransientDecay = 0.0f;
    }

    //==========================================================================
    // CALCULATE PITCH RATIO (using randomized values)

    if (randEngine == nullptr)
    {
        randomizedSemitones = currentSemitones;
        randomizedCents = currentCents;
    }

    float semitoneShift = std::pow(2.0f, randomizedSemitones / 12.0f);
    float centShift = std::pow(2.0f, randomizedCents / 1200.0f);
    pitchRatio = semitoneShift * centShift;

  
    //==========================================================================
    // START PLAYBACK

    sourceSamplePosition = 0.0;

    // cache audio data so buffer swaps can't corrupt playback
    const auto& buf = currentSound->getAudioBuffer();
    voiceBuffer.makeCopyOf(buf);           // deep copy — safe from buffer swaps
    cachedSampleLength = voiceBuffer.getNumSamples();

    // AD envelope: attack fade-in, decay fade-out to zero, no sustain
    envelopeParams.attack = juce::jlimit(0.001f, 5.0f, randomizedAttackMs / 1000.0f);
    envelopeParams.decay = juce::jlimit(0.001f, 10.0f, randomizedDecayMs / 1000.0f);
    envelopeParams.sustain = 0.0f;    // ← KEY: decays all the way to zero automatically
    envelopeParams.release = 0.001f;  // negligible - not used in one-shot mode
    envelope.setParameters(envelopeParams);

    envelope.reset();
    envelope.noteOn();

   

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
        clearCurrentNote();
        envelope.reset();
        isPlaying = false;
        voiceBuffer.setSize(0, 0);   // REPLACE cachedSampleData = nullptr
        cachedSampleLength = 0;
    }

    // If allowTailOff is true, do NOTHING - let the envelope play through!
    // The voice will stop automatically when envelope.isActive() returns false
}

//==============================================================================
// Render the next block of audio
void RRVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
    int startSample, int numSamples)
{
    if (!isPlaying || voiceBuffer.getNumSamples() == 0 || cachedSampleLength == 0)
        return;

    const float* sampleData = voiceBuffer.getReadPointer(0);
    const int sampleLength = cachedSampleLength;

    for (int i = 0; i < numSamples; ++i)
    {
        const float envelopeLevel = envelope.getNextSample();  // AD curve, no manual noteOff needed

        float outputSample = 0.0f;

        if (sourceSamplePosition < sampleLength - 1)
        {
            const int index0 = static_cast<int>(sourceSamplePosition);
            const int index1 = index0 + 1;
            const float fraction = static_cast<float>(sourceSamplePosition - index0);
            const float sample0 = sampleData[index0];
            const float sample1 = sampleData[index1];
            outputSample = (sample0 + (sample1 - sample0) * fraction) * envelopeLevel * randomizedVolume;
            sourceSamplePosition += pitchRatio;
        }
        else
        {
            if (!envelope.isActive())
            {
                isPlaying = false;
                voiceBuffer.setSize(0, 0);
                cachedSampleLength = 0;
                clearCurrentNote();
                return;
            }
            outputSample = 0.0f;  // envelope tail over silence
        }

        float panAngle = (randomizedPan + 1.0f) * 0.5f * juce::MathConstants<float>::halfPi;
        float leftGain = std::cos(panAngle);
        float rightGain = std::sin(panAngle);

        if (outputBuffer.getNumChannels() >= 2)
        {
            outputBuffer.addSample(0, startSample + i, outputSample * leftGain);
            outputBuffer.addSample(1, startSample + i, outputSample * rightGain);
        }
        else
        {
            outputBuffer.addSample(0, startSample + i, outputSample);
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
