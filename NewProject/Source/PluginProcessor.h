/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Audio/RRVoice.h"
#include "Audio/RRSound.h"
#include "Parameters/ParametersIDs.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    // PARAMETER SYSTEM

    /**
     * AudioProcessorValueTreeState (APVTS) - The parameter management system
     */
    juce::AudioProcessorValueTreeState apvts;

    /**
     * Helper function to create the parameter layout.
     */
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Audio Engine
    juce::Synthesiser synthesiser;
    juce::AudioFormatManager formatManager;

    // Parameter System
    juce::AudioProcessorValueTreeState apvts;

    // Current global pitch values (updated each block)
    std::atomic<float> globalSemitones{ 0.0f };
    std::atomic<float> globalCents{ 0.0f };
    std::atomic<float> globalAttackMs{ 0.0f };
    std::atomic<float> globalDecayMs{ 100.0f };
    //==============================================================================
    // PARAMETER SMOOTHING

    /**
     * SmoothedValue objects for all parameters.
     * These prevent clicks/pops when parameters change by ramping gradually
     * from old value to new value over a short time period (typically 20-50ms).
     */

     // Pitch Controls
    juce::LinearSmoothedValue<float> smoothedSemitone;
    juce::LinearSmoothedValue<float> smoothedFineTune;

    // Volume
    juce::LinearSmoothedValue<float> smoothedVolume;

    // Amplitude Envelope
    juce::LinearSmoothedValue<float> smoothedEnvAttack;
    juce::LinearSmoothedValue<float> smoothedEnvDecay;

    // EQ Parameters
    juce::LinearSmoothedValue<float> smoothedLowGain;
    juce::LinearSmoothedValue<float> smoothedLowFreq;
    juce::LinearSmoothedValue<float> smoothedMidGain;
    juce::LinearSmoothedValue<float> smoothedMidFreq;
    juce::LinearSmoothedValue<float> smoothedHighGain;
    juce::LinearSmoothedValue<float> smoothedHighFreq;

    // Transient Master
    juce::LinearSmoothedValue<float> smoothedTransientAttack;
    juce::LinearSmoothedValue<float> smoothedTransientDecay;

    //==============================================================================
    // SYNTHESISER - The core audio engine
    juce::Synthesiser synthesiser;

    // Audio format manager for loading samples (we'll use this in Phase 4)
    juce::AudioFormatManager formatManager;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessor)
};