#pragma once
#include <JuceHeader.h>
#include "RRLookAndFeel.h"

class NewProjectAudioProcessor;

class SampleManagerPanel : public juce::Component
{
public:
    explicit SampleManagerPanel(NewProjectAudioProcessor& processor);
    ~SampleManagerPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Called by the editor to wire up the load-samples file picker
    std::function<void()> onLoadSamplesClicked;

private:
    NewProjectAudioProcessor& processor;

    juce::TextButton loadSamplesButton;
    juce::TextButton playbackModeButton;

    RRToggleLAF toggleLAF;

    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<ButtonAttachment> playbackModeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleManagerPanel)
};
