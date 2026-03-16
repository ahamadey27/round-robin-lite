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
        };
    addAndMakeVisible(aboutButton);

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
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &lowGainSlider,  &lowFreqSlider,
                     &midGainSlider,  &midFreqSlider,
                     &highGainSlider, &highFreqSlider,
                     &transientAttackSlider, &transientDecaySlider,
                     &envAttackSlider, &envDecaySlider })
        setupKnob(*s);

    // Rnd sliders — hidden, APVTS-bound, values read by paintOverChildren
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
                     &envDecRndNegSlider,    &envDecRndPosSlider })
        setupSlider(*s);

    // Apply knob LAF to main parameter sliders
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &lowGainSlider, &lowFreqSlider, &midGainSlider, &midFreqSlider,
                     &highGainSlider, &highFreqSlider,
                     &transientAttackSlider, &transientDecaySlider,
                     &envAttackSlider, &envDecaySlider })
        s->setLookAndFeel(&knobLAF);
    
    // NEW: set indicator line color per section (rotarySliderFillColourId)
        for (auto* s : { &semitoneSlider, &fineTuneSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::pitchCol);

        for (auto* s : { &volumeSlider, &panSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::ampCol);

        for (auto* s : { &envAttackSlider, &envDecaySlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::envCol);

        for (auto* s : { &transientAttackSlider, &transientDecaySlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::transCol);

        for (auto* s : { &lowGainSlider, &lowFreqSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::eqLowCol);

        for (auto* s : { &midGainSlider, &midFreqSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::eqMidCol);

        for (auto* s : { &highGainSlider, &highFreqSlider })
            s->setColour(juce::Slider::rotarySliderFillColourId, RRColors::eqHighCol);


    setSize(700, 520);
    // Overlay must be added AFTER all knobs so it sits on top
    addAndMakeVisible(arcOverlay);
    resized();
}

//==============================================================================
NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &lowGainSlider, &lowFreqSlider, &midGainSlider, &midFreqSlider,
                     &highGainSlider, &highFreqSlider,
                     &transientAttackSlider, &transientDecaySlider,
                     &envAttackSlider, &envDecaySlider })
        s->setLookAndFeel(nullptr);

    for (auto* s : { &semitoneRndNegSlider, &fineTuneRndNegSlider,
                     &volumeRndNegSlider,   &panRndNegSlider,
                     &lowGainRndNegSlider,  &lowFreqRndNegSlider,
                     &midGainRndNegSlider,  &midFreqRndNegSlider,
                     &highGainRndNegSlider, &highFreqRndNegSlider,
                     &transAtkRndNegSlider, &transDecRndNegSlider,
                     &envAtkRndNegSlider,   &envDecRndNegSlider,
                     &semitoneRndPosSlider, &fineTuneRndPosSlider,
                     &volumeRndPosSlider,   &panRndPosSlider,
                     &lowGainRndPosSlider,  &lowFreqRndPosSlider,
                     &midGainRndPosSlider,  &midFreqRndPosSlider,
                     &highGainRndPosSlider, &highFreqRndPosSlider,
                     &transAtkRndPosSlider, &transDecRndPosSlider,
                     &envAtkRndPosSlider,   &envDecRndPosSlider })
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
                audioProcessor.savePreset(file.withFileExtension(".rrpreset"));
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
    // ── Background ───────────────────────────────────────────────────────────
    g.fillAll(RRColors::background);

    // ── Header bar ───────────────────────────────────────────────────────────
    g.setColour(RRColors::headerBg);
    g.fillRect(0, 0, getWidth(), 52);
    g.setColour(RRColors::sectionBorder);
    g.fillRect(0, 52, getWidth(), 1);

    // Logo: "RR" bold + "Lite" light
    g.setColour(juce::Colour(0xffd0d0d0));
    g.setFont(juce::Font(juce::FontOptions(26.0f)).boldened());
    g.drawText("RR", 14, 10, 50, 32, juce::Justification::left);
    g.setColour(juce::Colour(0xff999999));
    g.setFont(juce::Font(juce::FontOptions(17.0f)));
    g.drawText("Lite", 56, 14, 50, 26, juce::Justification::left);

    // ── Section boxes ─────────────────────────────────────────────────────────
    auto drawSectionBox = [&](juce::Rectangle<int> r)
    {
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        g.fillRoundedRectangle(r.toFloat(), 5.0f);
        g.setColour(RRColors::sectionBorder);
        g.drawRoundedRectangle(r.toFloat(), 5.0f, 1.0f);
    };

    // Layout constants — must match resized()
    constexpr int sRowY  = 98;
    constexpr int sRowH  = 150;
    constexpr int eRowY  = 254;
    constexpr int eRowH  = 140;
    constexpr int margin = 12;
    constexpr int secGap = 5;
    const int secW = (getWidth() - 2 * margin - 3 * secGap) / 4;  // ~165

    // Four top sections
    for (int i = 0; i < 4; ++i)
        drawSectionBox({ margin + i * (secW + secGap), sRowY, secW, sRowH });

    // EQ section
    const int pbW   = 112;
    const int eqW   = getWidth() - 2 * margin - secGap - pbW;
    drawSectionBox({ margin, eRowY, eqW, eRowH });
    // Playback section
    drawSectionBox({ margin + eqW + secGap, eRowY, pbW, eRowH });

    // ── Section labels ────────────────────────────────────────────────────────
    auto drawSecLabel = [&](const juce::String& text, int secIdx, juce::Colour col)
    {
        g.setColour(col);
        g.setFont(juce::Font(juce::FontOptions(7.5f)).boldened());
        g.drawText(text, margin + secIdx * (secW + secGap) + 8, sRowY + 7, secW - 10, 12,
                   juce::Justification::left);
    };
    drawSecLabel("PITCH",     0, RRColors::pitchCol);
    drawSecLabel("AMPLITUDE", 1, RRColors::ampCol);
    drawSecLabel("ENVELOPE",  2, RRColors::envCol);
    drawSecLabel("TRANSIENT", 3, RRColors::transCol);

    g.setColour(RRColors::eqLowCol);
    g.setFont(juce::Font(juce::FontOptions(7.5f)).boldened());
    g.drawText("EQ SETTINGS", margin + 8, eRowY + 7, 120, 12, juce::Justification::left);

    g.setColour(RRColors::eqLowCol);
    g.drawText("PLAYBACK", margin + eqW + secGap + 8, eRowY + 7,  pbW - 10, 10,
               juce::Justification::centred);
    g.drawText("TYPE",     margin + eqW + secGap + 8, eRowY + 17, pbW - 10, 10,
               juce::Justification::centred);

    // ── Knob labels ───────────────────────────────────────────────────────────
    g.setFont(juce::Font(juce::FontOptions(7.0f)));
    g.setColour(juce::Colour(0xff666666));

    auto drawKnobLabel = [&](const juce::String& text, juce::Slider& s)
    {
        auto b = s.getBounds();
        constexpr int tbH = 16;
        g.drawText(text, b.getX(), b.getY() - 12, b.getWidth(), 11,
                   juce::Justification::centred);
    };

    drawKnobLabel("SEMITONE",  semitoneSlider);
    drawKnobLabel("FINE TUNE", fineTuneSlider);
    drawKnobLabel("VOLUME",    volumeSlider);
    drawKnobLabel("PAN",       panSlider);
    drawKnobLabel("ATTACK",    envAttackSlider);
    drawKnobLabel("DECAY",     envDecaySlider);
    drawKnobLabel("ATTACK",    transientAttackSlider);
    drawKnobLabel("DECAY",     transientDecaySlider);
    drawKnobLabel("LOW GAIN",  lowGainSlider);
    drawKnobLabel("LOW FREQ",  lowFreqSlider);
    drawKnobLabel("MID GAIN",  midGainSlider);
    drawKnobLabel("MID FREQ",  midFreqSlider);
    drawKnobLabel("HIGH GAIN", highGainSlider);
    drawKnobLabel("HIGH FREQ", highFreqSlider);

    // ── Footer ────────────────────────────────────────────────────────────────
    g.setColour(RRColors::sectionBorder.darker(0.5f));
    g.fillRect(0, getHeight() - 30, getWidth(), 1);
    g.setColour(RRColors::companyText);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("Company Name", 0, getHeight() - 26, getWidth() - 14, 20,
               juce::Justification::right);
}

//==============================================================================
// PluginEditor.cpp — paintOverChildren()

void NewProjectAudioProcessorEditor::paintOverChildren(juce::Graphics& g)
{
    constexpr float pi      = juce::MathConstants<float>::pi;
    constexpr float twoPi   = juce::MathConstants<float>::twoPi;
    constexpr float maxNeg  = pi * 0.8f;
    constexpr float maxPos  = pi * 0.8f;
    constexpr float trackW  = 5.0f;
    constexpr float arcW    = 6.5f;
    constexpr float dotR    = 5.5f;
    constexpr int   tbH     = 16;

    auto drawRndArcs = [&](juce::Slider& knob,
                           juce::Slider& negSlider,
                           juce::Slider& posSlider,
                           juce::Colour  negCol,
                           juce::Colour  posCol)
    {
        auto  b  = knob.getBounds();
        float w  = (float)b.getWidth();
        float h  = (float)(b.getHeight() - tbH);
        float cx = b.getX() + w * 0.5f;
        float cy = b.getY() + h * 0.5f;
        float radius = juce::jmin(w, h) * 0.5f - 1.0f;

        auto getNorm = [](juce::Slider& s) -> float {
            double range = s.getMaximum() - s.getMinimum();
            if (range == 0.0) return 0.0f;
            return (float)((s.getValue() - s.getMinimum()) / range);
        };

        constexpr float dotOffset = 0.15f;
        float negExtent = getNorm(negSlider) * (maxNeg - dotOffset);
        float posExtent = getNorm(posSlider) * (maxPos - dotOffset);

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

        // White dot at neg arc endpoint
        float negDotX = cx - std::sin(negExtent + dotOffset) * radius;
        float negDotY = cy - std::cos(negExtent + dotOffset) * radius;
        g.setColour(juce::Colours::white);
        g.fillEllipse(negDotX - dotR, negDotY - dotR, dotR * 2.0f, dotR * 2.0f);

        // White dot at pos arc endpoint
        float posDotX = cx + std::sin(posExtent + dotOffset) * radius;
        float posDotY = cy - std::cos(posExtent + dotOffset) * radius;
        g.setColour(juce::Colours::white);
        g.fillEllipse(posDotX - dotR, posDotY - dotR, dotR * 2.0f, dotR * 2.0f);
    };

    // PITCH
    drawRndArcs(semitoneSlider,        semitoneRndNegSlider,  semitoneRndPosSlider,  RRColors::pitchNeg,  RRColors::pitchCol);
    drawRndArcs(fineTuneSlider,        fineTuneRndNegSlider,  fineTuneRndPosSlider,  RRColors::pitchNeg,  RRColors::pitchCol);
    // AMPLITUDE
    drawRndArcs(volumeSlider,          volumeRndNegSlider,    volumeRndPosSlider,    RRColors::ampNeg,    RRColors::ampCol);
    drawRndArcs(panSlider,             panRndNegSlider,       panRndPosSlider,       RRColors::ampNeg,    RRColors::ampCol);
    // ENVELOPE
    drawRndArcs(envAttackSlider,       envAtkRndNegSlider,    envAtkRndPosSlider,    RRColors::envNeg,    RRColors::envCol);
    drawRndArcs(envDecaySlider,        envDecRndNegSlider,    envDecRndPosSlider,    RRColors::envNeg,    RRColors::envCol);
    // TRANSIENT
    drawRndArcs(transientAttackSlider, transAtkRndNegSlider,  transAtkRndPosSlider,  RRColors::transNeg,  RRColors::transCol);
    drawRndArcs(transientDecaySlider,  transDecRndNegSlider,  transDecRndPosSlider,  RRColors::transNeg,  RRColors::transCol);
    // EQ LOW
    drawRndArcs(lowGainSlider,         lowGainRndNegSlider,   lowGainRndPosSlider,   RRColors::eqLowNeg,  RRColors::eqLowCol);
    drawRndArcs(lowFreqSlider,         lowFreqRndNegSlider,   lowFreqRndPosSlider,   RRColors::eqLowNeg,  RRColors::eqLowCol);
    // EQ MID
    drawRndArcs(midGainSlider,         midGainRndNegSlider,   midGainRndPosSlider,   RRColors::eqMidNeg,  RRColors::eqMidCol);
    drawRndArcs(midFreqSlider,         midFreqRndNegSlider,   midFreqRndPosSlider,   RRColors::eqMidNeg,  RRColors::eqMidCol);
    // EQ HIGH
    drawRndArcs(highGainSlider,        highGainRndNegSlider,  highGainRndPosSlider,  RRColors::eqHighNeg, RRColors::eqHighCol);
    drawRndArcs(highFreqSlider,        highFreqRndNegSlider,  highFreqRndPosSlider,  RRColors::eqHighNeg, RRColors::eqHighCol);
}


//==============================================================================
void NewProjectAudioProcessorEditor::resized()
{
    constexpr int margin = 12;
    constexpr int secGap = 5;
    constexpr int sRowY  = 98;
    constexpr int sRowH  = 150;
    constexpr int eRowY  = 254;
    constexpr int eRowH  = 140;
    constexpr int knobW  = 68;
    constexpr int knobH  = 80;   // includes 16px text box

    const int secW = (getWidth() - 2 * margin - 3 * secGap) / 4;  // ~165

    // ── Header buttons ────────────────────────────────────────────────────────
    loadSamplesButton.setBounds(margin, 58, 120, 26);
    samplesInfoLabel.setBounds(margin + 130, 58, 260, 26);
    savePresetButton.setBounds(getWidth() - 148, 58, 60, 26);
    loadPresetButton.setBounds(getWidth() - 80,  58, 60, 26);
    aboutButton.setBounds(getWidth() - 46, 12, 22, 22);

    // ── Top section knob helper ───────────────────────────────────────────────
    // secIdx 0-3, knobIdx 0-1
    auto placeTopKnob = [&](juce::Slider& s, int secIdx, int knobIdx)
    {
        const int secX  = margin + secIdx * (secW + secGap);
        const int inner = secW - 2 * 6;               // 153px usable
        const int gap   = inner - 2 * knobW;          // split remaining
        const int kx    = secX + 6 + knobIdx * (knobW + gap);
        const int ky    = sRowY + (sRowH - knobH - 12) / 2 + 16;
        s.setBounds(kx, ky, knobW, knobH);
    };

    placeTopKnob(semitoneSlider,        0, 0);
    placeTopKnob(fineTuneSlider,        0, 1);
    placeTopKnob(volumeSlider,          1, 0);
    placeTopKnob(panSlider,             1, 1);
    placeTopKnob(envAttackSlider,       2, 0);
    placeTopKnob(envDecaySlider,        2, 1);
    placeTopKnob(transientAttackSlider, 3, 0);
    placeTopKnob(transientDecaySlider,  3, 1);

    // ── EQ knobs ─────────────────────────────────────────────────────────────
    const int pbW  = 112;
    const int eqW  = getWidth() - 2 * margin - secGap - pbW;
    const int eqInner = eqW - 12;
    const int eqGap   = (eqInner - 6 * knobW) / 5;
    const int eqKy    = eRowY + (eRowH - knobH - 12) / 2 + 20;

    auto placeEQKnob = [&](juce::Slider& s, int idx)
    {
        s.setBounds(margin + 6 + idx * (knobW + eqGap), eqKy, knobW, knobH);
    };

    placeEQKnob(lowGainSlider,  0);
    placeEQKnob(lowFreqSlider,  1);
    placeEQKnob(midGainSlider,  2);
    placeEQKnob(midFreqSlider,  3);
    placeEQKnob(highGainSlider, 4);
    placeEQKnob(highFreqSlider, 5);

    // ── Playback toggle ───────────────────────────────────────────────────────
    const int pbX = margin + eqW + secGap;
    playbackModeButton.setBounds(pbX + (pbW - 90) / 2, eRowY + 26, 90, 90);
    playbackTypeLabel.setBounds(pbX, eRowY + 7, pbW, 14);  // hidden but harmless

    // ── Arc overlay (full canvas) ─────────────────────────────────────────────
    arcOverlay.setBounds(getLocalBounds());
}

//==============================================================================
// RndArcOverlay — transparent component sitting over all knobs.
// hitTest returns true only near arc dots, so knobs still receive
// normal mouse events everywhere else.

static void getDotPositions(juce::Slider& knob,
    juce::Slider& negSlider,
    juce::Slider& posSlider,
    juce::Point<float>& negPt,
    juce::Point<float>& posPt)
{
    constexpr float pi = juce::MathConstants<float>::pi;
    constexpr float maxNeg = pi * 0.8f;
    constexpr float maxPos = pi * 0.8f;
    constexpr float dotOffset = 0.15f;
    constexpr int   tbH = 16;

    auto b = knob.getBounds();
    float w = (float)b.getWidth();
    float h = (float)(b.getHeight() - tbH);
    float cx = b.getX() + w * 0.5f;
    float cy = b.getY() + h * 0.5f;
    float radius = juce::jmin(w, h) * 0.5f - 1.0f;

    auto getNorm = [](juce::Slider& s) -> float {
        double range = s.getMaximum() - s.getMinimum();
        if (range == 0.0) return 0.0f;
        return (float)((s.getValue() - s.getMinimum()) / range);
        };

    float negExtent = getNorm(negSlider) * (maxNeg - dotOffset);
    float posExtent = getNorm(posSlider) * (maxPos - dotOffset);

    negPt = { cx - std::sin(negExtent + dotOffset) * radius,
              cy - std::cos(negExtent + dotOffset) * radius };
    posPt = { cx + std::sin(posExtent + dotOffset) * radius,
              cy - std::cos(posExtent + dotOffset) * radius };
}

bool NewProjectAudioProcessorEditor::RndArcOverlay::hitTest(int x, int y)
{
    constexpr float hitR = 14.0f;
    auto pos = juce::Point<float>((float)x, (float)y);

    struct Trio { juce::Slider* k; juce::Slider* n; juce::Slider* p; };
    Trio trios[] = {
        { &editor.semitoneSlider,        &editor.semitoneRndNegSlider,  &editor.semitoneRndPosSlider  },
        { &editor.fineTuneSlider,        &editor.fineTuneRndNegSlider,  &editor.fineTuneRndPosSlider  },
        { &editor.volumeSlider,          &editor.volumeRndNegSlider,    &editor.volumeRndPosSlider    },
        { &editor.panSlider,             &editor.panRndNegSlider,       &editor.panRndPosSlider       },
        { &editor.lowGainSlider,         &editor.lowGainRndNegSlider,   &editor.lowGainRndPosSlider   },
        { &editor.lowFreqSlider,         &editor.lowFreqRndNegSlider,   &editor.lowFreqRndPosSlider   },
        { &editor.midGainSlider,         &editor.midGainRndNegSlider,   &editor.midGainRndPosSlider   },
        { &editor.midFreqSlider,         &editor.midFreqRndNegSlider,   &editor.midFreqRndPosSlider   },
        { &editor.highGainSlider,        &editor.highGainRndNegSlider,  &editor.highGainRndPosSlider  },
        { &editor.highFreqSlider,        &editor.highFreqRndNegSlider,  &editor.highFreqRndPosSlider  },
        { &editor.transientAttackSlider, &editor.transAtkRndNegSlider,  &editor.transAtkRndPosSlider  },
        { &editor.transientDecaySlider,  &editor.transDecRndNegSlider,  &editor.transDecRndPosSlider  },
        { &editor.envAttackSlider,       &editor.envAtkRndNegSlider,    &editor.envAtkRndPosSlider    },
        { &editor.envDecaySlider,        &editor.envDecRndNegSlider,    &editor.envDecRndPosSlider    },
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
    constexpr float hitR = 14.0f;

    struct Trio { juce::Slider* k; juce::Slider* n; juce::Slider* p; };
    Trio trios[] = {
        { &editor.semitoneSlider,        &editor.semitoneRndNegSlider,  &editor.semitoneRndPosSlider  },
        { &editor.fineTuneSlider,        &editor.fineTuneRndNegSlider,  &editor.fineTuneRndPosSlider  },
        { &editor.volumeSlider,          &editor.volumeRndNegSlider,    &editor.volumeRndPosSlider    },
        { &editor.panSlider,             &editor.panRndNegSlider,       &editor.panRndPosSlider       },
        { &editor.lowGainSlider,         &editor.lowGainRndNegSlider,   &editor.lowGainRndPosSlider   },
        { &editor.lowFreqSlider,         &editor.lowFreqRndNegSlider,   &editor.lowFreqRndPosSlider   },
        { &editor.midGainSlider,         &editor.midGainRndNegSlider,   &editor.midGainRndPosSlider   },
        { &editor.midFreqSlider,         &editor.midFreqRndNegSlider,   &editor.midFreqRndPosSlider   },
        { &editor.highGainSlider,        &editor.highGainRndNegSlider,  &editor.highGainRndPosSlider  },
        { &editor.highFreqSlider,        &editor.highFreqRndNegSlider,  &editor.highFreqRndPosSlider  },
        { &editor.transientAttackSlider, &editor.transAtkRndNegSlider,  &editor.transAtkRndPosSlider  },
        { &editor.transientDecaySlider,  &editor.transDecRndNegSlider,  &editor.transDecRndPosSlider  },
        { &editor.envAttackSlider,       &editor.envAtkRndNegSlider,    &editor.envAtkRndPosSlider    },
        { &editor.envDecaySlider,        &editor.envDecRndNegSlider,    &editor.envDecRndPosSlider    },
    };

    for (auto& t : trios)
    {
        juce::Point<float> negPt, posPt;
        getDotPositions(*t.k, *t.n, *t.p, negPt, posPt);

        if (pos.getDistanceFrom(negPt) < hitR)
        {
            activeSlider = t.n; 
            activeIsNeg = true;
            break;
        }
        if (pos.getDistanceFrom(posPt) < hitR)
        {
            activeSlider = t.p; 
            activeIsNeg = false;
            break;
        }
    }

    if (activeSlider != nullptr)
    {
        dragStartY = e.position.y;
        dragStartX = e.position.x;           // ← ADD
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
