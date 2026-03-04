#include "RRLookAndFeel.h"

//==============================================================================
void RRKnobLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&)
{
    const float radius = juce::jmin(width, height) * 0.5f - 4.0f;
    const float cx = x + width * 0.5f;
    const float cy = y + height * 0.5f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Full arc track (dark)
    juce::Path track;
    track.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
        rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(juce::Colour(55, 55, 65));
    g.strokePath(track, juce::PathStrokeType(3.5f,
        juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Knob body
    const float bodyR = radius - 6.0f;
    g.setColour(juce::Colour(52, 52, 62));
    g.fillEllipse(cx - bodyR, cy - bodyR, bodyR * 2.0f, bodyR * 2.0f);
    g.setColour(juce::Colour(78, 78, 92));
    g.drawEllipse(cx - bodyR, cy - bodyR, bodyR * 2.0f, bodyR * 2.0f, 1.0f);

    // Indicator line (center → edge)
    const float innerR = bodyR * 0.2f;
    const float outerR = bodyR * 0.82f;
    g.setColour(juce::Colours::white);
    g.drawLine(cx + std::sin(angle) * innerR, cy - std::cos(angle) * innerR,
        cx + std::sin(angle) * outerR, cy - std::cos(angle) * outerR,
        2.2f);
}

//==============================================================================
static void drawRndSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, bool isNeg)
{
    const float trackY = y + height * 0.5f;
    const float trackH = 3.5f;
    const float thumbR = 6.0f;

    // Track background
    g.setColour(juce::Colour(38, 38, 48));
    g.fillRoundedRectangle((float)x, trackY - trackH * 0.5f, (float)width, trackH, 2.0f);

    // Fill
    if (isNeg)
    {
        // Neg: fill from thumb rightward (shows how much negative range is set)
        g.setColour(juce::Colour(140, 35, 35));
        g.fillRoundedRectangle(sliderPos, trackY - trackH * 0.5f,
            (float)(x + width) - sliderPos, trackH, 2.0f);
    }
    else
    {
        // Pos: fill from left to thumb
        g.setColour(juce::Colour(35, 75, 165));
        g.fillRoundedRectangle((float)x, trackY - trackH * 0.5f,
            sliderPos - (float)x, trackH, 2.0f);
    }

    // Thumb ball
    const juce::Colour thumbCol = isNeg ? juce::Colour(210, 65, 65)
        : juce::Colour(65, 135, 235);
    g.setColour(thumbCol);
    g.fillEllipse(sliderPos - thumbR, trackY - thumbR, thumbR * 2.0f, thumbR * 2.0f);
    g.setColour(juce::Colours::white.withAlpha(0.35f));
    g.drawEllipse(sliderPos - thumbR, trackY - thumbR, thumbR * 2.0f, thumbR * 2.0f, 1.0f);
}

void RRNegSliderLAF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float, float, juce::Slider::SliderStyle, juce::Slider&)
{
    drawRndSlider(g, x, y, width, height, sliderPos, true);
}

void RRPosSliderLAF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float, float, juce::Slider::SliderStyle, juce::Slider&)
{
    drawRndSlider(g, x, y, width, height, sliderPos, false);
}

void RRToggleLAF::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour&, bool, bool)
{
    auto b = button.getLocalBounds().toFloat().reduced(1.0f);
    bool isOn = button.getToggleState();
    float r = b.getHeight() * 0.5f;

    // Pill background
    g.setColour(juce::Colour(38, 38, 50));
    g.fillRoundedRectangle(b, r);

    // Sliding thumb
    float halfW = b.getWidth() * 0.5f;
    float thumbX = isOn ? b.getX() + halfW : b.getX();
    g.setColour(isOn ? juce::Colour(180, 60, 60) : juce::Colour(55, 110, 200));
    g.fillRoundedRectangle(thumbX, b.getY(), halfW, b.getHeight(), r);

    // "Series" text (left)
    g.setColour(!isOn ? juce::Colours::white : juce::Colour(120, 120, 130));
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("Series", (int)b.getX(), (int)b.getY(),
        (int)halfW, (int)b.getHeight(), juce::Justification::centred);

    // "Random" text (right)
    g.setColour(isOn ? juce::Colours::white : juce::Colour(120, 120, 130));
    g.drawText("Random", (int)(b.getX() + halfW), (int)b.getY(),
        (int)halfW, (int)b.getHeight(), juce::Justification::centred);

    // Border
    g.setColour(juce::Colour(75, 75, 90));
    g.drawRoundedRectangle(b, r, 1.0f);
}

void RRToggleLAF::drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool)
{
    // Text drawn in drawButtonBackground — intentionally empty
}