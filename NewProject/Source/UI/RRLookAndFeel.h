#pragma once
#include <JuceHeader.h>

// Main parameter knobs — traditional line indicator
class RRKnobLAF : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;
};

class RRToggleLAF : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics&, juce::Button&,
        const juce::Colour&, bool, bool) override;
    void drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) override;
};

// Randomization neg sliders — red ball, fills right
class RRNegSliderLAF : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle, juce::Slider&) override;
};

// Randomization pos sliders — blue ball, fills left
class RRPosSliderLAF : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle, juce::Slider&) override;
};