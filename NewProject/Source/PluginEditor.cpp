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
    randomAlgorithmAttachment(p.apvts, ParameterIDs::randomAlgorithm, randomAlgorithmSlider),
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
    sampleManagerPanel.onClearSamplesClicked = [this]()
        {
            for (int i = 0; i < NewProjectAudioProcessor::NUM_SAMPLE_SLOTS; ++i)
                audioProcessor.sampleLoader.clearSlot(i);

            audioProcessor.rebuildLoadedIndices();
            audioProcessor.reshuffleIndices();
            audioProcessor.sampleLoader.updateSynthesiserSounds();
            sampleManagerPanel.repaint();
        };
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
    triggerButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff8a3030));
    triggerButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffe0d0d0));
    triggerButton.setLookAndFeel(&buttonLAF);
    triggerButton.onClick = [this]() { audioProcessor.requestTrigger(); };
    addAndMakeVisible(triggerButton);

    // About button
    aboutButton.setButtonText("?");
    aboutButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2a3530));
    aboutButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xff808880));
    aboutButton.setLookAndFeel(&buttonLAF);
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
    savePresetButton.setButtonText("Save");
    savePresetButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2a3a30));
    savePresetButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffa0b0a0));
    savePresetButton.setLookAndFeel(&buttonLAF);
    savePresetButton.onClick = [this]() { savePreset(); };
    addAndMakeVisible(savePresetButton);

    loadPresetButton.setButtonText("Load");
    loadPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2a3a30));
    loadPresetButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffa0b0a0));
    loadPresetButton.setLookAndFeel(&buttonLAF);
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

    // Random Algorithm knob — larger, integer snapping, no randomization arc
    randomAlgorithmSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    randomAlgorithmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 16);
    randomAlgorithmSlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(randomAlgorithmSlider);

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

        randomAlgorithmSlider.setLookAndFeel(&knobLAF);
        randomAlgorithmSlider.setColour(juce::Slider::rotarySliderFillColourId, RRColors::algoCol);

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
        setValueBoxColors(randomAlgorithmSlider, RRColors::algoCol);
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

    // Repaint arc outlines when main knobs move (arcs track knob position)
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &toneLowSlider, &toneHighSlider, &sampleStartSlider, &sampleEndSlider })
        s->onValueChange = [this] { repaint(); };

    setSize(1080, 504);
    resized();
}

//==============================================================================
NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &toneLowSlider, &toneHighSlider,
                     &sampleStartSlider, &sampleEndSlider,
                     &randomAlgorithmSlider })
        s->setLookAndFeel(nullptr);

    for (auto* b : { &triggerButton, &savePresetButton, &loadPresetButton, &aboutButton })
        b->setLookAndFeel(nullptr);
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
    constexpr int margin   = 12;
    constexpr int knobW    = 68;
    constexpr int topY     = 60;
    constexpr int gap      = 8;
    constexpr int headerH  = 52;
    constexpr int footerH  = 28;
    constexpr int botSecH  = 130;    // left bottom-row section height

    // All bottom borders align here
    constexpr int secBot   = 504 - footerH - 8;             // 468
    constexpr int botY     = secBot - botSecH;               // 338

    // Left panel
    constexpr int lpW      = 480;
    constexpr int lpH      = botY - topY - gap;              // 270 (sample pool)
    constexpr int lpHalfW  = (lpW - gap) / 2;               // 236

    // Left bottom: Sample Start/End (left) + Tone (right)
    constexpr int trimX    = margin;
    constexpr int toneX    = margin + lpHalfW + gap;

    // Right area — 3 columns: [Algorithm] [Amp/Pitch stack]
    constexpr int rpX      = margin + lpW + gap;             // 500
    constexpr int rpW      = 1080 - rpX - margin;            // 568
    constexpr int rpHalfW  = (rpW - gap) / 2;               // 280
    constexpr int fullH    = secBot - topY;                  // 408 (full right height)

    // Algorithm: left half, full height
    constexpr int algoX    = rpX;
    constexpr int algoW    = rpHalfW;                        // 280

    // Amp (top) + Pitch (bottom): right half, stacked
    constexpr int apX      = rpX + rpHalfW + gap;            // 788
    constexpr int apW      = rpHalfW;                        // 280
    constexpr int apSecH   = (fullH - gap) / 2;             // 200
    constexpr int ampY     = topY;
    constexpr int pitchY   = topY + apSecH + gap;            // 268

    // Knob X offsets (right-side sections, 280px wide)
    constexpr int knobGap  = 16;
    constexpr int rpKx0    = (apW - 2 * knobW - knobGap) / 2;
    constexpr int rpKx1    = rpKx0 + knobW + knobGap;

    // Knob X offsets (left bottom sections, 236px wide)
    constexpr int lpKx0    = (lpHalfW - 2 * knobW - knobGap) / 2;
    constexpr int lpKx1    = lpKx0 + knobW + knobGap;

    // ── Background ────────────────────────────────────────────────────────────
    g.fillAll(RRColors::background);

    // ── Header bar (subtle gradient) ────────────────────────────────────────
    {
        juce::ColourGradient hdrGrad(
            RRColors::headerBg.brighter(0.05f), 0.0f, 0.0f,
            RRColors::headerBg,                 0.0f, (float)headerH,
            false);
        g.setGradientFill(hdrGrad);
        g.fillRect(0, 0, getWidth(), headerH);
    }
    // Divider: dark line + subtle highlight below
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.fillRect(0, headerH, getWidth(), 1);
    g.setColour(juce::Colours::white.withAlpha(0.03f));
    g.fillRect(0, headerH + 1, getWidth(), 1);

    juce::Font rrFont(juce::FontOptions(26.0f));
    rrFont = rrFont.boldened();
    g.setFont(rrFont);
    g.setColour(juce::Colour(0xffc8c8c8));
    g.drawText("RoundRobin", 14, 10, 200, 32, juce::Justification::left);

    const int liteX = 14 + rrFont.getStringWidth("RoundRobin") + 3;
    g.setColour(juce::Colour(0xff808080));
    g.setFont(juce::Font(juce::FontOptions(17.0f)));
    g.drawText("Lite", liteX, 14, 60, 26, juce::Justification::left);

    // ── Section box helper (recessed panel look) ───────────────────────────
    auto drawSectionBox = [&](juce::Rectangle<int> r)
    {
        // Outer shadow (depth)
        g.setColour(juce::Colours::black.withAlpha(0.25f));
        g.fillRoundedRectangle(r.toFloat().translated(1.0f, 1.5f), 4.0f);
        // Panel fill
        g.setColour(RRColors::sectionBg);
        g.fillRoundedRectangle(r.toFloat(), 4.0f);
        // Top inner highlight (subtle bevel)
        {
            auto top = r.toFloat().removeFromTop(1.0f).reduced(4.0f, 0.0f);
            g.setColour(juce::Colours::white.withAlpha(0.03f));
            g.fillRect(top);
        }
        // Border
        g.setColour(RRColors::sectionBorder);
        g.drawRoundedRectangle(r.toFloat(), 4.0f, 1.0f);
    };

    // ── Left: Sample Pool ───────────────────────────────────────────────────
    drawSectionBox({ margin, topY, lpW, lpH });

    // ── Left bottom-left: Sample Start/End ──────────────────────────────────
    drawSectionBox({ trimX, botY, lpHalfW, botSecH });
    g.setColour(RRColors::trimCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("SAMPLE START/END", trimX + 8, botY + 7, lpHalfW - 16, 10, juce::Justification::left);

    // ── Left bottom-right: Tone ─────────────────────────────────────────────
    drawSectionBox({ toneX, botY, lpHalfW, botSecH });
    g.setColour(RRColors::toneCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("TONE", toneX + 8, botY + 7, lpHalfW - 16, 10, juce::Justification::left);

    // ── Right: Random Algorithm (left half, full height) ────────────────────
    drawSectionBox({ algoX, topY, algoW, fullH });
    g.setColour(RRColors::algoCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("RANDOM ALGORITHM", algoX + 8, topY + 7, algoW - 16, 10, juce::Justification::left);

    // ── Random Algorithm tick marks (18 ticks) ──────────────────────────────
    {
        constexpr int numTicks = 18;
        constexpr int tbH     = 16;

        auto kb = randomAlgorithmSlider.getBounds();
        auto rp = randomAlgorithmSlider.getRotaryParameters();

        const float w  = (float)kb.getWidth();
        const float h  = (float)(kb.getHeight() - tbH);
        const float cx = kb.getX() + w * 0.5f;
        const float cy = kb.getY() + h * 0.5f;

        const float bodyRadius = juce::jmin(w, h) * 0.5f - 2.5f;
        const float tickInner  = bodyRadius + 2.0f;
        const float tickOuter  = bodyRadius + 7.0f;

        g.setColour(RRColors::algoCol.withAlpha(0.45f));
        for (int i = 0; i < numTicks; ++i)
        {
            float norm  = (float)i / (float)(numTicks - 1);
            float angle = rp.startAngleRadians + norm * (rp.endAngleRadians - rp.startAngleRadians);
            float sinA  = std::sin(angle);
            float cosA  = std::cos(angle);
            g.drawLine(cx + sinA * tickInner, cy - cosA * tickInner,
                       cx + sinA * tickOuter, cy - cosA * tickOuter, 1.5f);
        }
    }

    // ── Right top: Amplitude ────────────────────────────────────────────────
    drawSectionBox({ apX, ampY, apW, apSecH });
    g.setColour(RRColors::ampCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("AMPLITUDE", apX + 8, ampY + 7, apW - 16, 10, juce::Justification::left);

    // ── Right bottom: Pitch ─────────────────────────────────────────────────
    drawSectionBox({ apX, pitchY, apW, apSecH });
    g.setColour(RRColors::pitchCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("PITCH", apX + 8, pitchY + 7, apW - 16, 10, juce::Justification::left);

    // ── Knob labels ─────────────────────────────────────────────────────────
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    auto drawKnobLabel = [&](const juce::String& text, int secY, int secHeight,
                             int knobX, juce::Colour col)
    {
        int padTop = (secHeight - 20 - 108) / 2;
        int labelY = secY + 20 + padTop;
        g.setColour(col.withAlpha(0.55f));
        g.drawText(text, knobX - 8, labelY, knobW + 16, 12,
                   juce::Justification::centred);
    };
    drawKnobLabel("Volume",     ampY,   apSecH,  apX + rpKx0,     RRColors::ampCol);
    drawKnobLabel("Pan",        ampY,   apSecH,  apX + rpKx1,     RRColors::ampCol);
    drawKnobLabel("Semitone",   pitchY, apSecH,  apX + rpKx0,     RRColors::pitchCol);
    drawKnobLabel("Fine Tune",  pitchY, apSecH,  apX + rpKx1,     RRColors::pitchCol);
    drawKnobLabel("Start",      botY,   botSecH, trimX + lpKx0,   RRColors::trimCol);
    drawKnobLabel("End",        botY,   botSecH, trimX + lpKx1,   RRColors::trimCol);
    drawKnobLabel("Low",        botY,   botSecH, toneX + lpKx0,   RRColors::toneCol);
    drawKnobLabel("High",       botY,   botSecH, toneX + lpKx1,   RRColors::toneCol);

    // ── Footer ──────────────────────────────────────────────────────────────
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillRect(0, getHeight() - footerH, getWidth(), 1);
    g.setColour(RRColors::companyText);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("Alex Hamadey", 0, getHeight() - footerH + 4, getWidth() - 14, 20,
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

        // Compute the knob's current angle from its value
        float knobNorm = getNorm(knob);
        auto rp = knob.getRotaryParameters();
        float startA    = rp.startAngleRadians;   // ~7 o'clock
        float endA      = rp.endAngleRadians;     // ~5 o'clock
        float knobAngle = startA + knobNorm * (endA - startA);

        // Available arc room from knob position to each rotary limit
        float roomNeg = knobAngle - startA;   // room toward 7 o'clock
        float roomPos = endA - knobAngle;     // room toward 5 o'clock

        float negExtent = negNorm * roomNeg;  // at max rnd → fills to 7 o'clock
        float posExtent = posNorm * roomPos;  // at max rnd → fills to 5 o'clock

        auto  b  = knob.getBounds();
        float w  = (float)b.getWidth();
        float h  = (float)(b.getHeight() - tbH);
        float cx = b.getX() + w * 0.5f;
        float cy = b.getY() + h * 0.5f;

        float knobRadius = juce::jmin(w, h) * 0.5f - 2.5f;  // match body radius
        float radius     = knobRadius;                     // draw right on the perimeter

        // Neg arc: counter-clockwise from knob position (clamped to start)
        if (negExtent > 0.01f)
        {
            juce::Path negArc;
            negArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                knobAngle - negExtent, knobAngle, true);
            g.setColour(col.withAlpha(0.7f));
            g.strokePath(negArc, juce::PathStrokeType(arcW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Pos arc: clockwise from knob position (clamped to end)
        if (posExtent > 0.01f)
        {
            juce::Path posArc;
            posArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                knobAngle, knobAngle + posExtent, true);
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
    constexpr int margin   = 12;
    constexpr int knobW    = 68;
    constexpr int knobH    = 80;   // 64 rotary + 16 text box
    constexpr int knobGap  = 16;
    constexpr int topY     = 60;
    constexpr int gap      = 8;
    constexpr int footerH  = 28;
    constexpr int botSecH  = 130;

    // All bottom borders align here
    constexpr int secBot   = 504 - footerH - 8;             // 468
    constexpr int botY     = secBot - botSecH;               // 338
    constexpr int fullH    = secBot - topY;                  // 408

    // Left panel
    constexpr int lpW      = 480;
    constexpr int lpH      = botY - topY - gap;              // 270
    constexpr int lpHalfW  = (lpW - gap) / 2;               // 236

    constexpr int trimX    = margin;
    constexpr int toneX    = margin + lpHalfW + gap;

    // Right area: [Algorithm | Amp/Pitch stack]
    constexpr int rpX      = margin + lpW + gap;             // 500
    constexpr int rpW      = 1080 - rpX - margin;            // 568
    constexpr int rpHalfW  = (rpW - gap) / 2;               // 280

    constexpr int algoX    = rpX;
    constexpr int algoW    = rpHalfW;
    constexpr int apX      = rpX + rpHalfW + gap;            // 788
    constexpr int apW      = rpHalfW;
    constexpr int apSecH   = (fullH - gap) / 2;             // 200
    constexpr int ampY     = topY;
    constexpr int pitchY   = topY + apSecH + gap;            // 268

    // Knob X offsets (right Amp/Pitch sections, 280px)
    constexpr int rpKx0    = (apW - 2 * knobW - knobGap) / 2;
    constexpr int rpKx1    = rpKx0 + knobW + knobGap;

    // Knob X offsets (left bottom sections, 236px)
    constexpr int lpKx0    = (lpHalfW - 2 * knobW - knobGap) / 2;
    constexpr int lpKx1    = lpKx0 + knobW + knobGap;

    // ── Header buttons ──────────────────────────────────────────────────────
    loadPresetButton.setBounds(getWidth() - 298, 13, 60, 26);
    savePresetButton.setBounds(getWidth() - 230, 13, 60, 26);
    triggerButton.setBounds   (getWidth() - 162, 13, 70, 26);
    aboutButton.setBounds     (getWidth() -  56, 13, 26, 26);

    // ── Left: Sample Manager ────────────────────────────────────────────────
    sampleManagerPanel.setBounds(margin, topY, lpW, lpH);

    // ── Knob pair placement (centered vertically in section) ────────────────
    auto placeKnobPair = [&](juce::Slider& knob0, juce::Slider& knob1,
                             int secY, int secHeight, int xBase, int xOff0, int xOff1)
    {
        int padTop = (secHeight - 20 - 108) / 2;
        int ky = secY + 20 + padTop + 14;          // 12px label + 2px gap
        knob0.setBounds(xBase + xOff0, ky, knobW, knobH);
        knob1.setBounds(xBase + xOff1, ky, knobW, knobH);
    };

    placeKnobPair(volumeSlider,      panSlider,         ampY,   apSecH,  apX,   rpKx0, rpKx1);
    placeKnobPair(semitoneSlider,    fineTuneSlider,    pitchY, apSecH,  apX,   rpKx0, rpKx1);
    placeKnobPair(sampleStartSlider, sampleEndSlider,   botY,   botSecH, trimX, lpKx0, lpKx1);
    placeKnobPair(toneLowSlider,     toneHighSlider,    botY,   botSecH, toneX, lpKx0, lpKx1);

    // ── Algorithm knob (centered in its tall section) ───────────────────────
    {
        constexpr int algoKnobW = 120;
        constexpr int algoKnobH = 136;  // 120 rotary + 16 text box
        int algoKnobX = algoX + (algoW - algoKnobW) / 2;
        int algoKnobY = topY + 20 + (fullH - 20 - algoKnobH) / 2;
        randomAlgorithmSlider.setBounds(algoKnobX, algoKnobY, algoKnobW, algoKnobH);
    }

    // ── Dual-thumb rnd sliders (2px below knob text box) ────────────────────
    auto placeRndBar = [&](DualThumbRndSlider& bar, juce::Slider& knob)
    {
        auto b = knob.getBounds();
        constexpr int barH = 12;
        int barY = b.getBottom() + 2;
        bar.setBounds(b.getX() + 2, barY, b.getWidth() - 4, barH);
    };

    placeRndBar(volumeRndBar,       volumeSlider);
    placeRndBar(panRndBar,          panSlider);
    placeRndBar(semitoneRndBar,     semitoneSlider);
    placeRndBar(fineTuneRndBar,     fineTuneSlider);
    placeRndBar(toneLowRndBar,      toneLowSlider);
    placeRndBar(toneHighRndBar,     toneHighSlider);
    placeRndBar(sampleStartRndBar,  sampleStartSlider);
    placeRndBar(sampleEndRndBar,    sampleEndSlider);
}
