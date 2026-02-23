#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters/ParametersIDs.h"

class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NewProjectAudioProcessorEditor(NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    NewProjectAudioProcessor& audioProcessor;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // --- Pitch ---
    juce::Slider semitoneSlider, fineTuneSlider;
    SliderAttachment semitoneAttachment, fineTuneAttachment;

    // --- Volume ---
    juce::Slider volumeSlider;
    SliderAttachment volumeAttachment;

    // --- EQ ---
    juce::Slider lowGainSlider, lowFreqSlider;
    juce::Slider midGainSlider, midFreqSlider;
    juce::Slider highGainSlider, highFreqSlider;
    SliderAttachment lowGainAttachment, lowFreqAttachment;
    SliderAttachment midGainAttachment, midFreqAttachment;
    SliderAttachment highGainAttachment, highFreqAttachment;

    // --- Transient ---
    juce::Slider transientAttackSlider, transientDecaySlider;
    SliderAttachment transientAttackAttachment, transientDecayAttachment;

    // --- Envelope ---
    juce::Slider envAttackSlider, envDecaySlider;
    SliderAttachment envAttackAttachment, envDecayAttachment;

    // --- Pan ---
    juce::Slider panSlider;
    SliderAttachment panAttachment;

    // --- Randomization ---
    juce::Slider semitoneRndNegSlider, semitoneRndPosSlider;
    juce::Slider fineTuneRndNegSlider, fineTuneRndPosSlider;
    juce::Slider volumeRndNegSlider, volumeRndPosSlider;
    juce::Slider panRndNegSlider, panRndPosSlider;
    juce::Slider lowGainRndNegSlider, lowGainRndPosSlider;
    juce::Slider lowFreqRndNegSlider, lowFreqRndPosSlider;
    juce::Slider midGainRndNegSlider, midGainRndPosSlider;
    juce::Slider midFreqRndNegSlider, midFreqRndPosSlider;
    juce::Slider highGainRndNegSlider, highGainRndPosSlider;
    juce::Slider highFreqRndNegSlider, highFreqRndPosSlider;
    juce::Slider transAtkRndNegSlider, transAtkRndPosSlider;
    juce::Slider transDecRndNegSlider, transDecRndPosSlider;

    SliderAttachment semitoneRndNegAttachment, semitoneRndPosAttachment;
    SliderAttachment fineTuneRndNegAttachment, fineTuneRndPosAttachment;
    SliderAttachment volumeRndNegAttachment, volumeRndPosAttachment;
    SliderAttachment panRndNegAttachment, panRndPosAttachment;
    SliderAttachment lowGainRndNegAttachment, lowGainRndPosAttachment;
    SliderAttachment lowFreqRndNegAttachment, lowFreqRndPosAttachment;
    SliderAttachment midGainRndNegAttachment, midGainRndPosAttachment;
    SliderAttachment midFreqRndNegAttachment, midFreqRndPosAttachment;
    SliderAttachment highGainRndNegAttachment, highGainRndPosAttachment;
    SliderAttachment highFreqRndNegAttachment, highFreqRndPosAttachment;
    SliderAttachment transAtkRndNegAttachment, transAtkRndPosAttachment;
    SliderAttachment transDecRndNegAttachment, transDecRndPosAttachment;

    // Helper to configure a basic slider
    void setupSlider(juce::Slider& slider);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};