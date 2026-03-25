#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters/ParametersIDs.h"
#include "UI/RRLookAndFeel.h"
#include "UI/SampleManagerPanel.h"

class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::ComponentListener
{
public:
    NewProjectAudioProcessorEditor(NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    void paintOverChildren(juce::Graphics&) override;
    void componentVisibilityChanged(juce::Component&) override;

private:
    NewProjectAudioProcessor& audioProcessor;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    // LAFs first � destroyed last
    RRKnobLAF      knobLAF;
    RRNegSliderLAF negSliderLAF;
    RRPosSliderLAF posSliderLAF;


    // Buttons & labels
    juce::TextButton savePresetButton;
    juce::TextButton loadPresetButton;
    std::unique_ptr<juce::FileChooser> fileChooser;

    // Main sliders � before attachments
    juce::Slider semitoneSlider, fineTuneSlider;
    juce::Slider volumeSlider, panSlider;
    juce::Slider toneLowSlider, toneHighSlider;
    juce::Slider sampleStartSlider, sampleEndSlider;
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //juce::Slider lowGainSlider, lowFreqSlider;
    //juce::Slider midGainSlider, midFreqSlider;
    //juce::Slider highGainSlider, highFreqSlider;
    //juce::Slider transientAttackSlider, transientDecaySlider;
    //juce::Slider envAttackSlider, envDecaySlider;

    SliderAttachment semitoneAttachment, fineTuneAttachment;
    SliderAttachment volumeAttachment, panAttachment;
    SliderAttachment toneLowAttachment, toneHighAttachment;
    SliderAttachment sampleStartAttachment, sampleEndAttachment;
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //SliderAttachment lowGainAttachment, lowFreqAttachment;
    //SliderAttachment midGainAttachment, midFreqAttachment;
    //SliderAttachment highGainAttachment, highFreqAttachment;
    //SliderAttachment transientAttackAttachment, transientDecayAttachment;
    //SliderAttachment envAttackAttachment, envDecayAttachment;

    // Rnd sliders � before attachments
    juce::Slider semitoneRndNegSlider, semitoneRndPosSlider;
    juce::Slider fineTuneRndNegSlider, fineTuneRndPosSlider;
    juce::Slider volumeRndNegSlider, volumeRndPosSlider;
    juce::Slider panRndNegSlider, panRndPosSlider;
    juce::Slider toneLowRndNegSlider, toneLowRndPosSlider;
    juce::Slider toneHighRndNegSlider, toneHighRndPosSlider;
    juce::Slider sampleStartRndNegSlider, sampleStartRndPosSlider;
    juce::Slider sampleEndRndNegSlider, sampleEndRndPosSlider;
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //juce::Slider lowGainRndNegSlider, lowGainRndPosSlider;
    //juce::Slider lowFreqRndNegSlider, lowFreqRndPosSlider;
    //juce::Slider midGainRndNegSlider, midGainRndPosSlider;
    //juce::Slider midFreqRndNegSlider, midFreqRndPosSlider;
    //juce::Slider highGainRndNegSlider, highGainRndPosSlider;
    //juce::Slider highFreqRndNegSlider, highFreqRndPosSlider;
    //juce::Slider transAtkRndNegSlider, transAtkRndPosSlider;
    //juce::Slider transDecRndNegSlider, transDecRndPosSlider;
    //juce::Slider envAtkRndNegSlider, envAtkRndPosSlider;
    //juce::Slider envDecRndNegSlider, envDecRndPosSlider;

    SliderAttachment semitoneRndNegAttachment, semitoneRndPosAttachment;
    SliderAttachment fineTuneRndNegAttachment, fineTuneRndPosAttachment;
    SliderAttachment volumeRndNegAttachment, volumeRndPosAttachment;
    SliderAttachment panRndNegAttachment, panRndPosAttachment;
    SliderAttachment toneLowRndNegAttachment, toneLowRndPosAttachment;
    SliderAttachment toneHighRndNegAttachment, toneHighRndPosAttachment;
    SliderAttachment sampleStartRndNegAttachment, sampleStartRndPosAttachment;
    SliderAttachment sampleEndRndNegAttachment, sampleEndRndPosAttachment;
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //SliderAttachment lowGainRndNegAttachment, lowGainRndPosAttachment;
    //SliderAttachment lowFreqRndNegAttachment, lowFreqRndPosAttachment;
    //SliderAttachment midGainRndNegAttachment, midGainRndPosAttachment;
    //SliderAttachment midFreqRndNegAttachment, midFreqRndPosAttachment;
    //SliderAttachment highGainRndNegAttachment, highGainRndPosAttachment;
    //SliderAttachment highFreqRndNegAttachment, highFreqRndPosAttachment;
    //SliderAttachment transAtkRndNegAttachment, transAtkRndPosAttachment;
    //SliderAttachment transDecRndNegAttachment, transDecRndPosAttachment;
    //SliderAttachment envAtkRndNegAttachment, envAtkRndPosAttachment;
    //SliderAttachment envDecRndNegAttachment, envDecRndPosAttachment;

    // AboutWindow � inline popup component
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
            g.setFont(juce::Font(13.f));
            juce::String body =
                "Load up to 20 one-shot samples. Each sample is\n"
                "mapped to all white keys and played back randomly\n"
                "based on selected playback algorithm type.\n\n"
                "Use the knobs to control pitch, volume, etc.\n"
                "Drag the blue/red arc dots to set negative and positive\n"
                "(respectively) per-note randomization ranges.";
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
    SampleManagerPanel sampleManagerPanel;


    // Overlay AFTER all sliders � accesses slider members via editor reference
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
    void addMoreSamples();
    void updateSamplesInfo();
    void savePreset();
    void loadPreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};
