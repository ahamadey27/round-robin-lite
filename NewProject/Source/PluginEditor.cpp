#include "PluginProcessor.h"
#include "PluginEditor.h"

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    // --- Base parameters ---
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
    // --- Randomization parameters ---
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
    // Configure base sliders
    for (auto* s : { &semitoneSlider, &fineTuneSlider, &volumeSlider, &panSlider,
                     &lowGainSlider,  &lowFreqSlider,
                     &midGainSlider,  &midFreqSlider,
                     &highGainSlider, &highFreqSlider,
                     &transientAttackSlider, &transientDecaySlider,
                     &envAttackSlider, &envDecaySlider })
    {
        setupSlider(*s);
    }

    // Configure randomization sliders
    for (auto* s : { &semitoneRndNegSlider, &semitoneRndPosSlider,
                     &fineTuneRndNegSlider, &fineTuneRndPosSlider,
                     &volumeRndNegSlider,   &volumeRndPosSlider,
                     &panRndNegSlider,      &panRndPosSlider,
                     &lowGainRndNegSlider,  &lowGainRndPosSlider,
                     &lowFreqRndNegSlider,  &lowFreqRndPosSlider,
                     &midGainRndNegSlider,  &midGainRndPosSlider,
                     &midFreqRndNegSlider,  &midFreqRndPosSlider,
                     &highGainRndNegSlider, &highGainRndPosSlider,
                     &highFreqRndNegSlider, &highFreqRndPosSlider,
                     &transAtkRndNegSlider, &transAtkRndPosSlider,
                     &transDecRndNegSlider, &transDecRndPosSlider })
    {
        setupSlider(*s);
    }

    // Total rows: 14 base + 24 rnd = 38 rows * 40px + 10px top margin
    setSize(500, 38 * 40 + 10);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor() {}

void NewProjectAudioProcessorEditor::setupSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible(slider);
}

void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);

    int y = 10;
    for (auto& label : { "Semitone",      "Fine Tune",     "Volume",        "Pan",
                         "EQ Low Gain",   "EQ Low Freq",   "EQ Mid Gain",   "EQ Mid Freq",
                         "EQ High Gain",  "EQ High Freq",
                         "Trans Attack",  "Trans Decay",
                         "Env Attack",    "Env Decay",
        // Randomization labels
        "Semitone Rnd-", "Semitone Rnd+",
        "FineTune Rnd-", "FineTune Rnd+",
        "Volume Rnd-",   "Volume Rnd+",
        "Pan Rnd-",      "Pan Rnd+",
        "LowGain Rnd-",  "LowGain Rnd+",
        "LowFreq Rnd-",  "LowFreq Rnd+",
        "MidGain Rnd-",  "MidGain Rnd+",
        "MidFreq Rnd-",  "MidFreq Rnd+",
        "HiGain Rnd-",   "HiGain Rnd+",
        "HiFreq Rnd-",   "HiFreq Rnd+",
        "TransAtk Rnd-", "TransAtk Rnd+",
        "TransDec Rnd-", "TransDec Rnd+" })
    {
        g.drawText(label, 10, y + 3, 100, 20, juce::Justification::left);
        y += 40;
    }
}

void NewProjectAudioProcessorEditor::resized()
{
    int x = 115, width = 360, height = 25, y = 10, gap = 40;

    // --- Base parameters ---
    semitoneSlider.setBounds(x, y, width, height); y += gap;
    fineTuneSlider.setBounds(x, y, width, height); y += gap;
    volumeSlider.setBounds(x, y, width, height); y += gap;
    panSlider.setBounds(x, y, width, height); y += gap;
    lowGainSlider.setBounds(x, y, width, height); y += gap;
    lowFreqSlider.setBounds(x, y, width, height); y += gap;
    midGainSlider.setBounds(x, y, width, height); y += gap;
    midFreqSlider.setBounds(x, y, width, height); y += gap;
    highGainSlider.setBounds(x, y, width, height); y += gap;
    highFreqSlider.setBounds(x, y, width, height); y += gap;
    transientAttackSlider.setBounds(x, y, width, height); y += gap;
    transientDecaySlider.setBounds(x, y, width, height); y += gap;
    envAttackSlider.setBounds(x, y, width, height); y += gap;
    envDecaySlider.setBounds(x, y, width, height); y += gap;

    // --- Randomization parameters ---
    semitoneRndNegSlider.setBounds(x, y, width, height); y += gap;
    semitoneRndPosSlider.setBounds(x, y, width, height); y += gap;
    fineTuneRndNegSlider.setBounds(x, y, width, height); y += gap;
    fineTuneRndPosSlider.setBounds(x, y, width, height); y += gap;
    volumeRndNegSlider.setBounds(x, y, width, height); y += gap;
    volumeRndPosSlider.setBounds(x, y, width, height); y += gap;
    panRndNegSlider.setBounds(x, y, width, height); y += gap;
    panRndPosSlider.setBounds(x, y, width, height); y += gap;
    lowGainRndNegSlider.setBounds(x, y, width, height); y += gap;
    lowGainRndPosSlider.setBounds(x, y, width, height); y += gap;
    lowFreqRndNegSlider.setBounds(x, y, width, height); y += gap;
    lowFreqRndPosSlider.setBounds(x, y, width, height); y += gap;
    midGainRndNegSlider.setBounds(x, y, width, height); y += gap;
    midGainRndPosSlider.setBounds(x, y, width, height); y += gap;
    midFreqRndNegSlider.setBounds(x, y, width, height); y += gap;
    midFreqRndPosSlider.setBounds(x, y, width, height); y += gap;
    highGainRndNegSlider.setBounds(x, y, width, height); y += gap;
    highGainRndPosSlider.setBounds(x, y, width, height); y += gap;
    highFreqRndNegSlider.setBounds(x, y, width, height); y += gap;
    highFreqRndPosSlider.setBounds(x, y, width, height); y += gap;
    transAtkRndNegSlider.setBounds(x, y, width, height); y += gap;
    transAtkRndPosSlider.setBounds(x, y, width, height); y += gap;
    transDecRndNegSlider.setBounds(x, y, width, height); y += gap;
    transDecRndPosSlider.setBounds(x, y, width, height);
}