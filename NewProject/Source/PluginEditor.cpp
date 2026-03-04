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

    // Apply neg LAF
    for (auto* s : { &semitoneRndNegSlider, &fineTuneRndNegSlider,
                     &volumeRndNegSlider,   &panRndNegSlider,
                     &lowGainRndNegSlider,  &lowFreqRndNegSlider,
                     &midGainRndNegSlider,  &midFreqRndNegSlider,
                     &highGainRndNegSlider, &highFreqRndNegSlider,
                     &transAtkRndNegSlider, &transDecRndNegSlider,
                     &envAtkRndNegSlider,   &envDecRndNegSlider })
        s->setLookAndFeel(&negSliderLAF);

    // Apply pos LAF
    for (auto* s : { &semitoneRndPosSlider, &fineTuneRndPosSlider,
                     &volumeRndPosSlider,   &panRndPosSlider,
                     &lowGainRndPosSlider,  &lowFreqRndPosSlider,
                     &midGainRndPosSlider,  &midFreqRndPosSlider,
                     &highGainRndPosSlider, &highFreqRndPosSlider,
                     &transAtkRndPosSlider, &transDecRndPosSlider,
                     &envAtkRndPosSlider,   &envDecRndPosSlider })
        s->setLookAndFeel(&posSliderLAF);

    setSize(700, 520);
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
    // JUCE addArc convention: 0 = 3 o'clock, increases clockwise.
    // 12 o'clock = 3*pi/2
    constexpr float pi = juce::MathConstants<float>::pi;
    constexpr float twelve = pi * 1.5f;         // 12 o'clock (top)
    constexpr float maxNeg = pi;                 // CCW sweep: 12 -> 6 (left side)
    constexpr float maxPos = pi * 2.0f / 3.0f;  // CW  sweep: 12 -> 4 (right side)
    constexpr float trackW = 5.0f;               // dim background track thickness
    constexpr float arcW = 6.0f;               // active arc thickness
    constexpr float dotR = 5.0f;               // endpoint dot radius
    constexpr int   textBoxH = 16;

    auto drawRndArcs = [&](juce::Slider& knob,
        juce::Slider& negSlider,
        juce::Slider& posSlider)
        {
            auto  b = knob.getBounds();
            float w = (float)b.getWidth();
            float h = (float)(b.getHeight() - textBoxH);
            float cx = b.getX() + w * 0.5f;
            float cy = b.getY() + h * 0.5f;
            float radius = juce::jmin(w, h) * 0.5f - 1.0f;

            auto getNorm = [](juce::Slider& s) -> float
                {
                    double range = s.getMaximum() - s.getMinimum();
                    if (range == 0.0) return 0.0f;
                    return (float)((s.getValue() - s.getMinimum()) / range);
                };

            float negExtent = getNorm(negSlider) * maxNeg;
            float posExtent = getNorm(posSlider) * maxPos;

            //----------------------------------------------------------------------
            // Dim background tracks — always visible so user can see grab zones

            // Neg track: CW from (12 - maxNeg) to 12  →  6 o'clock through 9 to 12
            juce::Path negTrack;
            negTrack.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                twelve - maxNeg, twelve, true);
            g.setColour(juce::Colour(40, 60, 100).withAlpha(0.55f));
            g.strokePath(negTrack, juce::PathStrokeType(trackW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // Pos track: CW from 12 to (12 + maxPos)  →  12 o'clock through 2 to 4
            juce::Path posTrack;
            posTrack.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                twelve, twelve + maxPos, true);
            g.setColour(juce::Colour(100, 30, 30).withAlpha(0.55f));
            g.strokePath(posTrack, juce::PathStrokeType(trackW,
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            //----------------------------------------------------------------------
            // Active filled arcs

            if (negExtent > 0.01f)
            {
                juce::Path negArc;
                negArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                    twelve - negExtent, twelve, true);
                g.setColour(juce::Colour(65, 135, 235).withAlpha(0.9f));
                g.strokePath(negArc, juce::PathStrokeType(arcW,
                    juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            }

            if (posExtent > 0.01f)
            {
                juce::Path posArc;
                posArc.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
                    twelve, twelve + posExtent, true);
                g.setColour(juce::Colour(210, 65, 65).withAlpha(0.9f));
                g.strokePath(posArc, juce::PathStrokeType(arcW,
                    juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            }

            //----------------------------------------------------------------------
            // Endpoint dots — always drawn so user knows where to grab
            // Neg dot: sits at current neg arc end (or 12 o'clock if zero)
            float negDotAngle = twelve - negExtent;
            float posDotAngle = twelve + posExtent;

            auto drawDot = [&](float angle, juce::Colour col)
                {
                    float dx = cx + std::cos(angle) * radius;
                    float dy = cy + std::sin(angle) * radius;
                    g.setColour(col);
                    g.fillEllipse(dx - dotR, dy - dotR, dotR * 2.0f, dotR * 2.0f);
                    g.setColour(juce::Colours::white.withAlpha(0.5f));
                    g.drawEllipse(dx - dotR, dy - dotR, dotR * 2.0f, dotR * 2.0f, 1.0f);
                };

            drawDot(negDotAngle, juce::Colour(65, 135, 235));
            drawDot(posDotAngle, juce::Colour(210, 65, 65));
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
// Returns the rnd slider whose endpoint dot is nearest to pos (within hitRadius),
// or nullptr if no dot is close enough.
static juce::Slider* hitTestDot(const juce::Point<float> pos,
    juce::Slider& knob,
    juce::Slider& negSlider,
    juce::Slider& posSlider)
{
    constexpr float pi = juce::MathConstants<float>::pi;
    constexpr float twelve = pi * 1.5f;
    constexpr float maxNeg = pi;
    constexpr float maxPos = pi * 2.0f / 3.0f;
    constexpr int   textBoxH = 16;
    constexpr float hitR = 14.0f;   // generous hit radius around each dot

    auto  b = knob.getBounds();
    float w = (float)b.getWidth();
    float h = (float)(b.getHeight() - textBoxH);
    float cx = b.getX() + w * 0.5f;
    float cy = b.getY() + h * 0.5f;
    float radius = juce::jmin(w, h) * 0.5f - 1.0f;

    auto getNorm = [](juce::Slider& s) -> float
        {
            double range = s.getMaximum() - s.getMinimum();
            if (range == 0.0) return 0.0f;
            return (float)((s.getValue() - s.getMinimum()) / range);
        };

    float negAngle = twelve - getNorm(negSlider) * maxNeg;
    float posAngle = twelve + getNorm(posSlider) * maxPos;

    auto dotPos = [&](float angle) -> juce::Point<float>
        {
            return { cx + std::cos(angle) * radius,
                     cy + std::sin(angle) * radius };
        };

    if (pos.getDistanceFrom(dotPos(negAngle)) < hitR) return &negSlider;
    if (pos.getDistanceFrom(dotPos(posAngle)) < hitR) return &posSlider;
    return nullptr;
}
// Helper — finds which rnd slider the mouse is targeting on a given knob.
// Returns &negSlider if on left arc zone, &posSlider if on right, nullptr if not on arc.
static juce::Slider* hitTestArc(const juce::Point<float> pos,
    juce::Slider& knob,
    juce::Slider& negSlider,
    juce::Slider& posSlider)
{
    constexpr int   textBoxH = 16;
    constexpr float arcThick = 4.0f;

    auto  b = knob.getBounds();
    float w = (float)b.getWidth();
    float h = (float)(b.getHeight() - textBoxH);
    float cx = b.getX() + w * 0.5f;
    float cy = b.getY() + h * 0.5f;
    float radius = juce::jmin(w, h) * 0.5f - 1.0f;

    float dx = pos.x - cx;
    float dy = pos.y - cy;
    float dist = std::sqrt(dx * dx + dy * dy);

    // Only respond within the arc ring band
    if (dist < radius - arcThick || dist > radius + arcThick)
        return nullptr;

    // Angle from 12 o'clock, positive CW
    float angle = std::atan2(dx, -dy);   // atan2(x,-y) gives 0 at top

    if (angle <= 0.0f)  return &negSlider;  // left of 12 = neg zone
    else                return &posSlider;  // right of 12 = pos zone
}

void NewProjectAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    activeRndSlider = nullptr;
    auto pos = e.position;

    struct Pair { juce::Slider* knob; juce::Slider* neg; juce::Slider* pos; };
    Pair pairs[] = {
        { &semitoneSlider,        &semitoneRndNegSlider,  &semitoneRndPosSlider  },
        { &fineTuneSlider,        &fineTuneRndNegSlider,  &fineTuneRndPosSlider  },
        { &volumeSlider,          &volumeRndNegSlider,    &volumeRndPosSlider    },
        { &panSlider,             &panRndNegSlider,       &panRndPosSlider       },
        { &lowGainSlider,         &lowGainRndNegSlider,   &lowGainRndPosSlider   },
        { &lowFreqSlider,         &lowFreqRndNegSlider,   &lowFreqRndPosSlider   },
        { &midGainSlider,         &midGainRndNegSlider,   &midGainRndPosSlider   },
        { &midFreqSlider,         &midFreqRndNegSlider,   &midFreqRndPosSlider   },
        { &highGainSlider,        &highGainRndNegSlider,  &highGainRndPosSlider  },
        { &highFreqSlider,        &highFreqRndNegSlider,  &highFreqRndPosSlider  },
        { &transientAttackSlider, &transAtkRndNegSlider,  &transAtkRndPosSlider  },
        { &transientDecaySlider,  &transDecRndNegSlider,  &transDecRndPosSlider  },
        { &envAttackSlider,       &envAtkRndNegSlider,    &envAtkRndPosSlider    },
        { &envDecaySlider,        &envDecRndNegSlider,    &envDecRndPosSlider    },
    };

    for (auto& p : pairs)
    {
        auto* hit = hitTestDot(pos, *p.knob, *p.neg, *p.pos);
        if (hit != nullptr)
        {
            activeRndSlider = hit;
            arcDragStartY = pos.y;
            arcDragStartVal = (float)hit->getValue();
            break;
        }
    }
}

void NewProjectAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    if (activeRndSlider == nullptr) return;

    // 150px drag = full range — feels natural
    float delta = arcDragStartY - e.position.y;
    float range = (float)(activeRndSlider->getMaximum() - activeRndSlider->getMinimum());
    float newVal = arcDragStartVal + delta * (range / 150.0f);
    newVal = juce::jlimit((float)activeRndSlider->getMinimum(),
        (float)activeRndSlider->getMaximum(), newVal);
    activeRndSlider->setValue(newVal, juce::sendNotificationAsync);
    repaint();
}


void NewProjectAudioProcessorEditor::mouseUp(const juce::MouseEvent&)
{
    activeRndSlider = nullptr;
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
    playbackModeButton.setBounds(margin + 250, 46, 130, btnH);

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

    // KNOB ROW 3: EQ
    constexpr int row3Y = 390;
    constexpr int eqStep = (700 - 2 * margin - 6 * knobW) / 5;

    lowGainSlider.setBounds(margin + 0 * (knobW + eqStep), row3Y, knobW, knobH);
    lowFreqSlider.setBounds(margin + 1 * (knobW + eqStep), row3Y, knobW, knobH);
    midGainSlider.setBounds(margin + 2 * (knobW + eqStep), row3Y, knobW, knobH);
    midFreqSlider.setBounds(margin + 3 * (knobW + eqStep), row3Y, knobW, knobH);
    highGainSlider.setBounds(margin + 4 * (knobW + eqStep), row3Y, knobW, knobH);
    highFreqSlider.setBounds(margin + 5 * (knobW + eqStep), row3Y, knobW, knobH);
}