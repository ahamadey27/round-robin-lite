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
    // Sample Load Button:
    loadSamplesButton.setButtonText("Load Samples");
    loadSamplesButton.onClick = [this]() { loadSamplesFromFiles(); };
    addAndMakeVisible(loadSamplesButton);

    // User Presets
    savePresetButton.setButtonText("Save Preset");
    savePresetButton.onClick = [this]() { savePreset(); };
    addAndMakeVisible(savePresetButton);

    loadPresetButton.setButtonText("Load Preset");
    loadPresetButton.onClick = [this]() { loadPreset(); };
    addAndMakeVisible(loadPresetButton);

    // Sample Info Label
    samplesInfoLabel.setText("No samples loaded", juce::dontSendNotification);
    samplesInfoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(samplesInfoLabel);

    // Playback button — OUTSIDE the loop
    playbackModeButton.setButtonText("Series");
    playbackModeButton.setClickingTogglesState(true);
    playbackModeButton.onClick = [this]()
        {
            bool isRandom = playbackModeButton.getToggleState();
            playbackModeButton.setButtonText(isRandom ? "Random" : "Series");
        };
    addAndMakeVisible(playbackModeButton);

    // Base sliders
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &lowGainSlider,  &lowFreqSlider,
                     &midGainSlider,  &midFreqSlider,
                     &highGainSlider, &highFreqSlider,
                     &transientAttackSlider, &transientDecaySlider,
                     &envAttackSlider, &envDecaySlider })
        setupKnob(*s);

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

    setSize(700, 650);
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

void NewProjectAudioProcessorEditor::setupKnob(juce::Slider& s)
{
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 16);
    addAndMakeVisible(s);   // directly on editor, NOT contentComponent
}

//==============================================================================
void NewProjectAudioProcessorEditor::loadSamplesFromFiles()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Load Samples (up to 20)",
        juce::File{},
        "*.wav;*.aif;*.aiff;*.flac;*.ogg"
    );

    fileChooser->launchAsync(
        juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::canSelectMultipleItems,
        [this](const juce::FileChooser& fc)
        {
            auto results = fc.getResults();

            // Clear existing pool before loading new selection
            for (int i = 0; i < NewProjectAudioProcessor::NUM_SAMPLE_SLOTS; ++i)
                audioProcessor.sampleLoader.clearSlot(i);

            int slot = 0;

            for (const auto& file : results)
            {
                if (slot >= NewProjectAudioProcessor::NUM_SAMPLE_SLOTS) break;
                if (file.existsAsFile())
                {
                    audioProcessor.sampleLoader.loadSample(slot, file);
                    ++slot;
                }
            }

            audioProcessor.rebuildLoadedIndices();
            updateSamplesInfo();
        }
    );
}

void NewProjectAudioProcessorEditor::updateSamplesInfo()
{
    int count = (int)audioProcessor.loadedSlotIndices.size();
    if (count == 0)
        samplesInfoLabel.setText("No samples loaded", juce::dontSendNotification);
    else
        samplesInfoLabel.setText(juce::String(count) + " sample(s) loaded",
            juce::dontSendNotification);
}

void NewProjectAudioProcessorEditor::savePreset()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Save Preset", juce::File{}, "*.rrpreset");

    fileChooser->launchAsync(
        juce::FileBrowserComponent::saveMode |
        juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                auto withExt = file.withFileExtension(".rrpreset");
                audioProcessor.savePreset(withExt);
            }
        });
}

void NewProjectAudioProcessorEditor::loadPreset()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Load Preset", juce::File{}, "*.rrpreset");

    fileChooser->launchAsync(
        juce::FileBrowserComponent::openMode |
        juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                audioProcessor.loadPreset(file);
                updateSamplesInfo();
            }
        });
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    //==========================================================================
    // Background
    g.fillAll(juce::Colour(28, 28, 33));

    //==========================================================================
    // Layout constants — must match resized() exactly
    constexpr int margin = 20;
    constexpr int knobW = 85;
    constexpr int knobH = 100;
    constexpr int knobGap = 10;
    constexpr int sectGap = 35;
    constexpr int row1Y = 100;
    constexpr int row2Y = 233;
    constexpr int row3Y = 366;
    constexpr int eqStep = (700 - 2 * margin - 6 * knobW) / 5;

    // Section x positions (matching resized)
    const int pitchX = margin;
    const int ampX = pitchX + 2 * (knobW + knobGap) + sectGap;
    const int envX = ampX + 2 * (knobW + knobGap) + sectGap;
    const int transX = margin;

    //==========================================================================
    // Helpers
    auto drawSectionLabel = [&](int x, int y, const juce::String& text, juce::Colour col)
        {
            g.setColour(col);
            g.setFont(juce::Font(10.5f).boldened());
            g.drawText(text, x, y, 160, 14, juce::Justification::left);
        };

    auto drawKnobLabel = [&](int x, int knobY, const juce::String& text)
        {
            g.setColour(juce::Colour(180, 180, 195));
            g.setFont(juce::Font(10.0f));
            g.drawText(text, x, knobY - 15, knobW, 13, juce::Justification::centred);
        };

    auto drawDivider = [&](int y)
        {
            g.setColour(juce::Colour(55, 55, 65));
            g.fillRect(margin, y, getWidth() - 2 * margin, 1);
        };

    //==========================================================================
    // Plugin header bar
    g.setColour(juce::Colour(40, 40, 48));
    g.fillRect(0, 0, getWidth(), 82);
    g.setColour(juce::Colour(100, 180, 255));
    g.setFont(juce::Font(13.0f).boldened());
    g.drawText("ROUND ROBIN LITE", getWidth() - 170, 10, 150, 20, juce::Justification::right);

    //==========================================================================
    // ROW 1 — PITCH
    drawSectionLabel(pitchX, row1Y - 16, "PITCH", juce::Colour(100, 180, 255));
    drawKnobLabel(pitchX, row1Y, "Semitone");
    drawKnobLabel(pitchX + knobW + knobGap, row1Y, "Fine Tune");

    // ROW 1 — AMPLITUDE
    drawSectionLabel(ampX, row1Y - 16, "AMPLITUDE", juce::Colour(100, 210, 140));
    drawKnobLabel(ampX, row1Y, "Volume");
    drawKnobLabel(ampX + knobW + knobGap, row1Y, "Pan");

    // ROW 1 — ENVELOPE
    drawSectionLabel(envX, row1Y - 16, "ENVELOPE", juce::Colour(210, 170, 90));
    drawKnobLabel(envX, row1Y, "Atk");
    drawKnobLabel(envX + knobW + knobGap, row1Y, "Dec");

    drawDivider(row1Y + knobH + 6);

    //==========================================================================
    // ROW 2 — TRANSIENT
    drawSectionLabel(transX, row2Y - 16, "TRANSIENT", juce::Colour(220, 110, 110));
    drawKnobLabel(transX, row2Y, "Attack");
    drawKnobLabel(transX + knobW + knobGap, row2Y, "Decay");

    drawDivider(row2Y + knobH + 6);

    //==========================================================================
    // ROW 3 — EQ
    drawSectionLabel(margin, row3Y - 16, "3-BAND EQ", juce::Colour(170, 120, 220));
    const char* eqLabels[] = { "Lo Gain", "Lo Freq", "Mid Gain", "Mid Freq", "Hi Gain", "Hi Freq" };
    for (int i = 0; i < 6; ++i)
        drawKnobLabel(margin + i * (knobW + eqStep), row3Y, eqLabels[i]);

    drawDivider(row3Y + knobH + 6);

    //==========================================================================
    // RANDOMIZATION header
    constexpr int rndHeaderY = 478;
    g.setColour(juce::Colour(140, 140, 155));
    g.setFont(juce::Font(10.5f).boldened());
    g.drawText("RANDOMIZATION  —  Neg / Pos per parameter",
        margin, rndHeaderY, 400, 14, juce::Justification::left);
}

//==============================================================================
void NewProjectAudioProcessorEditor::resized()
{
    //==========================================================================
    constexpr int margin = 20;
    constexpr int btnH = 28;
    constexpr int knobW = 85;
    constexpr int knobH = 100;
    constexpr int knobGap = 10;   // gap between knobs within a section
    constexpr int sectGap = 35;   // gap between sections

    //==========================================================================
    // BUTTON ROW 1: Load Samples + info label
    loadSamplesButton.setBounds(margin, 10, 140, btnH);
    samplesInfoLabel.setBounds(margin + 150, 10, 320, btnH);

    // BUTTON ROW 2: Save / Load preset / playback mode
    savePresetButton.setBounds(margin, 46, 110, btnH);
    loadPresetButton.setBounds(margin + 120, 46, 110, btnH);
    playbackModeButton.setBounds(margin + 250, 46, 130, btnH);

    //==========================================================================
    // KNOB ROW 1: PITCH | AMPLITUDE | ENVELOPE  (y = 100)
    constexpr int row1Y = 100;

    const int pitchX = margin;
    semitoneSlider.setBounds(pitchX, row1Y, knobW, knobH);
    fineTuneSlider.setBounds(pitchX + knobW + knobGap, row1Y, knobW, knobH);

    const int ampX = pitchX + 2 * (knobW + knobGap) + sectGap;  // 225
    volumeSlider.setBounds(ampX, row1Y, knobW, knobH);
    panSlider.setBounds(ampX + knobW + knobGap, row1Y, knobW, knobH);

    const int envX = ampX + 2 * (knobW + knobGap) + sectGap;    // 430
    envAttackSlider.setBounds(envX, row1Y, knobW, knobH);
    envDecaySlider.setBounds(envX + knobW + knobGap, row1Y, knobW, knobH);

    //==========================================================================
    // KNOB ROW 2: TRANSIENT  (y = 233)
    constexpr int row2Y = 233;

    const int transX = margin;
    transientAttackSlider.setBounds(transX, row2Y, knobW, knobH);
    transientDecaySlider.setBounds(transX + knobW + knobGap, row2Y, knobW, knobH);

    //==========================================================================
    // KNOB ROW 3: EQ  (y = 366)
    constexpr int row3Y = 366;
    constexpr int eqStep = (700 - 2 * margin - 6 * knobW) / 5;  // = 30px spacing

    lowGainSlider.setBounds(margin + 0 * (knobW + eqStep), row3Y, knobW, knobH);
    lowFreqSlider.setBounds(margin + 1 * (knobW + eqStep), row3Y, knobW, knobH);
    midGainSlider.setBounds(margin + 2 * (knobW + eqStep), row3Y, knobW, knobH);
    midFreqSlider.setBounds(margin + 3 * (knobW + eqStep), row3Y, knobW, knobH);
    highGainSlider.setBounds(margin + 4 * (knobW + eqStep), row3Y, knobW, knobH);
    highFreqSlider.setBounds(margin + 5 * (knobW + eqStep), row3Y, knobW, knobH);

    //==========================================================================
    // RANDOMIZATION — scrollable contentComponent inside viewport
    constexpr int rndViewportY = 498;
    const     int contentW = getWidth() - 2 * margin;
    constexpr int labelW = 120;
    constexpr int rowH = 20;
    constexpr int rowGap = 26;
    const     int halfW = (contentW - labelW - 5) / 2;

    contentComponent.labelPositions.clear();
    int cy = 4;

    auto addRndRow = [&](juce::Slider& neg, juce::Slider& pos, const juce::String& name)
        {
            contentComponent.labelPositions.push_back({ cy, name });
            neg.setBounds(labelW, cy, halfW, rowH);
            pos.setBounds(labelW + halfW + 5, cy, halfW, rowH);
            cy += rowGap;
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

    contentComponent.setSize(contentW, cy + 4);
    viewport.setBounds(margin, rndViewportY, contentW, getHeight() - rndViewportY - 10);

    contentComponent.repaint();
}