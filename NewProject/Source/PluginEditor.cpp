#include "PluginProcessor.h"
#include "PluginEditor.h"

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    // Base parameters
    semitoneAttachment(p.apvts, ParameterIDs::semitone, semitoneSlider),
    fineTuneAttachment(p.apvts, ParameterIDs::fineTune, fineTuneSlider),
    volumeAttachment(p.apvts, ParameterIDs::volume, volumeSlider),
    panAttachment(p.apvts, ParameterIDs::pan, panSlider),
    lowGainAttachment(p.apvts, ParameterIDs::lowGain, lowGainSlider),
    lowFreqAttachment(p.apvts, ParameterIDs::lowFreq, lowFreqSlider),
    midGainAttachment(p.apvts, ParameterIDs::midGain, midGainSlider),
    midFreqAttachment(p.apvts, ParameterIDs::midFreq, midFreqSlider),
    highGainAttachment(p.apvts, ParameterIDs::highGain, highGainSlider),
    highFreqAttachment(p.apvts, ParameterIDs::highFreq, highFreqSlider),
    transientAttackAttachment(p.apvts, ParameterIDs::transientAttack, transientAttackSlider),
    transientDecayAttachment(p.apvts, ParameterIDs::transientDecay, transientDecaySlider),
    envAttackAttachment(p.apvts, ParameterIDs::envAttack, envAttackSlider),
    envDecayAttachment(p.apvts, ParameterIDs::envDecay, envDecaySlider),
    // Randomization parameters
    semitoneRndNegAttachment(p.apvts, ParameterIDs::semitoneRndNeg, semitoneRndNegSlider),
    semitoneRndPosAttachment(p.apvts, ParameterIDs::semitoneRndPos, semitoneRndPosSlider),
    fineTuneRndNegAttachment(p.apvts, ParameterIDs::fineTuneRndNeg, fineTuneRndNegSlider),
    fineTuneRndPosAttachment(p.apvts, ParameterIDs::fineTuneRndPos, fineTuneRndPosSlider),
    volumeRndNegAttachment(p.apvts, ParameterIDs::volumeRndNeg, volumeRndNegSlider),
    volumeRndPosAttachment(p.apvts, ParameterIDs::volumeRndPos, volumeRndPosSlider),
    panRndNegAttachment(p.apvts, ParameterIDs::panRndNeg, panRndNegSlider),
    panRndPosAttachment(p.apvts, ParameterIDs::panRndPos, panRndPosSlider),
    lowGainRndNegAttachment(p.apvts, ParameterIDs::lowGainRndNeg, lowGainRndNegSlider),
    lowGainRndPosAttachment(p.apvts, ParameterIDs::lowGainRndPos, lowGainRndPosSlider),
    lowFreqRndNegAttachment(p.apvts, ParameterIDs::lowFreqRndNeg, lowFreqRndNegSlider),
    lowFreqRndPosAttachment(p.apvts, ParameterIDs::lowFreqRndPos, lowFreqRndPosSlider),
    midGainRndNegAttachment(p.apvts, ParameterIDs::midGainRndNeg, midGainRndNegSlider),
    midGainRndPosAttachment(p.apvts, ParameterIDs::midGainRndPos, midGainRndPosSlider),
    midFreqRndNegAttachment(p.apvts, ParameterIDs::midFreqRndNeg, midFreqRndNegSlider),
    midFreqRndPosAttachment(p.apvts, ParameterIDs::midFreqRndPos, midFreqRndPosSlider),
    highGainRndNegAttachment(p.apvts, ParameterIDs::highGainRndNeg, highGainRndNegSlider),
    highGainRndPosAttachment(p.apvts, ParameterIDs::highGainRndPos, highGainRndPosSlider),
    highFreqRndNegAttachment(p.apvts, ParameterIDs::highFreqRndNeg, highFreqRndNegSlider),
    highFreqRndPosAttachment(p.apvts, ParameterIDs::highFreqRndPos, highFreqRndPosSlider),
    transAtkRndNegAttachment(p.apvts, ParameterIDs::transientAttackRndNeg, transAtkRndNegSlider),
    transAtkRndPosAttachment(p.apvts, ParameterIDs::transientAttackRndPos, transAtkRndPosSlider),
    transDecRndNegAttachment(p.apvts, ParameterIDs::transientDecayRndNeg, transDecRndNegSlider),
    transDecRndPosAttachment(p.apvts, ParameterIDs::transientDecayRndPos, transDecRndPosSlider)
{
    //==========================================================================
    // Setup load buttons and add to contentComponent
    for (int i = 0; i < 20; ++i)
    {
        loadButtons[i].setButtonText("Slot " + juce::String(i + 1) + ": Empty");
        loadButtons[i].onClick = [this, i]() { loadSampleForSlot(i); };
        contentComponent.addAndMakeVisible(loadButtons[i]);
    }

    //==========================================================================
    // Setup base sliders and add to contentComponent
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &lowGainSlider,  &lowFreqSlider,
                     &midGainSlider,  &midFreqSlider,
                     &highGainSlider, &highFreqSlider,
                     &transientAttackSlider, &transientDecaySlider,
                     &envAttackSlider, &envDecaySlider })
    {
        setupSlider(*s);
    }

    //==========================================================================
    // Setup randomization sliders and add to contentComponent
    for (auto* s : { &semitoneRndNegSlider,  &semitoneRndPosSlider,
                     &fineTuneRndNegSlider,  &fineTuneRndPosSlider,
                     &volumeRndNegSlider,    &volumeRndPosSlider,
                     &panRndNegSlider,       &panRndPosSlider,
                     &lowGainRndNegSlider,   &lowGainRndPosSlider,
                     &lowFreqRndNegSlider,   &lowFreqRndPosSlider,
                     &midGainRndNegSlider,   &midGainRndPosSlider,
                     &midFreqRndNegSlider,   &midFreqRndPosSlider,
                     &highGainRndNegSlider,  &highGainRndPosSlider,
                     &highFreqRndNegSlider,  &highFreqRndPosSlider,
                     &transAtkRndNegSlider,  &transAtkRndPosSlider,
                     &transDecRndNegSlider,  &transDecRndPosSlider })
    {
        setupSlider(*s);
    }

    //==========================================================================
    // Setup viewport
    viewport.setViewedComponent(&contentComponent, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);

    // Content width (window - scrollbar), content height calculated below:
    // Slots: 30 header + 5 rows*35 + 20 padding = 225
    // Base params: 25 header + 14 rows*35 + 20 padding = 535
    // Rnd params: 25 header + 12 rows*35 + 10 padding = 455
    // Total ~ 1215
    contentComponent.setSize(680, 1215);

    setSize(700, 750);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor() {}

//==============================================================================
void NewProjectAudioProcessorEditor::setupSlider(juce::Slider& s)
{
    s.setSliderStyle(juce::Slider::LinearHorizontal);
    s.setTextBoxStyle(juce::Slider::TextBoxRight, false, 55, 20);
    contentComponent.addAndMakeVisible(s);
}

//==============================================================================
void NewProjectAudioProcessorEditor::loadSampleForSlot(int slotIndex)
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Load Sample for Slot " + juce::String(slotIndex + 1),
        juce::File{},
        "*.wav;*.aif;*.aiff;*.flac;*.ogg"
    );

    fileChooser->launchAsync(
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this, slotIndex](const juce::FileChooser& fc)
        {
            auto result = fc.getResult();
            if (result.existsAsFile())
            {
                audioProcessor.sampleLoader.loadSample(slotIndex, result);
                audioProcessor.rebuildLoadedIndices();  // <-- ADD THIS
                updateSlotLabels();
            }
        }
    );
}

void NewProjectAudioProcessorEditor::updateSlotLabels()
{
    for (int i = 0; i < 20; ++i)
    {
        if (audioProcessor.sampleSlots[i].isLoaded)
            loadButtons[i].setButtonText(audioProcessor.sampleSlots[i].displayName);
        else
            loadButtons[i].setButtonText("Slot " + juce::String(i + 1) + ": Empty");
    }
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

//==============================================================================
void NewProjectAudioProcessorEditor::resized()
{
    // Viewport fills the entire editor
    viewport.setBounds(getLocalBounds());
    contentComponent.setSize(680, 1215);

    const int margin = 10;
    const int rowH = 28;
    const int gap = 35;
    const int labelW = 110;
    const int contentW = 660;

    int y = margin;

    //==========================================================================
    // SAMPLE SLOTS SECTION
    // Draw 4 columns x 5 rows of load buttons
    {
        const int cols = 4;
        const int btnW = (contentW - margin * (cols - 1)) / cols;
        const int btnH = 30;
        const int btnGap = 5;

        for (int i = 0; i < 20; ++i)
        {
            int col = i % cols;
            int row = i / cols;
            loadButtons[i].setBounds(margin + col * (btnW + btnGap),
                y + row * (btnH + btnGap),
                btnW, btnH);
        }
        y += 5 * (30 + 5) + 20; // 5 rows + padding
    }

    //==========================================================================
    // BASE PARAMETERS SECTION
    // Layout: label(110) | slider(fills rest)
    const int sliderW = contentW - labelW - margin;

    auto addRow = [&](juce::Slider& s)
        {
            s.setBounds(margin + labelW, y, sliderW, rowH);
            y += gap;
        };

    y += 25; // section header space
    addRow(semitoneSlider);
    addRow(fineTuneSlider);
    addRow(volumeSlider);
    addRow(panSlider);
    addRow(lowGainSlider);
    addRow(lowFreqSlider);
    addRow(midGainSlider);
    addRow(midFreqSlider);
    addRow(highGainSlider);
    addRow(highFreqSlider);
    addRow(transientAttackSlider);
    addRow(transientDecaySlider);
    addRow(envAttackSlider);
    addRow(envDecaySlider);

    //==========================================================================
    // RANDOMIZATION SECTION
    // Layout: label(110) | neg slider(half) | pos slider(half)
    const int halfW = (sliderW - 5) / 2;

    y += 25; // section header space

    auto addRndRow = [&](juce::Slider& neg, juce::Slider& pos)
        {
            neg.setBounds(margin + labelW, y, halfW, rowH);
            pos.setBounds(margin + labelW + halfW + 5, y, halfW, rowH);
            y += gap;
        };

    addRndRow(semitoneRndNegSlider, semitoneRndPosSlider);
    addRndRow(fineTuneRndNegSlider, fineTuneRndPosSlider);
    addRndRow(volumeRndNegSlider, volumeRndPosSlider);
    addRndRow(panRndNegSlider, panRndPosSlider);
    addRndRow(lowGainRndNegSlider, lowGainRndPosSlider);
    addRndRow(lowFreqRndNegSlider, lowFreqRndPosSlider);
    addRndRow(midGainRndNegSlider, midGainRndPosSlider);
    addRndRow(midFreqRndNegSlider, midFreqRndPosSlider);
    addRndRow(highGainRndNegSlider, highGainRndPosSlider);
    addRndRow(highFreqRndNegSlider, highFreqRndPosSlider);
    addRndRow(transAtkRndNegSlider, transAtkRndPosSlider);
    addRndRow(transDecRndNegSlider, transDecRndPosSlider);
}