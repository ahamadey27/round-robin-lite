#include "PluginProcessor.h"
#include "PluginEditor.h"

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
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
    transDecRndPosAttachment(p.apvts, ParameterIDs::transientDecayRndPos, transDecRndPosSlider),
    envAtkRndNegAttachment(p.apvts, ParameterIDs::envAttackRndNeg, envAtkRndNegSlider),
    envAtkRndPosAttachment(p.apvts, ParameterIDs::envAttackRndPos, envAtkRndPosSlider),
    envDecRndNegAttachment(p.apvts, ParameterIDs::envDecayRndNeg, envDecRndNegSlider),
    envDecRndPosAttachment(p.apvts, ParameterIDs::envDecayRndPos, envDecRndPosSlider),
    playbackModeAttachment(p.apvts, ParameterIDs::playbackMode, playbackModeButton)

{
    // Load buttons and Playback toggle Switch 
    for (int i = 0; i < 20; ++i)
    {
        loadButtons[i].setButtonText("Slot " + juce::String(i + 1) + ": Empty");
        loadButtons[i].onClick = [this, i]() { loadSampleForSlot(i); };

        playbackModeButton.setClickingTogglesState(true);
        playbackModeButton.onClick = [this]()
            {
                bool isRandom = playbackModeButton.getToggleState();
                playbackModeButton.setButtonText(isRandom ? "Random" : "Series");
            };

        contentComponent.addAndMakeVisible(playbackModeButton);
    }

    // Base sliders
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &lowGainSlider,  &lowFreqSlider,
                     &midGainSlider,  &midFreqSlider,
                     &highGainSlider, &highFreqSlider,
                     &transientAttackSlider, &transientDecaySlider,
                     &envAttackSlider, &envDecaySlider })
        setupSlider(*s);

    // Rnd sliders
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
                     &transDecRndNegSlider,  &transDecRndPosSlider,
                     &envAtkRndNegSlider,    &envAtkRndPosSlider,
                     &envDecRndNegSlider,    &envDecRndPosSlider
        })
        setupSlider(*s);

    viewport.setViewedComponent(&contentComponent, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);

    setSize(700, 750);
    resized(); // force layout so contentComponent size is set
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
                audioProcessor.rebuildLoadedIndices();
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
    viewport.setBounds(getLocalBounds());

    const int margin = 10;
    const int rowH = 24;
    const int gap = 34;
    const int labelW = 110;
    const int contentW = 665;

    //==========================================================================
    // Calculate total content height upfront
    // Slots: 5 rows * 35px = 175 + margin
    // Section header: 30px each
    // Base params: 14 rows * gap
    // Rnd params: 12 rows * gap
    const int slotsHeight = 5 * 35 + margin;
    const int baseHeight = 30 + 14 * gap;
    const int rndHeight = 30 + 12 * gap;
    const int totalH = margin + slotsHeight + baseHeight + rndHeight + 20;
    contentComponent.setSize(contentW, totalH);

    int y = margin;

    //==========================================================================
    // SAMPLE SLOTS — 4 columns x 5 rows / Sample Playback Toggle
    {
        const int cols = 4;
        const int btnW = (contentW - margin * 2 - (cols - 1) * 5) / cols;
        const int btnH = 28;
        const int btnGap = 5;

        for (int i = 0; i < 20; ++i)
        {
            int col = i % cols;
            int row = i / cols;
            loadButtons[i].setBounds(margin + col * (btnW + btnGap),
                y + row * (btnH + btnGap),
                btnW, btnH);
        }
        y += 5 * (28 + 5) + 10;

        // Playback mode toggle — sits just below the sample slots
        playbackModeButton.setBounds(margin, y, 120, 28);
        y += 38;

        y += 30; // section header gap  ← existing
    }

    //==========================================================================
    // BASE PARAMETERS
    const int sliderW = contentW - labelW - margin * 2;

    // Record y positions for labels BEFORE advancing y
    contentComponent.labelPositions.clear();

    y += 30; // section header gap

    // Helper lambda — records label position then places slider
    auto addRow = [&](juce::Slider& s, const juce::String& name)
        {
            contentComponent.labelPositions.push_back({ y, name });
            s.setBounds(margin + labelW, y, sliderW, rowH);
            y += gap;
        };

    addRow(semitoneSlider, "Semitone");
    addRow(fineTuneSlider, "Fine Tune");
    addRow(volumeSlider, "Volume");
    addRow(panSlider, "Pan");
    addRow(lowGainSlider, "EQ Low Gain");
    addRow(lowFreqSlider, "EQ Low Freq");
    addRow(midGainSlider, "EQ Mid Gain");
    addRow(midFreqSlider, "EQ Mid Freq");
    addRow(highGainSlider, "EQ High Gain");
    addRow(highFreqSlider, "EQ High Freq");
    addRow(transientAttackSlider, "Trans Attack");
    addRow(transientDecaySlider, "Trans Decay");
    addRow(envAttackSlider, "Env Attack");
    addRow(envDecaySlider, "Env Decay");

    //==========================================================================
    // RANDOMIZATION PARAMETERS — neg and pos side by side
    const int halfW = (sliderW - 5) / 2;

    y += 30; // section header gap

    auto addRndRow = [&](juce::Slider& neg, juce::Slider& pos, const juce::String& name)
        {
            contentComponent.labelPositions.push_back({ y, name });
            neg.setBounds(margin + labelW, y, halfW, rowH);
            pos.setBounds(margin + labelW + halfW + 5, y, halfW, rowH);
            y += gap;
        };

    addRndRow(semitoneRndNegSlider, semitoneRndPosSlider, "Semitone Rnd");
    addRndRow(fineTuneRndNegSlider, fineTuneRndPosSlider, "Fine Tune Rnd");
    addRndRow(volumeRndNegSlider, volumeRndPosSlider, "Volume Rnd");
    addRndRow(panRndNegSlider, panRndPosSlider, "Pan Rnd");
    addRndRow(lowGainRndNegSlider, lowGainRndPosSlider, "LowGain Rnd");
    addRndRow(lowFreqRndNegSlider, lowFreqRndPosSlider, "LowFreq Rnd");
    addRndRow(midGainRndNegSlider, midGainRndPosSlider, "MidGain Rnd");
    addRndRow(midFreqRndNegSlider, midFreqRndPosSlider, "MidFreq Rnd");
    addRndRow(highGainRndNegSlider, highGainRndPosSlider, "HiGain Rnd");
    addRndRow(highFreqRndNegSlider, highFreqRndPosSlider, "HiFreq Rnd");
    addRndRow(transAtkRndNegSlider, transAtkRndPosSlider, "Trans Atk Rnd");
    addRndRow(transDecRndNegSlider, transDecRndPosSlider, "Trans Dec Rnd");
    addRndRow(envAtkRndNegSlider, envAtkRndPosSlider, "Env Atk Rnd");
    addRndRow(envDecRndNegSlider, envDecRndPosSlider, "Env Dec Rnd");

    contentComponent.repaint();
}