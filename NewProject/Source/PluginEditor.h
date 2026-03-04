#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters/ParametersIDs.h"
#include "UI/RRLookAndFeel.h"  

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
    void paintOverChildren(juce::Graphics&) override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

private:
    NewProjectAudioProcessor& audioProcessor;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    // LAFs first — destroyed last (sliders must be gone before LAFs)
    RRKnobLAF      knobLAF;
    RRNegSliderLAF negSliderLAF;
    RRPosSliderLAF posSliderLAF;

    // Viewport
    juce::Viewport  viewport;
    LabelledContent contentComponent;

    // Buttons & labels
    juce::TextButton loadSamplesButton;
    juce::TextButton savePresetButton;
    juce::TextButton loadPresetButton;
    juce::TextButton playbackModeButton;
    juce::Label      samplesInfoLabel;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Button attachment (after its button)
    ButtonAttachment playbackModeAttachment;

    // Main sliders — before their attachments
    juce::Slider semitoneSlider, fineTuneSlider;
    juce::Slider volumeSlider, panSlider;
    juce::Slider lowGainSlider, lowFreqSlider;
    juce::Slider midGainSlider, midFreqSlider;
    juce::Slider highGainSlider, highFreqSlider;
    juce::Slider transientAttackSlider, transientDecaySlider;
    juce::Slider envAttackSlider, envDecaySlider;

    // Main attachments — after their sliders
    SliderAttachment semitoneAttachment, fineTuneAttachment;
    SliderAttachment volumeAttachment, panAttachment;
    SliderAttachment lowGainAttachment, lowFreqAttachment;
    SliderAttachment midGainAttachment, midFreqAttachment;
    SliderAttachment highGainAttachment, highFreqAttachment;
    SliderAttachment transientAttackAttachment, transientDecayAttachment;
    SliderAttachment envAttackAttachment, envDecayAttachment;

    // Rnd sliders — before their attachments
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
    juce::Slider envAtkRndNegSlider, envAtkRndPosSlider;
    juce::Slider envDecRndNegSlider, envDecRndPosSlider;

    // Rnd attachments — after their sliders
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
    SliderAttachment envAtkRndNegAttachment, envAtkRndPosAttachment;
    SliderAttachment envDecRndNegAttachment, envDecRndPosAttachment;

    // Helpers
    void setupSlider(juce::Slider& s);
    void setupKnob(juce::Slider& s);
    void loadSamplesFromFiles();
    void updateSamplesInfo();
    void savePreset();
    void loadPreset();

    // Arc drag state
    juce::Slider* activeRndSlider = nullptr;
    float         arcDragStartY = 0.0f;
    float         arcDragStartVal = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};