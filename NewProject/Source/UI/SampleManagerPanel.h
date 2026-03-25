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
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    // Callbacks wired by the editor
    std::function<void()> onLoadSamplesClicked;
    std::function<void()> onAddMoreClicked;
    std::function<void(int slotIndex)> onReplaceSample;
    std::function<void(int slotIndex)> onAuditionSample;

private:
    NewProjectAudioProcessor& processor;

    juce::TextButton loadSamplesButton;
    juce::TextButton playbackModeButton;

    RRToggleLAF toggleLAF;

    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<ButtonAttachment> playbackModeAttachment;

    // Hit areas computed during paint
    juce::Rectangle<int> addMoreArea;

    struct RowHitAreas
    {
        int slotIndex = -1;
        juce::Rectangle<int> rowArea;     // full row for drop target
        juce::Rectangle<int> reorderBtn;
        juce::Rectangle<int> playBtn;
        juce::Rectangle<int> replaceBtn;
        juce::Rectangle<int> deleteBtn;
    };
    std::vector<RowHitAreas> rowHitAreas;

    // Drag state
    bool isDragging = false;
    int dragSourceSlot = -1;
    int dragTargetSlot = -1;
    bool dragIsInsert = false;  // true = insert between, false = swap onto
    juce::Point<int> dragPos;

    int getSlotAtPosition(juce::Point<int> pos, bool& isInsertGap) const;

    void deleteSample(int slotIndex);
    static juce::String truncateName(const juce::String& name, int maxChars);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleManagerPanel)
};
