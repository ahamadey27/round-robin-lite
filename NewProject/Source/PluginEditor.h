#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters/ParametersIDs.h"
#include "UI/RRLookAndFeel.h"  

class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NewProjectAudioProcessorEditor(NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    void paintOverChildren(juce::Graphics&) override;

private:
    NewProjectAudioProcessor& audioProcessor;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    // LAFs first — destroyed last
    RRKnobLAF      knobLAF;
    RRNegSliderLAF negSliderLAF;
    RRPosSliderLAF posSliderLAF;
    RRToggleLAF    toggleLAF;


    // Buttons & labels
    juce::TextButton loadSamplesButton;
    juce::TextButton savePresetButton;
    juce::TextButton loadPresetButton;
    juce::TextButton playbackModeButton;
    juce::Label      samplesInfoLabel;
    juce::Label      playbackTypeLabel;
    std::unique_ptr<juce::FileChooser> fileChooser;
    ButtonAttachment playbackModeAttachment;

    // Main sliders — before attachments
    juce::Slider semitoneSlider, fineTuneSlider;
    juce::Slider volumeSlider, panSlider;
    juce::Slider lowGainSlider, lowFreqSlider;
    juce::Slider midGainSlider, midFreqSlider;
    juce::Slider highGainSlider, highFreqSlider;
    juce::Slider transientAttackSlider, transientDecaySlider;
    juce::Slider envAttackSlider, envDecaySlider;

    SliderAttachment semitoneAttachment, fineTuneAttachment;
    SliderAttachment volumeAttachment, panAttachment;
    SliderAttachment lowGainAttachment, lowFreqAttachment;
    SliderAttachment midGainAttachment, midFreqAttachment;
    SliderAttachment highGainAttachment, highFreqAttachment;
    SliderAttachment transientAttackAttachment, transientDecayAttachment;
    SliderAttachment envAttackAttachment, envDecayAttachment;

    // Rnd sliders — before attachments
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

    // Overlay AFTER all sliders — accesses slider members via editor reference
    struct RndArcOverlay : public juce::Component
    {
        NewProjectAudioProcessorEditor& editor;
        explicit RndArcOverlay(NewProjectAudioProcessorEditor& e) : editor(e) {}

        bool hitTest(int x, int y) override;
        void mouseDown(const juce::MouseEvent& e) override;
        void mouseDrag(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent& e) override;

        juce::Slider* activeSlider = nullptr;
        float         dragStartY = 0.0f;
        float         dragStartVal = 0.0f;
    } arcOverlay{ *this };

    // Helpers
    void setupSlider(juce::Slider& s);
    void setupKnob(juce::Slider& s);
    void loadSamplesFromFiles();
    void updateSamplesInfo();
    void savePreset();
    void loadPreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};