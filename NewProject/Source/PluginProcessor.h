/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Audio/RRVoice.h"
#include "Audio/RRSound.h"
#include "Data/SampleSlot.h"
#include "Data/SampleLoader.h"
#include "Parameters/ParametersIDs.h"
#include "DSP/ThreeBandEQ.h" 
#include "DSP/TransientShaper.h"  
#include "DSP/RandomizationEngine.h"



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

    juce::AudioProcessorValueTreeState apvts;

    static constexpr int NUM_SAMPLE_SLOTS = 20;
    SampleSlot sampleSlots[NUM_SAMPLE_SLOTS];
    SampleLoader sampleLoader{ formatManager, synthesiser, sampleSlots, NUM_SAMPLE_SLOTS };

    std::vector<int> loadedSlotIndices;   // indices of non-empty slots
    int roundRobinIndex = 0;

    void advanceRoundRobin();
    void rebuildLoadedIndices();

private:
    //==============================================================================
    // Parameter Layout Creation
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    // Audio Engine
    juce::Synthesiser synthesiser;
    juce::AudioFormatManager formatManager;

    

    // DSP Processors
    ThreeBandEQ threeBandEQ;  
    TransientShaper transientShaper;

    // Current global pitch values (updated each block)
    std::atomic<float> globalSemitones{ 0.0f };
    std::atomic<float> globalCents{ 0.0f };
    std::atomic<float> globalAttackMs{ 0.0f };
    std::atomic<float> globalDecayMs{ 100.0f };

    // Parameter Smoothing
    juce::LinearSmoothedValue<float> smoothedSemitone;
    juce::LinearSmoothedValue<float> smoothedFineTune;
    juce::LinearSmoothedValue<float> smoothedVolume;
    juce::LinearSmoothedValue<float> smoothedEnvAttack;
    juce::LinearSmoothedValue<float> smoothedEnvDecay;
    juce::LinearSmoothedValue<float> smoothedTransientAttack;
    juce::LinearSmoothedValue<float> smoothedTransientDecay;

    RandomizationEngine randomizationEngine;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessor)
};