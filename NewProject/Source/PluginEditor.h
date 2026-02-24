#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters/ParametersIDs.h"

//= ============================================================================ =
class LabelledContent : public juce::Component
{
public:
    std::vector<std::pair<int, juce::String>> labelPositions; // y -> label text

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
        g.setColour(juce::Colours::white);
        g.setFont(11.5f);
        for (auto& [y, text] : labelPositions)
            g.drawText(text, 5, y + 4, 108, 20, juce::Justification::left, true);
    }
};

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

    //==========================================================================
    // Viewport for scrolling
    juce::Viewport viewport;
    LabelledContent contentComponent;

    //==========================================================================
    // Sample slot load buttons (20 slots)
    juce::TextButton loadButtons[20];
    std::unique_ptr<juce::FileChooser> fileChooser;

    //==========================================================================
    // Base parameter sliders — declared BEFORE attachments
    juce::Slider semitoneSlider, fineTuneSlider;
    juce::Slider volumeSlider, panSlider;
    juce::Slider lowGainSlider, lowFreqSlider;
    juce::Slider midGainSlider, midFreqSlider;
    juce::Slider highGainSlider, highFreqSlider;
    juce::Slider transientAttackSlider, transientDecaySlider;
    juce::Slider envAttackSlider, envDecaySlider;

    // Attachments AFTER their sliders (destroyed before sliders — required by JUCE)
    SliderAttachment semitoneAttachment, fineTuneAttachment;
    SliderAttachment volumeAttachment, panAttachment;
    SliderAttachment lowGainAttachment, lowFreqAttachment;
    SliderAttachment midGainAttachment, midFreqAttachment;
    SliderAttachment highGainAttachment, highFreqAttachment;
    SliderAttachment transientAttackAttachment, transientDecayAttachment;
    SliderAttachment envAttackAttachment, envDecayAttachment;

    //==========================================================================
    // Randomization sliders — declared BEFORE attachments
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

    // Attachments AFTER their sliders
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

    //==========================================================================
    void setupSlider(juce::Slider& s);
    void loadSampleForSlot(int slotIndex);
    void updateSlotLabels();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};