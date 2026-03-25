#pragma once
#include <JuceHeader.h>

namespace RRColors
{
    // Backgrounds
    inline const juce::Colour background    { 0xff192619 };
    inline const juce::Colour headerBg      { 0xff132013 };
    inline const juce::Colour sectionBorder { 0xff2e4e2e };
    inline const juce::Colour knobBody      { 0xff1c1c1c };
    inline const juce::Colour knobTrack     { 0xff2a2a2a };
    inline const juce::Colour valueText     { 0xff888888 };
    inline const juce::Colour companyText   { 0xff3a5a3a };

    // Section label colors (also used as indicator line + pos arc)
    inline const juce::Colour pitchCol  { 0xffc87050 };
    inline const juce::Colour ampCol    { 0xff70c870 };
    inline const juce::Colour envCol    { 0xffc8a040 };
    inline const juce::Colour transCol  { 0xff8060c8 };
    inline const juce::Colour eqLowCol  { 0xff6a9a5a };
    inline const juce::Colour eqMidCol  { 0xff4a90c8 };
    inline const juce::Colour eqHighCol { 0xffc89040 };
    inline const juce::Colour toneCol   { 0xff50a0c8 };
    inline const juce::Colour trimCol   { 0xffc8a060 };

    // Rand arc neg colors (lighter version of each section)
    inline const juce::Colour pitchNeg  { 0xffe8b898 };
    inline const juce::Colour ampNeg    { 0xffb0e8b0 };
    inline const juce::Colour envNeg    { 0xffe8d090 };
    inline const juce::Colour transNeg  { 0xffc0a8e8 };
    inline const juce::Colour eqLowNeg  { 0xffa8c890 };
    inline const juce::Colour eqMidNeg  { 0xffa0cce8 };
    inline const juce::Colour eqHighNeg { 0xffe8cc90 };
    inline const juce::Colour toneNeg   { 0xff98d0e8 };
    inline const juce::Colour trimNeg   { 0xffe8d0a0 };
}

// Main parameter knobs
// Set rotarySliderFillColourId on each slider to control indicator line color
class RRKnobLAF : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPos, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;
};

// Vertical pill toggle: off=SERIES (knob top), on=RANDOM (knob bottom)
class RRToggleLAF : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics&, juce::Button&,
        const juce::Colour&, bool, bool) override;
    void drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) override;
};

class RRNegSliderLAF : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPos, float, float,
        juce::Slider::SliderStyle, juce::Slider&) override;
};

class RRPosSliderLAF : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPos, float, float,
        juce::Slider::SliderStyle, juce::Slider&) override;
};
