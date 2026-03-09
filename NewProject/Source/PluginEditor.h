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

    // AboutWindow — inline popup component
    class AboutWindow : public juce::Component
    {
    public:
        AboutWindow()
        {
            closeButton.setButtonText("Close");
            closeButton.onClick = [this] { setVisible(false); };
            addAndMakeVisible(closeButton);
            setSize(340, 200);
        }

        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colour(40, 40, 50));
            g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.f);
            g.setColour(juce::Colour(100, 180, 255));
            g.setFont(juce::Font(13.f).boldened());
            g.drawText("Round Robin Lite", 0, 18, getWidth(), 20, juce::Justification::centred);
            g.setColour(juce::Colour(180, 180, 195));
            g.setFont(juce::Font(11.f));
            juce::String body =
                "Load up to 20 one-shot samples. Each sample is mapped\n"
                "to a white-key pair (C0-C4), root pitch at C2.\n\n"
                "Use the knobs to control pitch, volume, pan, EQ,\n"
                "and transients. Drag the red/blue arc dots to set\n"
                "per-note randomization ranges.";
            g.drawFittedText(body, 20, 50, getWidth() - 40, 110,
                juce::Justification::centredTop, 6);
        }

        void resized() override
        {
            closeButton.setBounds(getWidth() / 2 - 40, getHeight() - 36, 80, 24);
        }

    private:
        juce::TextButton closeButton;
    };

    AboutWindow      aboutWindow;
    juce::TextButton aboutButton;


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

        float dragStartX = 0.f;
        bool  activeIsNeg = false;   // true = neg (blue) dot grabbed
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