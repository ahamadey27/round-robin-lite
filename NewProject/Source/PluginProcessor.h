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

    //==============================================================================
    // Public Data (accessed by PluginEditor)
    juce::AudioProcessorValueTreeState apvts;

    static constexpr int NUM_SAMPLE_SLOTS = 20;
    SampleSlot sampleSlots[NUM_SAMPLE_SLOTS];
    std::vector<int> loadedSlotIndices;
    int roundRobinIndex = 0;

    std::vector<int> shuffledIndices;     // Fisher-Yates shuffled copy
    void reshuffleIndices();              // add declaration

    void advanceRoundRobin();
    void rebuildLoadedIndices();

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    // Audio Engine — ORDER MATTERS: synthesiser and formatManager must come before sampleLoader
    juce::Synthesiser synthesiser;
    juce::AudioFormatManager formatManager;

public:
    // Declared after private dependencies so it initializes after them
    SampleLoader sampleLoader{ formatManager, synthesiser, sampleSlots, NUM_SAMPLE_SLOTS };

private:
    //==============================================================================
    // DSP Processors
    ThreeBandEQ threeBandEQ;
    TransientShaper transientShaper;

    // Current global pitch values
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