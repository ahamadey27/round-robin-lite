#include "PluginProcessor.h"
#include "PluginEditor.h"

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    semitoneAttachment(p.apvts, ParameterIDs::semitone, semitoneSlider),
    fineTuneAttachment(p.apvts, ParameterIDs::fineTune, fineTuneSlider),
    volumeAttachment(p.apvts, ParameterIDs::volume, volumeSlider),
    panAttachment(p.apvts, ParameterIDs::pan, panSlider),
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
    playbackModeAttachment(p.apvts, ParameterIDs::playbackMode, playbackModeButton)
{
    // Sample Load Button
    loadSamplesButton.setButtonText("Load Samples");
    loadSamplesButton.onClick = [this]() { loadSamplesFromFiles(); };
    addAndMakeVisible(loadSamplesButton);

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

    // Playback mode button
    playbackModeButton.setButtonText("Series");
    playbackModeButton.setClickingTogglesState(true);
    playbackModeButton.onClick = [this]()
        {
            bool isRandom = playbackModeButton.getToggleState();
            playbackModeButton.setButtonText(isRandom ? "Random" : "Series");
        };
    addAndMakeVisible(playbackModeButton);
    playbackModeButton.setLookAndFeel(&toggleLAF);

    // Label setup:
    //playbackTypeLabel.setText("Playback Type", juce::dontSendNotification);
    //playbackTypeLabel.setFont(juce::Font(juce::FontOptions(10.0f)));
    //playbackTypeLabel.setColour(juce::Label::textColourId, juce::Colour(180, 180, 195));
    //playbackTypeLabel.setJustificationType(juce::Justification::centred);
    //addAndMakeVisible(playbackTypeLabel);

    // Main parameter knobs — visible, on the editor
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider
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
                     &panRndNegSlider,       &panRndPosSlider
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
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider
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


    setSize(700, 420);
    // Overlay must be added AFTER all knobs so it sits on top
    addAndMakeVisible(arcOverlay);
    resized();
}

static constexpr float kRndDotOffset = 0.18f;   // ~15 degrees

//==============================================================================
NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider
                     // COMMENTED FOR LITE — ACTIVE IN PREMIUM
                     //,&lowGainSlider, &lowFreqSlider, &midGainSlider, &midFreqSlider,
                     //,&highGainSlider, &highFreqSlider,
                     //,&transientAttackSlider, &transientDecaySlider,
                     //,&envAttackSlider, &envDecaySlider
                     })
        s->setLookAndFeel(nullptr);

    for (auto* s : { &semitoneRndNegSlider, &fineTuneRndNegSlider,
                     &volumeRndNegSlider,   &panRndNegSlider,
                     // COMMENTED FOR LITE — ACTIVE IN PREMIUM
                     //&lowGainRndNegSlider,  &lowFreqRndNegSlider,
                     //&midGainRndNegSlider,  &midFreqRndNegSlider,
                     //&highGainRndNegSlider, &highFreqRndNegSlider,
                     //&transAtkRndNegSlider, &transDecRndNegSlider,
                     //&envAtkRndNegSlider,   &envDecRndNegSlider,
                     &semitoneRndPosSlider, &fineTuneRndPosSlider,
                     &volumeRndPosSlider,   &panRndPosSlider
                     // COMMENTED FOR LITE — ACTIVE IN PREMIUM
                     //,&lowGainRndPosSlider,  &lowFreqRndPosSlider,
                     //,&midGainRndPosSlider,  &midFreqRndPosSlider,
                     //,&highGainRndPosSlider, &highFreqRndPosSlider,
                     //,&transAtkRndPosSlider, &transDecRndPosSlider,
                     //,&envAtkRndPosSlider,   &envDecRndPosSlider
                     })
        s->setLookAndFeel(nullptr);

        playbackModeButton.setLookAndFeel(nullptr);
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
            updateSamplesInfo();
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
    constexpr int pitchY  = ampY + ampH + 50;   // 250
    constexpr int pitchH  = 140;
    constexpr int kxOff0  = (rpW - 2 * knobW - knobGap) / 2;  // 86
    constexpr int kxOff1  = kxOff0 + knobW + knobGap;          // 178

    const int secLabelBottomAmp   = ampY   + 7 + 9;
    const int secLabelBottomPitch = pitchY + 7 + 9;

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
    const int lpH = pitchY + pitchH - ampY;  // total height of both right sections
    drawSectionBox({ margin, ampY, lpW, lpH });
    g.setColour(juce::Colour(0xff3a3a4a));
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("SAMPLE MANAGER", margin + 8, ampY + 7, lpW - 16, 10,
               juce::Justification::left);

    // ── Right panel: Amplitude section ───────────────────────────────────────
    drawSectionBox({ rpX, ampY, rpW, ampH });
    g.setColour(RRColors::ampCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("AMPLITUDE", rpX + 8, ampY + 7, rpW - 16, 10, juce::Justification::left);

    // ── Right panel: Algorithm placeholder (Phase 5) ─────────────────────────
    g.setColour(juce::Colour(0xff2e2e3e));
    g.setFont(juce::Font(juce::FontOptions(8.0f)));
    g.drawText("[ ALGORITHM ]", rpX, ampY + ampH + 16, rpW, 12,
               juce::Justification::centred);

    // ── Right panel: Pitch section ────────────────────────────────────────────
    drawSectionBox({ rpX, pitchY, rpW, pitchH });
    g.setColour(RRColors::pitchCol);
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("PITCH", rpX + 8, pitchY + 7, rpW - 16, 10, juce::Justification::left);

    // ── Knob labels ───────────────────────────────────────────────────────────
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    auto drawKnobLabel = [&](const juce::String& text, juce::Slider& s, int secLabelBottom)
    {
        auto b = s.getBounds();
        int centerY = (secLabelBottom + b.getY()) / 2;
        g.setColour(juce::Colour(0xff666666));
        g.drawText(text, b.getX() - 8, centerY - 6, b.getWidth() + 16, 12,
                   juce::Justification::centred);
    };
    drawKnobLabel("VOLUME",    volumeSlider,   secLabelBottomAmp);
    drawKnobLabel("PAN",       panSlider,       secLabelBottomAmp);
    drawKnobLabel("SEMITONE",  semitoneSlider,  secLabelBottomPitch);
    drawKnobLabel("FINE TUNE", fineTuneSlider,  secLabelBottomPitch);

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
    // Skip drawing arc overlay entirely when about window is open
    if (aboutWindow.isVisible())
    {
        juce::RectangleList<int> clip(getLocalBounds());
        clip.subtract(aboutWindow.getBounds());
        g.reduceClipRegion(clip);   // ← arcs render outside window bounds only
    }
            
    constexpr float pi     = juce::MathConstants<float>::pi;
    constexpr float twoPi  = juce::MathConstants<float>::twoPi;
    constexpr float maxNeg = pi * 0.8f;
    constexpr float maxPos = pi * 0.8f;
    constexpr float trackW = 5.0f;
    constexpr float arcW   = 6.5f;
    constexpr float dotR   = 5.0f;
    constexpr int   tbH    = 16;

    // Muted gray-green dot color — fits dark forest palette, not harsh white
    const juce::Colour dotCol { 0xff8a9a8a };

    auto drawRndArcs = [&](juce::Slider& knob,
                           juce::Slider& negSlider,
                           juce::Slider& posSlider,
                           juce::Colour  negCol,
                           juce::Colour  posCol)
    {
        auto  b  = knob.getBounds();
        float w  = (float)b.getWidth();
        float h  = (float)(b.getHeight() - tbH);

        // cy matches new drawRotarySlider: cy = y + (height-tbH)*0.5
        float cx = b.getX() + w * 0.5f;
        float cy = b.getY() + h * 0.5f;

        // Arc sits 5px outside the knob rim — same knob radius formula as LAF
        float knobRadius = juce::jmin(w, h) * 0.5f - 4.0f;
        float radius     = knobRadius + 5.0f;

        auto getNorm = [](juce::Slider& s) -> float {
            double range = s.getMaximum() - s.getMinimum();
            if (range == 0.0) return 0.0f;
            return (float)((s.getValue() - s.getMinimum()) / range);
        };

        float negExtent = getNorm(negSlider) * (maxNeg - kRndDotOffset);
        float posExtent = getNorm(posSlider) * (maxPos - kRndDotOffset);

        // Dim background tracks
        juce::Path negTrack;
        negTrack.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
            twoPi - maxNeg, twoPi, true);
        g.setColour(negCol.withAlpha(0.25f));
        g.strokePath(negTrack, juce::PathStrokeType(trackW,
            juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        juce::Path posTrack;
        posTrack.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
            0.0f, maxPos, true);
        g.setColour(posCol.withAlpha(0.25f));
        g.strokePath(posTrack, juce::PathStrokeType(trackW,
            juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Active neg arc
        if (negExtent > 0.01f)
        {
            juce::Path negArc;
            negArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                twoPi - negExtent, twoPi, true);
            g.setColour(negCol.withAlpha(0.9f));
            g.strokePath(negArc, juce::PathStrokeType(arcW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Active pos arc
        if (posExtent > 0.01f)
        {
            juce::Path posArc;
            posArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                0.0f, posExtent, true);
            g.setColour(posCol.withAlpha(0.9f));
            g.strokePath(posArc, juce::PathStrokeType(arcW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Muted gray-green dots at arc endpoints (grab handles)
        float negDotX = cx - std::sin(negExtent + kRndDotOffset) * radius;
        float negDotY = cy - std::cos(negExtent + kRndDotOffset) * radius;
        g.setColour(dotCol);
        g.fillEllipse(negDotX - dotR, negDotY - dotR, dotR * 2.0f, dotR * 2.0f);

        float posDotX = cx + std::sin(posExtent + kRndDotOffset) * radius;
        float posDotY = cy - std::cos(posExtent + kRndDotOffset) * radius;
        g.setColour(dotCol);
        g.fillEllipse(posDotX - dotR, posDotY - dotR, dotR * 2.0f, dotR * 2.0f);
    };

    // PITCH
    drawRndArcs(semitoneSlider,        semitoneRndNegSlider,  semitoneRndPosSlider,  RRColors::pitchNeg,  RRColors::pitchCol);
    drawRndArcs(fineTuneSlider,        fineTuneRndNegSlider,  fineTuneRndPosSlider,  RRColors::pitchNeg,  RRColors::pitchCol);
    // AMPLITUDE
    drawRndArcs(volumeSlider,          volumeRndNegSlider,    volumeRndPosSlider,    RRColors::ampNeg,    RRColors::ampCol);
    drawRndArcs(panSlider,             panRndNegSlider,       panRndPosSlider,       RRColors::ampNeg,    RRColors::ampCol);
    // COMMENTED FOR LITE — ACTIVE IN PREMIUM
    // ENVELOPE
    //drawRndArcs(envAttackSlider,       envAtkRndNegSlider,    envAtkRndPosSlider,    RRColors::envNeg,    RRColors::envCol);
    //drawRndArcs(envDecaySlider,        envDecRndNegSlider,    envDecRndPosSlider,    RRColors::envNeg,    RRColors::envCol);
    // TRANSIENT
    //drawRndArcs(transientAttackSlider, transAtkRndNegSlider,  transAtkRndPosSlider,  RRColors::transNeg,  RRColors::transCol);
    //drawRndArcs(transientDecaySlider,  transDecRndNegSlider,  transDecRndPosSlider,  RRColors::transNeg,  RRColors::transCol);
    // EQ LOW
    //drawRndArcs(lowGainSlider,         lowGainRndNegSlider,   lowGainRndPosSlider,   RRColors::eqLowNeg,  RRColors::eqLowCol);
    //drawRndArcs(lowFreqSlider,         lowFreqRndNegSlider,   lowFreqRndPosSlider,   RRColors::eqLowNeg,  RRColors::eqLowCol);
    // EQ MID
    //drawRndArcs(midGainSlider,         midGainRndNegSlider,   midGainRndPosSlider,   RRColors::eqMidNeg,  RRColors::eqMidCol);
    //drawRndArcs(midFreqSlider,         midFreqRndNegSlider,   midFreqRndPosSlider,   RRColors::eqMidNeg,  RRColors::eqMidCol);
    // EQ HIGH
    //drawRndArcs(highGainSlider,        highGainRndNegSlider,  highGainRndPosSlider,  RRColors::eqHighNeg, RRColors::eqHighCol);
    //drawRndArcs(highFreqSlider,        highFreqRndNegSlider,  highFreqRndPosSlider,  RRColors::eqHighNeg, RRColors::eqHighCol);
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

    // Right panel — Pitch + Amplitude sections
    constexpr int rpX    = 356;
    constexpr int rpW    = 332;   // 700 - 356 - 12
    constexpr int ampY   = 60;
    constexpr int ampH   = 140;
    constexpr int pitchY = ampY + ampH + 50;   // 250 — 50px gap reserved for Algorithm knob (Phase 5)
    constexpr int pitchH = 140;

    // Knob X offsets within right panel (two knobs centered)
    constexpr int kxOff0 = (rpW - 2 * knobW - knobGap) / 2;  // 86
    constexpr int kxOff1 = kxOff0 + knobW + knobGap;          // 178

    // ── Header: preset buttons + about — all inside header bar ───────────────
    savePresetButton.setBounds(getWidth() - 226, 13, 80, 26);
    loadPresetButton.setBounds(getWidth() - 138, 13, 80, 26);
    aboutButton.setBounds    (getWidth() -  50, 13, 26, 26);

    // ── Left panel: Sample Manager placeholder ────────────────────────────────
    // Load Samples + Playback Mode temporarily parked here until Phase 3
    loadSamplesButton .setBounds(margin + 8, ampY + 8,  120, 26);
    playbackModeButton.setBounds(margin + 8, ampY + 42,  90, 26);

    // ── Amplitude knobs (Volume, Pan) ─────────────────────────────────────────
    {
        const int ky          = ampY + (ampH - knobH - 12) / 2 + 16;
        const int knobDrawBot = ky + knobH - tbH;
        const int extraPad    = ((ampY + ampH) - knobDrawBot - tbH) / 2;
        volumeSlider.setBounds(rpX + kxOff0, ky, knobW, knobH + extraPad);
        panSlider   .setBounds(rpX + kxOff1, ky, knobW, knobH + extraPad);
    }

    // ── Pitch knobs (Semitone, Fine Tune) ─────────────────────────────────────
    {
        const int ky          = pitchY + (pitchH - knobH - 12) / 2 + 16;
        const int knobDrawBot = ky + knobH - tbH;
        const int extraPad    = ((pitchY + pitchH) - knobDrawBot - tbH) / 2;
        semitoneSlider.setBounds(rpX + kxOff0, ky, knobW, knobH + extraPad);
        fineTuneSlider.setBounds(rpX + kxOff1, ky, knobW, knobH + extraPad);
    }

    // ── Arc overlay (full canvas) ─────────────────────────────────────────────
    arcOverlay.setBounds(getLocalBounds());
}

//==============================================================================
// RndArcOverlay — transparent component sitting over all knobs.
// hitTest returns true only near arc dots, so knobs still receive
// normal mouse events everywhere else.

// PluginEditor.cpp — before RndArcOverlay::hitTest

static void getDotPositions(juce::Slider& knob,
    juce::Slider& negSlider,
    juce::Slider& posSlider,
    juce::Point<float>& negPt,
    juce::Point<float>& posPt)
{
    constexpr float pi     = juce::MathConstants<float>::pi;
    constexpr float maxNeg = pi * 0.8f;
    constexpr float maxPos = pi * 0.8f;
    constexpr int   tbH    = 16;

    auto b  = knob.getBounds();
    float w = (float)b.getWidth();
    float h = (float)(b.getHeight() - tbH);    // match new drawRotarySlider
    float cx = b.getX() + w * 0.5f;
    float cy = b.getY() + h * 0.5f;            // match new drawRotarySlider
    float knobRadius = juce::jmin(w, h) * 0.5f - 4.0f;
    float radius     = knobRadius + 5.0f;       // arc sits 5px outside knob rim

    auto getNorm = [](juce::Slider& s) -> float {
        double range = s.getMaximum() - s.getMinimum();
        if (range == 0.0) return 0.0f;
        return (float)((s.getValue() - s.getMinimum()) / range);
    };

    float negExtent = getNorm(negSlider) * (maxNeg - kRndDotOffset);
    float posExtent = getNorm(posSlider) * (maxPos - kRndDotOffset);

    negPt = { cx - std::sin(negExtent + kRndDotOffset) * radius,
              cy - std::cos(negExtent + kRndDotOffset) * radius };
    posPt = { cx + std::sin(posExtent + kRndDotOffset) * radius,
              cy - std::cos(posExtent + kRndDotOffset) * radius };
}

bool NewProjectAudioProcessorEditor::RndArcOverlay::hitTest(int x, int y)
{
    
    if (editor.aboutWindow.isVisible())   // ← ADD THIS
            return false;
    
    constexpr float hitR = 14.0f;
    auto pos = juce::Point<float>((float)x, (float)y);

    struct Trio { juce::Slider* k; juce::Slider* n; juce::Slider* p; };
    Trio trios[] = {
        { &editor.semitoneSlider,        &editor.semitoneRndNegSlider,  &editor.semitoneRndPosSlider  },
        { &editor.fineTuneSlider,        &editor.fineTuneRndNegSlider,  &editor.fineTuneRndPosSlider  },
        { &editor.volumeSlider,          &editor.volumeRndNegSlider,    &editor.volumeRndPosSlider    },
        { &editor.panSlider,             &editor.panRndNegSlider,       &editor.panRndPosSlider       },
        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        //{ &editor.lowGainSlider,         &editor.lowGainRndNegSlider,   &editor.lowGainRndPosSlider   },
        //{ &editor.lowFreqSlider,         &editor.lowFreqRndNegSlider,   &editor.lowFreqRndPosSlider   },
        //{ &editor.midGainSlider,         &editor.midGainRndNegSlider,   &editor.midGainRndPosSlider   },
        //{ &editor.midFreqSlider,         &editor.midFreqRndNegSlider,   &editor.midFreqRndPosSlider   },
        //{ &editor.highGainSlider,        &editor.highGainRndNegSlider,  &editor.highGainRndPosSlider  },
        //{ &editor.highFreqSlider,        &editor.highFreqRndNegSlider,  &editor.highFreqRndPosSlider  },
        //{ &editor.transientAttackSlider, &editor.transAtkRndNegSlider,  &editor.transAtkRndPosSlider  },
        //{ &editor.transientDecaySlider,  &editor.transDecRndNegSlider,  &editor.transDecRndPosSlider  },
        //{ &editor.envAttackSlider,       &editor.envAtkRndNegSlider,    &editor.envAtkRndPosSlider    },
        //{ &editor.envDecaySlider,        &editor.envDecRndNegSlider,    &editor.envDecRndPosSlider    },
    };

    for (auto& t : trios)
    {
        juce::Point<float> negPt, posPt;
        getDotPositions(*t.k, *t.n, *t.p, negPt, posPt);
        if (pos.getDistanceFrom(negPt) < hitR) return true;
        if (pos.getDistanceFrom(posPt) < hitR) return true;
    }
    return false; // pass through to knob below
}

void NewProjectAudioProcessorEditor::RndArcOverlay::mouseDown(const juce::MouseEvent& e)
{
    activeSlider = nullptr;
    auto pos = e.position;

    struct Trio { juce::Slider* k; juce::Slider* n; juce::Slider* p; };
    Trio trios[] = {
        { &editor.semitoneSlider,        &editor.semitoneRndNegSlider,  &editor.semitoneRndPosSlider  },
        { &editor.fineTuneSlider,        &editor.fineTuneRndNegSlider,  &editor.fineTuneRndPosSlider  },
        { &editor.volumeSlider,          &editor.volumeRndNegSlider,    &editor.volumeRndPosSlider    },
        { &editor.panSlider,             &editor.panRndNegSlider,       &editor.panRndPosSlider       },
        // COMMENTED FOR LITE — ACTIVE IN PREMIUM
        //{ &editor.lowGainSlider,         &editor.lowGainRndNegSlider,   &editor.lowGainRndPosSlider   },
        //{ &editor.lowFreqSlider,         &editor.lowFreqRndNegSlider,   &editor.lowFreqRndPosSlider   },
        //{ &editor.midGainSlider,         &editor.midGainRndNegSlider,   &editor.midGainRndPosSlider   },
        //{ &editor.midFreqSlider,         &editor.midFreqRndNegSlider,   &editor.midFreqRndPosSlider   },
        //{ &editor.highGainSlider,        &editor.highGainRndNegSlider,  &editor.highGainRndPosSlider  },
        //{ &editor.highFreqSlider,        &editor.highFreqRndNegSlider,  &editor.highFreqRndPosSlider  },
        //{ &editor.transientAttackSlider, &editor.transAtkRndNegSlider,  &editor.transAtkRndPosSlider  },
        //{ &editor.transientDecaySlider,  &editor.transDecRndNegSlider,  &editor.transDecRndPosSlider  },
        //{ &editor.envAttackSlider,       &editor.envAtkRndNegSlider,    &editor.envAtkRndPosSlider    },
        //{ &editor.envDecaySlider,        &editor.envDecRndNegSlider,    &editor.envDecRndPosSlider    },
    };

    // FIX: use closest-dot selection instead of first-found.
    // Prevents neg dot being grabbed when clicking the pos dot at 0% (both near top).
    float        closestDist = 18.0f;   // hit radius
    juce::Slider* closestSlider = nullptr;
    bool          closestIsNeg  = false;

    for (auto& t : trios)
    {
        juce::Point<float> negPt, posPt;
        getDotPositions(*t.k, *t.n, *t.p, negPt, posPt);

        float negDist = pos.getDistanceFrom(negPt);
        float posDist = pos.getDistanceFrom(posPt);

        if (negDist < closestDist)
        {
            closestDist   = negDist;
            closestSlider = t.n;
            closestIsNeg  = true;
        }
        if (posDist < closestDist)
        {
            closestDist   = posDist;
            closestSlider = t.p;
            closestIsNeg  = false;
        }
    }

    activeSlider = closestSlider;
    activeIsNeg  = closestIsNeg;

    if (activeSlider != nullptr)
    {
        dragStartY   = e.position.y;
        dragStartX   = e.position.x;
        dragStartVal = (float)activeSlider->getValue();
    }
}

void NewProjectAudioProcessorEditor::RndArcOverlay::mouseDrag(const juce::MouseEvent& e)
{
    if (activeSlider == nullptr) return;

    float vertDelta = dragStartY - e.position.y;                  // up   = +
    float horzDelta = activeIsNeg ? (dragStartX - e.position.x)   // left = + for neg
        : (e.position.x - dragStartX);  // right = + for pos
    float delta = vertDelta + horzDelta;
    float range = (float)(activeSlider->getMaximum() - activeSlider->getMinimum());
    float newVal = dragStartVal + delta * (range / 150.0f);
    newVal = juce::jlimit((float)activeSlider->getMinimum(),
        (float)activeSlider->getMaximum(), newVal);
    activeSlider->setValue(newVal, juce::sendNotificationAsync);
    editor.repaint();
}

void NewProjectAudioProcessorEditor::RndArcOverlay::mouseUp(const juce::MouseEvent&)
{
    activeSlider = nullptr;
}
