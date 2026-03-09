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
    playbackTypeLabel.setText("Playback Type", juce::dontSendNotification);
    playbackTypeLabel.setFont(juce::Font(juce::FontOptions(10.0f)));
    playbackTypeLabel.setColour(juce::Label::textColourId, juce::Colour(180, 180, 195));
    playbackTypeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(playbackTypeLabel);

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
    // Background
    g.fillAll(juce::Colour(28, 28, 33));

    // Layout constants — must match resized() exactly
    constexpr int margin = 20;
    constexpr int knobW = 85;
    constexpr int knobH = 100;
    constexpr int knobGap = 10;
    constexpr int sectGap = 35;
    constexpr int row1Y = 110;
    constexpr int row2Y = 250;
    constexpr int row3Y = 390;
    constexpr int eqStep = (700 - 2 * margin - 6 * knobW) / 5;

    const int pitchX = margin;
    const int ampX = pitchX + 2 * (knobW + knobGap) + sectGap;
    const int envX = ampX + 2 * (knobW + knobGap) + sectGap;
    const int transX = margin;

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

    // Header bar
    g.setColour(juce::Colour(40, 40, 48));
    g.fillRect(0, 0, getWidth(), 82);
    g.setColour(juce::Colour(100, 180, 255));
    g.setFont(juce::Font(13.0f).boldened());
    g.drawText("ROUND ROBIN LITE", getWidth() - 170, 10, 150, 20, juce::Justification::right);

    // ROW 1 — PITCH
    drawSectionLabel(pitchX, row1Y - 30, "PITCH", juce::Colour(100, 180, 255));
    drawKnobLabel(pitchX, row1Y, "Semitone");
    drawKnobLabel(pitchX + knobW + knobGap, row1Y, "Fine Tune");

    // ROW 1 — AMPLITUDE
    drawSectionLabel(ampX, row1Y - 30, "AMPLITUDE", juce::Colour(100, 210, 140));
    drawKnobLabel(ampX, row1Y, "Volume");
    drawKnobLabel(ampX + knobW + knobGap, row1Y, "Pan");

    // ROW 1 — ENVELOPE
    drawSectionLabel(envX, row1Y - 30, "ENVELOPE", juce::Colour(210, 170, 90));
    drawKnobLabel(envX, row1Y, "Atk");
    drawKnobLabel(envX + knobW + knobGap, row1Y, "Dec");

    drawDivider(row1Y + knobH + 6);

    // ROW 2 — TRANSIENT
    drawSectionLabel(transX, row2Y - 30, "TRANSIENT", juce::Colour(220, 110, 110));
    drawKnobLabel(transX, row2Y, "Attack");
    drawKnobLabel(transX + knobW + knobGap, row2Y, "Decay");

    drawDivider(row2Y + knobH + 6);

    // ROW 3 — EQ
    drawSectionLabel(margin, row3Y - 30, "3-BAND EQ", juce::Colour(170, 120, 220));
    juce::StringArray eqLabels{ "Lo Gain", "Lo Freq", "Mid Gain", "Mid Freq", "Hi Gain", "Hi Freq" };
    for (int i = 0; i < 6; ++i)
        drawKnobLabel(margin + i * (knobW + eqStep), row3Y, eqLabels[i]);

    drawDivider(row3Y + knobH + 6);
}

//==============================================================================
void NewProjectAudioProcessorEditor::paintOverChildren(juce::Graphics& g)
{
    // JUCE addArc convention: 0 = 12 o'clock, clockwise positive.
    // Point at angle A: x = cx + sin(A)*r,  y = cy - cos(A)*r
    constexpr float pi = juce::MathConstants<float>::pi;
    constexpr float twoPi = juce::MathConstants<float>::twoPi;
    constexpr float maxNeg = pi * 0.8f;                // CCW max: 12 -> 6 o'clock (left side)
    constexpr float maxPos = pi * 0.8f; // CW  max: 12 -> 4 o'clock (right side)
    constexpr float trackW = 5.0f;
    constexpr float arcW = 6.5f;
    constexpr float dotR = 5.5f;
    constexpr int   tbH = 16; // text box height

    auto drawRndArcs = [&](juce::Slider& knob,
        juce::Slider& negSlider,
        juce::Slider& posSlider)
        {
            auto  b = knob.getBounds();
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

            constexpr float dotOffset = 0.15f;

            float negExtent = getNorm(negSlider) * (maxNeg - dotOffset);
            float posExtent = getNorm(posSlider) * (maxPos - dotOffset);

            // Dim background tracks — always visible
            // Neg track: CW from 6 o'clock (pi) to 12 o'clock (twoPi) via 9 o'clock
            juce::Path negTrack;
            negTrack.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                twoPi - maxNeg, twoPi, true);
            g.setColour(juce::Colour(40, 60, 100).withAlpha(0.55f));
            g.strokePath(negTrack, juce::PathStrokeType(trackW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // Pos track: CW from 12 o'clock (0) to 4 o'clock (maxPos)
            juce::Path posTrack;
            posTrack.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                0.0f, maxPos, true);
            g.setColour(juce::Colour(100, 30, 30).withAlpha(0.55f));
            g.strokePath(posTrack, juce::PathStrokeType(trackW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // Active neg arc: CW from (twoPi - negExtent) to twoPi
            if (negExtent > 0.01f)
            {
                juce::Path negArc;
                negArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                    twoPi - negExtent, twoPi, true);
                g.setColour(juce::Colour(65, 135, 235).withAlpha(0.9f));
                g.strokePath(negArc, juce::PathStrokeType(arcW,
                    juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            }

            // Active pos arc: CW from 0 to posExtent
            if (posExtent > 0.01f)
            {
                juce::Path posArc;
                posArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                    0.0f, posExtent, true);
                g.setColour(juce::Colour(210, 65, 65).withAlpha(0.9f));
                g.strokePath(posArc, juce::PathStrokeType(arcW,
                    juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            }

            // Blue dot at neg arc endpoint — always drawn, this is the grab handle
            // Angle in JUCE convention = twoPi - negExtent
            // Point formula: x = cx + sin(A)*r,  y = cy - cos(A)*r
            float negDotX = cx - std::sin(negExtent + dotOffset) * radius;
            float negDotY = cy - std::cos(negExtent + dotOffset) * radius;
            g.setColour(juce::Colour(65, 135, 235));
            g.fillEllipse(negDotX - dotR, negDotY - dotR, dotR * 2.0f, dotR * 2.0f);
            g.setColour(juce::Colours::white.withAlpha(0.45f));
            g.drawEllipse(negDotX - dotR, negDotY - dotR, dotR * 2.0f, dotR * 2.0f, 1.0f);

            // Red dot at pos arc endpoint
            float posDotX = cx + std::sin(posExtent + dotOffset) * radius;
            float posDotY = cy - std::cos(posExtent + dotOffset) * radius;
            g.setColour(juce::Colour(210, 65, 65));
            g.fillEllipse(posDotX - dotR, posDotY - dotR, dotR * 2.0f, dotR * 2.0f);
            g.setColour(juce::Colours::white.withAlpha(0.45f));
            g.drawEllipse(posDotX - dotR, posDotY - dotR, dotR * 2.0f, dotR * 2.0f, 1.0f);
        };

    drawRndArcs(semitoneSlider, semitoneRndNegSlider, semitoneRndPosSlider);
    drawRndArcs(fineTuneSlider, fineTuneRndNegSlider, fineTuneRndPosSlider);
    drawRndArcs(volumeSlider, volumeRndNegSlider, volumeRndPosSlider);
    drawRndArcs(panSlider, panRndNegSlider, panRndPosSlider);
    drawRndArcs(lowGainSlider, lowGainRndNegSlider, lowGainRndPosSlider);
    drawRndArcs(lowFreqSlider, lowFreqRndNegSlider, lowFreqRndPosSlider);
    drawRndArcs(midGainSlider, midGainRndNegSlider, midGainRndPosSlider);
    drawRndArcs(midFreqSlider, midFreqRndNegSlider, midFreqRndPosSlider);
    drawRndArcs(highGainSlider, highGainRndNegSlider, highGainRndPosSlider);
    drawRndArcs(highFreqSlider, highFreqRndNegSlider, highFreqRndPosSlider);
    drawRndArcs(transientAttackSlider, transAtkRndNegSlider, transAtkRndPosSlider);
    drawRndArcs(transientDecaySlider, transDecRndNegSlider, transDecRndPosSlider);
    drawRndArcs(envAttackSlider, envAtkRndNegSlider, envAtkRndPosSlider);
    drawRndArcs(envDecaySlider, envDecRndNegSlider, envDecRndPosSlider);
}


//==============================================================================
void NewProjectAudioProcessorEditor::resized()
{
    constexpr int margin = 20;
    constexpr int btnH = 28;
    constexpr int knobW = 85;
    constexpr int knobH = 100;
    constexpr int knobGap = 10;
    constexpr int sectGap = 35;

    // BUTTON ROW 1: Load Samples + info label
    loadSamplesButton.setBounds(margin, 10, 140, btnH);
    samplesInfoLabel.setBounds(margin + 150, 10, 320, btnH);

    // BUTTON ROW 2: Save / Load preset / playback mode
    savePresetButton.setBounds(margin, 46, 110, btnH);
    loadPresetButton.setBounds(margin + 120, 46, 110, btnH);

    // KNOB ROW 1: PITCH | AMPLITUDE | ENVELOPE
    constexpr int row1Y = 110;

    const int pitchX = margin;
    semitoneSlider.setBounds(pitchX, row1Y, knobW, knobH);
    fineTuneSlider.setBounds(pitchX + knobW + knobGap, row1Y, knobW, knobH);

    const int ampX = pitchX + 2 * (knobW + knobGap) + sectGap;
    volumeSlider.setBounds(ampX, row1Y, knobW, knobH);
    panSlider.setBounds(ampX + knobW + knobGap, row1Y, knobW, knobH);

    const int envX = ampX + 2 * (knobW + knobGap) + sectGap;
    envAttackSlider.setBounds(envX, row1Y, knobW, knobH);
    envDecaySlider.setBounds(envX + knobW + knobGap, row1Y, knobW, knobH);

    // KNOB ROW 2: TRANSIENT
    constexpr int row2Y = 250;
    const int     transX = margin;
    transientAttackSlider.setBounds(transX, row2Y, knobW, knobH);
    transientDecaySlider.setBounds(transX + knobW + knobGap, row2Y, knobW, knobH);

    // Toggle to the right of transient knobs, vertically centred in row
    constexpr int toggleX = transX + 2 * (knobW + knobGap) + sectGap;
    playbackTypeLabel.setBounds(toggleX, row2Y + 28, 150, 14);
    playbackModeButton.setBounds(toggleX, row2Y + 46, 150, 34);

    // KNOB ROW 3: EQ
    constexpr int row3Y = 390;
    constexpr int eqStep = (700 - 2 * margin - 6 * knobW) / 5;

    lowGainSlider.setBounds(margin + 0 * (knobW + eqStep), row3Y, knobW, knobH);
    lowFreqSlider.setBounds(margin + 1 * (knobW + eqStep), row3Y, knobW, knobH);
    midGainSlider.setBounds(margin + 2 * (knobW + eqStep), row3Y, knobW, knobH);
    midFreqSlider.setBounds(margin + 3 * (knobW + eqStep), row3Y, knobW, knobH);
    highGainSlider.setBounds(margin + 4 * (knobW + eqStep), row3Y, knobW, knobH);
    highFreqSlider.setBounds(margin + 5 * (knobW + eqStep), row3Y, knobW, knobH);

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