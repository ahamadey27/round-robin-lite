#include "PluginProcessor.h"
#include "PluginEditor.h"

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    semitoneAttachment(p.apvts, ParameterIDs::semitone, semitoneSlider),
    fineTuneAttachment(p.apvts, ParameterIDs::fineTune, fineTuneSlider),
    volumeAttachment(p.apvts, ParameterIDs::volume, volumeSlider),
    panAttachment(p.apvts, ParameterIDs::pan, panSlider),
    toneLowAttachment(p.apvts, ParameterIDs::toneLow, toneLowSlider),
    toneHighAttachment(p.apvts, ParameterIDs::toneHigh, toneHighSlider),
    sampleStartAttachment(p.apvts, ParameterIDs::sampleStart, sampleStartSlider),
    sampleEndAttachment(p.apvts, ParameterIDs::sampleEnd, sampleEndSlider),
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //lowGainAttachment(p.apvts, ParameterIDs::lowGain, lowGainSlider),
    //lowFreqAttachment(p.apvts, ParameterIDs::lowFreq, lowFreqSlider),
    //midGainAttachment(p.apvts, ParameterIDs::midGain, midGainSlider),
    //midFreqAttachment(p.apvts, ParameterIDs::midFreq, midFreqSlider),
    //highGainAttachment(p.apvts, ParameterIDs::highGain, highGainSlider),
    //highFreqAttachment(p.apvts, ParameterIDs::highFreq, highFreqSlider),
    //transientAttackAttachment(p.apvts, ParameterIDs::transientAttack, transientAttackSlider),
    //transientDecayAttachment(p.apvts, ParameterIDs::transientDecay, transientDecaySlider),
    //envAttackAttachment(p.apvts, ParameterIDs::envAttack, envAttackSlider),
    //envDecayAttachment(p.apvts, ParameterIDs::envDecay, envDecaySlider),
    semitoneRndNegAttachment(p.apvts, ParameterIDs::semitoneRndNeg, semitoneRndNegSlider),
    semitoneRndPosAttachment(p.apvts, ParameterIDs::semitoneRndPos, semitoneRndPosSlider),
    fineTuneRndNegAttachment(p.apvts, ParameterIDs::fineTuneRndNeg, fineTuneRndNegSlider),
    fineTuneRndPosAttachment(p.apvts, ParameterIDs::fineTuneRndPos, fineTuneRndPosSlider),
    volumeRndNegAttachment(p.apvts, ParameterIDs::volumeRndNeg, volumeRndNegSlider),
    volumeRndPosAttachment(p.apvts, ParameterIDs::volumeRndPos, volumeRndPosSlider),
    panRndNegAttachment(p.apvts, ParameterIDs::panRndNeg, panRndNegSlider),
    panRndPosAttachment(p.apvts, ParameterIDs::panRndPos, panRndPosSlider),
    toneLowRndNegAttachment(p.apvts, ParameterIDs::toneLowRndNeg, toneLowRndNegSlider),
    toneLowRndPosAttachment(p.apvts, ParameterIDs::toneLowRndPos, toneLowRndPosSlider),
    toneHighRndNegAttachment(p.apvts, ParameterIDs::toneHighRndNeg, toneHighRndNegSlider),
    toneHighRndPosAttachment(p.apvts, ParameterIDs::toneHighRndPos, toneHighRndPosSlider),
    sampleStartRndNegAttachment(p.apvts, ParameterIDs::sampleStartRndNeg, sampleStartRndNegSlider),
    sampleStartRndPosAttachment(p.apvts, ParameterIDs::sampleStartRndPos, sampleStartRndPosSlider),
    sampleEndRndNegAttachment(p.apvts, ParameterIDs::sampleEndRndNeg, sampleEndRndNegSlider),
    sampleEndRndPosAttachment(p.apvts, ParameterIDs::sampleEndRndPos, sampleEndRndPosSlider),
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    //lowGainRndNegAttachment(p.apvts, ParameterIDs::lowGainRndNeg, lowGainRndNegSlider),
    //lowGainRndPosAttachment(p.apvts, ParameterIDs::lowGainRndPos, lowGainRndPosSlider),
    //lowFreqRndNegAttachment(p.apvts, ParameterIDs::lowFreqRndNeg, lowFreqRndNegSlider),
    //lowFreqRndPosAttachment(p.apvts, ParameterIDs::lowFreqRndPos, lowFreqRndPosSlider),
    //midGainRndNegAttachment(p.apvts, ParameterIDs::midGainRndNeg, midGainRndNegSlider),
    //midGainRndPosAttachment(p.apvts, ParameterIDs::midGainRndPos, midGainRndPosSlider),
    //midFreqRndNegAttachment(p.apvts, ParameterIDs::midFreqRndNeg, midFreqRndNegSlider),
    //midFreqRndPosAttachment(p.apvts, ParameterIDs::midFreqRndPos, midFreqRndPosSlider),
    //highGainRndNegAttachment(p.apvts, ParameterIDs::highGainRndNeg, highGainRndNegSlider),
    //highGainRndPosAttachment(p.apvts, ParameterIDs::highGainRndPos, highGainRndPosSlider),
    //highFreqRndNegAttachment(p.apvts, ParameterIDs::highFreqRndNeg, highFreqRndNegSlider),
    //highFreqRndPosAttachment(p.apvts, ParameterIDs::highFreqRndPos, highFreqRndPosSlider),
    //transAtkRndNegAttachment(p.apvts, ParameterIDs::transientAttackRndNeg, transAtkRndNegSlider),
    //transAtkRndPosAttachment(p.apvts, ParameterIDs::transientAttackRndPos, transAtkRndPosSlider),
    //transDecRndNegAttachment(p.apvts, ParameterIDs::transientDecayRndNeg, transDecRndNegSlider),
    //transDecRndPosAttachment(p.apvts, ParameterIDs::transientDecayRndPos, transDecRndPosSlider),
    //envAtkRndNegAttachment(p.apvts, ParameterIDs::envAttackRndNeg, envAtkRndNegSlider),
    //envAtkRndPosAttachment(p.apvts, ParameterIDs::envAttackRndPos, envAtkRndPosSlider),
    //envDecRndNegAttachment(p.apvts, ParameterIDs::envDecayRndNeg, envDecRndNegSlider),
    //envDecRndPosAttachment(p.apvts, ParameterIDs::envDecayRndPos, envDecRndPosSlider),
    sampleManagerPanel(p)
{
    // Wire sample manager panel callbacks
    sampleManagerPanel.onLoadSamplesClicked = [this]() { loadSamplesFromFiles(); };
    sampleManagerPanel.onAddMoreClicked = [this]() { addMoreSamples(); };
    sampleManagerPanel.onReplaceSample = [this](int slotIndex)
        {
            fileChooser = std::make_unique<juce::FileChooser>(
                "Replace Sample", juce::File{}, "*.wav;*.aif;*.aiff;*.flac;*.ogg");
            fileChooser->launchAsync(
                juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                [this, slotIndex](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    if (file.existsAsFile())
                    {
                        audioProcessor.sampleLoader.loadSample(slotIndex, file);
                        audioProcessor.rebuildLoadedIndices();
                        sampleManagerPanel.repaint();
                    }
                });
        };
    sampleManagerPanel.onAuditionSample = [this](int slotIndex)
        {
            audioProcessor.auditionSample(slotIndex);
        };

    // Trigger button
    triggerButton.setButtonText("Trigger");
    triggerButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffa03030));
    triggerButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    triggerButton.onClick = [this]() { audioProcessor.requestTrigger(); };
    addAndMakeVisible(triggerButton);

    // About button
    aboutButton.setButtonText("?");
    aboutButton.onClick = [this]
        {
            aboutWindow.setTopLeftPosition(getWidth() / 2 - 170, getHeight() / 2 - 100); 
            addAndMakeVisible(aboutWindow);
            aboutWindow.toFront(true);
            repaint();   // ← ADD: hides dots immediately when window opens
        };
    
    addAndMakeVisible(aboutButton);
    aboutWindow.addComponentListener(this);   // ← ADD: watch for visibility changes

    // User Presets
    savePresetButton.setButtonText("Save Preset");
    savePresetButton.onClick = [this]() { savePreset(); };
    addAndMakeVisible(savePresetButton);

    loadPresetButton.setButtonText("Load Preset");
    loadPresetButton.onClick = [this]() { loadPreset(); };
    addAndMakeVisible(loadPresetButton);

    // REMOVED FOR LITE: samplesInfoLabel
    //samplesInfoLabel.setText("No samples loaded", juce::dontSendNotification);
    //samplesInfoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    //addAndMakeVisible(samplesInfoLabel);

    addAndMakeVisible(sampleManagerPanel);

    // Label setup:
    //playbackTypeLabel.setText("Playback Type", juce::dontSendNotification);
    //playbackTypeLabel.setFont(juce::Font(juce::FontOptions(10.0f)));
    //playbackTypeLabel.setColour(juce::Label::textColourId, juce::Colour(180, 180, 195));
    //playbackTypeLabel.setJustificationType(juce::Justification::centred);
    //addAndMakeVisible(playbackTypeLabel);

    // Main parameter knobs — visible, on the editor
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &toneLowSlider, &toneHighSlider,
                     &sampleStartSlider, &sampleEndSlider
                     // COMMENTED FOR LITE — ACTIVE IN PREMIUM
                     //,&lowGainSlider,  &lowFreqSlider,
                     //,&midGainSlider,  &midFreqSlider,
                     //,&highGainSlider, &highFreqSlider,
                     //,&transientAttackSlider, &transientDecaySlider,
                     //,&envAttackSlider, &envDecaySlider
                     })
        setupKnob(*s);

    // Rnd sliders — hidden, APVTS-bound, values read by paintOverChildren
    for (auto* s : { &semitoneRndNegSlider,  &semitoneRndPosSlider,
                     &fineTuneRndNegSlider,  &fineTuneRndPosSlider,
                     &volumeRndNegSlider,    &volumeRndPosSlider,
                     &panRndNegSlider,       &panRndPosSlider,
                     &toneLowRndNegSlider,   &toneLowRndPosSlider,
                     &toneHighRndNegSlider,  &toneHighRndPosSlider,
                     &sampleStartRndNegSlider, &sampleStartRndPosSlider,
                     &sampleEndRndNegSlider,   &sampleEndRndPosSlider
                     // COMMENTED FOR LITE — ACTIVE IN PREMIUM
                     //,&lowGainRndNegSlider,   &lowGainRndPosSlider,
                     //,&lowFreqRndNegSlider,   &lowFreqRndPosSlider,
                     //,&midGainRndNegSlider,   &midGainRndPosSlider,
                     //,&midFreqRndNegSlider,   &midFreqRndPosSlider,
                     //,&highGainRndNegSlider,  &highGainRndPosSlider,
                     //,&highFreqRndNegSlider,  &highFreqRndPosSlider,
                     //,&transAtkRndNegSlider,  &transAtkRndPosSlider,
                     //,&transDecRndNegSlider,  &transDecRndPosSlider,
                     //,&envAtkRndNegSlider,    &envAtkRndPosSlider,
                     //,&envDecRndNegSlider,    &envDecRndPosSlider
                     })
        setupSlider(*s);

    // Apply knob LAF to main parameter sliders
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &toneLowSlider, &toneHighSlider,
                     &sampleStartSlider, &sampleEndSlider
                     // COMMENTED FOR LITE — ACTIVE IN PREMIUM
                     //,&lowGainSlider, &lowFreqSlider, &midGainSlider, &midFreqSlider,
                     //,&highGainSlider, &highFreqSlider,
                     //,&transientAttackSlider, &transientDecaySlider,
                     //,&envAttackSlider, &envDecaySlider
                     })
        s->setLookAndFeel(&knobLAF);
    
    // NEW: set indicator line color per section (rotarySliderFillColourId)
        for (auto* s : { &semitoneSlider, &fineTuneSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::pitchCol);

        for (auto* s : { &volumeSlider, &panSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::ampCol);

        for (auto* s : { &toneLowSlider, &toneHighSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::toneCol);

        for (auto* s : { &sampleStartSlider, &sampleEndSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::trimCol);

        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        //for (auto* s : { &envAttackSlider, &envDecaySlider })
        //    s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::envCol);

        //for (auto* s : { &transientAttackSlider, &transientDecaySlider })
        //    s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::transCol);

        //for (auto* s : { &lowGainSlider, &lowFreqSlider })
        //    s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::eqLowCol);

        //for (auto* s : { &midGainSlider, &midFreqSlider })
        //    s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::eqMidCol);

        //for (auto* s : { &highGainSlider, &highFreqSlider })
        //    s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::eqHighCol);
    
    // NEW: value box text + border color matches section label color
        auto setValueBoxColors = [](juce::Slider& s, juce::Colour col)
        {
            s.setColour(juce::Slider::textBoxTextColourId,       col);
            s.setColour(juce::Slider::textBoxOutlineColourId,    col.withAlpha(0.55f));
            s.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff111111));
        };

        setValueBoxColors(semitoneSlider,        RRColors::pitchCol);
        setValueBoxColors(fineTuneSlider,        RRColors::pitchCol);
        setValueBoxColors(volumeSlider,          RRColors::ampCol);
        setValueBoxColors(panSlider,             RRColors::ampCol);
        setValueBoxColors(toneLowSlider,         RRColors::toneCol);
        setValueBoxColors(toneHighSlider,        RRColors::toneCol);
        setValueBoxColors(sampleStartSlider,     RRColors::trimCol);
        setValueBoxColors(sampleEndSlider,       RRColors::trimCol);
        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        //setValueBoxColors(envAttackSlider,       RRColors::envCol);
        //setValueBoxColors(envDecaySlider,        RRColors::envCol);
        //setValueBoxColors(transientAttackSlider, RRColors::transCol);
        //setValueBoxColors(transientDecaySlider,  RRColors::transCol);
        //setValueBoxColors(lowGainSlider,         RRColors::eqLowCol);
        //setValueBoxColors(lowFreqSlider,         RRColors::eqLowCol);
        //setValueBoxColors(midGainSlider,         RRColors::eqMidCol);
        //setValueBoxColors(midFreqSlider,         RRColors::eqMidCol);
        //setValueBoxColors(highGainSlider,        RRColors::eqHighCol);
        //setValueBoxColors(highFreqSlider,        RRColors::eqHighCol);


    // Dual-thumb randomization sliders
    for (auto* bar : { &semitoneRndBar, &fineTuneRndBar, &volumeRndBar, &panRndBar,
                       &toneLowRndBar, &toneHighRndBar, &sampleStartRndBar, &sampleEndRndBar })
        addAndMakeVisible(*bar);

    setSize(700, 714);
    resized();
}

//==============================================================================
NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &toneLowSlider, &toneHighSlider,
                     &sampleStartSlider, &sampleEndSlider })
        s->setLookAndFeel(nullptr);
}

//==============================================================================
void NewProjectAudioProcessorEditor::setupSlider(juce::Slider& s)
{
    // Hidden — APVTS-bound but not rendered directly.
    // Values are read by paintOverChildren() to draw rnd arcs over knobs.
    s.setSliderStyle(juce::Slider::LinearHorizontal);
    s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addChildComponent(s);   // addChildComponent = part of tree but invisible
}

void NewProjectAudioProcessorEditor::setupKnob(juce::Slider& s)
{
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 16);
    addAndMakeVisible(s);
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
            sampleManagerPanel.repaint();
        }
    );
}

void NewProjectAudioProcessorEditor::addMoreSamples()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Add Samples",
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

            // Find first empty slot
            int slot = 0;
            for (; slot < NewProjectAudioProcessor::NUM_SAMPLE_SLOTS; ++slot)
                if (!audioProcessor.sampleSlots[slot].isLoaded)
                    break;

            for (const auto& file : results)
            {
                if (slot >= NewProjectAudioProcessor::NUM_SAMPLE_SLOTS) break;
                if (file.existsAsFile())
                {
                    audioProcessor.sampleLoader.loadSample(slot, file);
                    ++slot;
                    // Skip to next empty slot
                    while (slot < NewProjectAudioProcessor::NUM_SAMPLE_SLOTS &&
                           audioProcessor.sampleSlots[slot].isLoaded)
                        ++slot;
                }
            }

            audioProcessor.rebuildLoadedIndices();
            sampleManagerPanel.repaint();
        }
    );
}

void NewProjectAudioProcessorEditor::updateSamplesInfo()
{
    // REMOVED FOR LITE: samplesInfoLabel removed — no-op
    //int count = (int)audioProcessor.loadedSlotIndices.size();
    //if (count == 0)
    //    samplesInfoLabel.setText("No Samples Loaded", juce::dontSendNotification);
    //else
    //    samplesInfoLabel.setText(juce::String(count) + " sample(s) loaded",
    //        juce::dontSendNotification);
}

void NewProjectAudioProcessorEditor::savePreset()
{
    DBG("=== SAVE PRESET BUTTON CLICKED ===");
    DBG("  loadedSlotIndices.size() = " + juce::String((int)audioProcessor.loadedSlotIndices.size()));

    // Warn the user early if no samples are loaded — the preset would be empty
    if (audioProcessor.loadedSlotIndices.empty())
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "No Samples Loaded",
            "Please load at least one sample before saving a preset.");
        return;
    }

    // useOSNativeDialogBox = false: JUCE 8 on macOS has a bug where the native
    // file chooser always creates NSOpenPanel even when saveMode is set, causing
    // "Ignoring NSSavePanel method sent to NSOpenPanel" and empty getResult().
    // JUCE's own cross-platform dialog correctly handles save mode.
    fileChooser = std::make_unique<juce::FileChooser>(
        "Save Preset",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.rrpreset",
        false);   // false = use JUCE dialog, not native macOS panel

    fileChooser->launchAsync(
        juce::FileBrowserComponent::saveMode |
        juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::warnAboutOverwriting,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            DBG("Save dialog result: '" + file.getFullPathName() + "'");
            if (file.getFullPathName().isNotEmpty())
                audioProcessor.savePreset(file.withFileExtension(".rrpreset"));
            else
                DBG("Save dialog: cancelled or returned empty path");
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
    // ── Layout constants (must match resized() exactly) ───────────────────────
    constexpr int margin  = 12;
    constexpr int knobW   = 68;
    constexpr int knobH   = 80;
    constexpr int tbH     = 16;
    constexpr int knobGap = 24;
    constexpr int rpX     = 356;
    constexpr int rpW     = 332;
    constexpr int ampY    = 60;
    constexpr int ampH    = 140;
    constexpr int toneY   = ampY + ampH + 8;
    constexpr int toneH   = 140;
    constexpr int pitchY  = toneY + toneH + 8;
    constexpr int pitchH  = 140;
    constexpr int trimY   = pitchY + pitchH + 8;
    constexpr int trimH   = 140;
    constexpr int kxOff0  = (rpW - 2 * knobW - knobGap) / 2;  // 86
    constexpr int kxOff1  = kxOff0 + knobW + knobGap;          // 178

    // knob label positions computed in drawKnobLabel lambda

    // ── Background ────────────────────────────────────────────────────────────
    g.fillAll(RRColors::background);

    // ── Header bar ────────────────────────────────────────────────────────────
    g.setColour(RRColors::headerBg);
    g.fillRect(0, 0, getWidth(), 52);
    g.setColour(RRColors::sectionBorder);
    g.fillRect(0, 52, getWidth(), 1);

    juce::Font rrFont(juce::FontOptions(26.0f));
    rrFont = rrFont.boldened();
    g.setFont(rrFont);
    g.setColour(juce::Colour(0xffd0d0d0));
    g.drawText("RoundRobin", 14, 10, 200, 32, juce::Justification::left);

    const int liteX = 14 + rrFont.getStringWidth("RoundRobin") + 3;
    g.setColour(juce::Colour(0xff999999));
    g.setFont(juce::Font(juce::FontOptions(17.0f)));
    g.drawText("Lite", liteX, 14, 60, 26, juce::Justification::left);

    // ── Section box helper ────────────────────────────────────────────────────
    auto drawSectionBox = [&](juce::Rectangle<int> r)
    {
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        g.fillRoundedRectangle(r.toFloat(), 5.0f);
        g.setColour(RRColors::sectionBorder);
        g.drawRoundedRectangle(r.toFloat(), 5.0f, 1.0f);
    };

    // ── Left panel: Sample Manager placeholder (Phase 3) ─────────────────────
    const int lpW = rpX - margin - 8;  // 336
    const int lpH = pitchY + pitchH - ampY;  // total height of all right sections
    drawSectionBox({ margin, ampY, lpW, lpH });

    // ── Right panel: Amplitude section ───────────────────────────────────────
    drawSectionBox({ rpX, ampY, rpW, ampH });
    g.setColour(RRColors::ampCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("AMPLITUDE", rpX + 8, ampY + 7, rpW - 16, 10, juce::Justification::left);

    // ── Right panel: Tone section ────────────────────────────────────────────
    drawSectionBox({ rpX, toneY, rpW, toneH });
    g.setColour(RRColors::toneCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("TONE", rpX + 8, toneY + 7, rpW - 16, 10, juce::Justification::left);

    // ── Right panel: Pitch section ────────────────────────────────────────────
    drawSectionBox({ rpX, pitchY, rpW, pitchH });
    g.setColour(RRColors::pitchCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("PITCH", rpX + 8, pitchY + 7, rpW - 16, 10, juce::Justification::left);

    // ── Right panel: Sample Start/End section ────────────────────────────────
    drawSectionBox({ rpX, trimY, rpW, trimH });
    g.setColour(RRColors::trimCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("SAMPLE START/END", rpX + 8, trimY + 7, rpW - 16, 10, juce::Justification::left);

    // ── Knob labels ───────────────────────────────────────────────────────────
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    auto drawKnobLabel = [&](const juce::String& text, int secY, int knobX)
    {
        int labelY = secY + 22;
        g.setColour(juce::Colour(0xff666666));
        g.drawText(text, knobX - 8, labelY, knobW + 16, 12,
                   juce::Justification::centred);
    };
    drawKnobLabel("VOLUME",    ampY,   rpX + kxOff0);
    drawKnobLabel("PAN",       ampY,   rpX + kxOff1);
    drawKnobLabel("LOW",       toneY,  rpX + kxOff0);
    drawKnobLabel("HIGH",      toneY,  rpX + kxOff1);
    drawKnobLabel("SEMITONE",  pitchY, rpX + kxOff0);
    drawKnobLabel("FINE TUNE", pitchY, rpX + kxOff1);
    drawKnobLabel("START",     trimY,  rpX + kxOff0);
    drawKnobLabel("END",       trimY,  rpX + kxOff1);

    // ── Footer ────────────────────────────────────────────────────────────────
    g.setColour(RRColors::sectionBorder.darker(0.5f));
    g.fillRect(0, getHeight() - 30, getWidth(), 1);
    g.setColour(RRColors::companyText);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("Alex Hamadey", 0, getHeight() - 26, getWidth() - 14, 20,
               juce::Justification::right);
}

//==============================================================================
// PluginEditor.cpp — paintOverChildren()

void NewProjectAudioProcessorEditor::paintOverChildren(juce::Graphics& g)
{
    if (aboutWindow.isVisible())
    {
        juce::RectangleList<int> clip(getLocalBounds());
        clip.subtract(aboutWindow.getBounds());
        g.reduceClipRegion(clip);
    }

    constexpr float twoPi  = juce::MathConstants<float>::twoPi;
    constexpr float maxArc = juce::MathConstants<float>::pi * 0.8f;
    constexpr float arcW   = 2.0f;
    constexpr int   tbH    = 16;

    auto drawArcOutline = [&](juce::Slider& knob,
                              juce::Slider& negSlider,
                              juce::Slider& posSlider,
                              juce::Colour  col)
    {
        auto getNorm = [](juce::Slider& s) -> float {
            double range = s.getMaximum() - s.getMinimum();
            if (range == 0.0) return 0.0f;
            return (float)((s.getValue() - s.getMinimum()) / range);
        };

        float negNorm = getNorm(negSlider);
        float posNorm = getNorm(posSlider);
        if (negNorm < 0.01f && posNorm < 0.01f) return;

        auto  b  = knob.getBounds();
        float w  = (float)b.getWidth();
        float h  = (float)(b.getHeight() - tbH);
        float cx = b.getX() + w * 0.5f;
        float cy = b.getY() + h * 0.5f;

        float knobRadius = juce::jmin(w, h) * 0.5f - 4.0f;
        float radius     = knobRadius + 4.0f;

        float negExtent = negNorm * maxArc;
        float posExtent = posNorm * maxArc;

        // Neg arc: counter-clockwise from 12 o'clock
        if (negExtent > 0.01f)
        {
            juce::Path negArc;
            negArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                twoPi - negExtent, twoPi, true);
            g.setColour(col.withAlpha(0.7f));
            g.strokePath(negArc, juce::PathStrokeType(arcW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Pos arc: clockwise from 12 o'clock
        if (posExtent > 0.01f)
        {
            juce::Path posArc;
            posArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                0.0f, posExtent, true);
            g.setColour(col.withAlpha(0.7f));
            g.strokePath(posArc, juce::PathStrokeType(arcW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
    };

    drawArcOutline(semitoneSlider,     semitoneRndNegSlider,     semitoneRndPosSlider,     RRColors::pitchCol);
    drawArcOutline(fineTuneSlider,     fineTuneRndNegSlider,     fineTuneRndPosSlider,     RRColors::pitchCol);
    drawArcOutline(volumeSlider,       volumeRndNegSlider,       volumeRndPosSlider,       RRColors::ampCol);
    drawArcOutline(panSlider,          panRndNegSlider,          panRndPosSlider,          RRColors::ampCol);
    drawArcOutline(toneLowSlider,      toneLowRndNegSlider,      toneLowRndPosSlider,      RRColors::toneCol);
    drawArcOutline(toneHighSlider,     toneHighRndNegSlider,     toneHighRndPosSlider,     RRColors::toneCol);
    drawArcOutline(sampleStartSlider,  sampleStartRndNegSlider,  sampleStartRndPosSlider,  RRColors::trimCol);
    drawArcOutline(sampleEndSlider,    sampleEndRndNegSlider,    sampleEndRndPosSlider,    RRColors::trimCol);
}

void NewProjectAudioProcessorEditor::componentVisibilityChanged(juce::Component& component)
{
    if (&component == &aboutWindow)
        repaint();   // redraws arcs when window opens OR closes
}

//==============================================================================
void NewProjectAudioProcessorEditor::resized()
{
    // ── Layout constants (must match paint() exactly) ─────────────────────────
    constexpr int margin  = 12;
    constexpr int knobW   = 68;
    constexpr int knobH   = 80;   // includes 16px text box
    constexpr int tbH     = 16;
    constexpr int knobGap = 24;   // horizontal gap between knob pair

    // Right panel — Amplitude + Tone + Pitch sections
    constexpr int rpX    = 356;
    constexpr int rpW    = 332;   // 700 - 356 - 12
    constexpr int ampY   = 60;
    constexpr int ampH   = 140;
    constexpr int toneY  = ampY + ampH + 8;
    constexpr int toneH  = 140;
    constexpr int pitchY = toneY + toneH + 8;
    constexpr int pitchH = 140;
    constexpr int trimY  = pitchY + pitchH + 8;
    constexpr int trimH  = 140;

    // Knob X offsets within right panel (two knobs centered)
    constexpr int kxOff0 = (rpW - 2 * knobW - knobGap) / 2;  // 86
    constexpr int kxOff1 = kxOff0 + knobW + knobGap;          // 178

    // ── Header: preset buttons + about — all inside header bar ───────────────
    triggerButton.setBounds   (getWidth() - 310, 13, 70, 26);
    savePresetButton.setBounds(getWidth() - 232, 13, 80, 26);
    loadPresetButton.setBounds(getWidth() - 144, 13, 80, 26);
    aboutButton.setBounds    (getWidth() -  56, 13, 26, 26);

    // ── Left panel: Sample Manager ──────────────────────────────────────────
    {
        const int lpW = rpX - margin - 8;
        const int lpH = trimY + trimH - ampY;
        sampleManagerPanel.setBounds(margin, ampY, lpW, lpH);
    }
    // Load Samples + Playback Mode now inside SampleManagerPanel

    // ── Knob positioning helper ─────────────────────────────────────────────
    // Layout per section (top to bottom):
    //   secY+22..34    = knob name label
    //   centered       = knob (80px: 64px rotary + 16px value text)
    //   bottom-14      = rnd bar (12px + 2px gap)
    constexpr int nameBottom = 34;   // name label ends here (within section)
    constexpr int barSpace   = 14;   // rnd bar (12) + gap (2) at section bottom

    auto placeKnobPair = [&](juce::Slider& knob0, juce::Slider& knob1, int secY, int secH)
    {
        int available = secH - nameBottom - barSpace;
        int ky = secY + nameBottom + (available - knobH) / 2;
        knob0.setBounds(rpX + kxOff0, ky, knobW, knobH);
        knob1.setBounds(rpX + kxOff1, ky, knobW, knobH);
    };

    placeKnobPair(volumeSlider,       panSlider,          ampY,   ampH);
    placeKnobPair(toneLowSlider,      toneHighSlider,     toneY,  toneH);
    placeKnobPair(semitoneSlider,     fineTuneSlider,     pitchY, pitchH);
    placeKnobPair(sampleStartSlider,  sampleEndSlider,    trimY,  trimH);

    // ── Dual-thumb randomization sliders (below each knob, above text box) ──
    auto placeRndBar = [&](DualThumbRndSlider& bar, juce::Slider& knob, int secY, int secH)
    {
        auto b = knob.getBounds();
        constexpr int barH = 12;
        int barY = secY + secH - barH - 2;
        bar.setBounds(b.getX() + 2, barY, b.getWidth() - 4, barH);
    };

    placeRndBar(volumeRndBar,       volumeSlider,       ampY,   ampH);
    placeRndBar(panRndBar,          panSlider,          ampY,   ampH);
    placeRndBar(toneLowRndBar,      toneLowSlider,      toneY,  toneH);
    placeRndBar(toneHighRndBar,     toneHighSlider,     toneY,  toneH);
    placeRndBar(semitoneRndBar,     semitoneSlider,     pitchY, pitchH);
    placeRndBar(fineTuneRndBar,     fineTuneSlider,     pitchY, pitchH);
    placeRndBar(sampleStartRndBar,  sampleStartSlider,  trimY,  trimH);
    placeRndBar(sampleEndRndBar,    sampleEndSlider,    trimY,  trimH);
}
