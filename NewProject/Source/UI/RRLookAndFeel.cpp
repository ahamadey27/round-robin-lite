#include "RRLookAndFeel.h"

//==============================================================================
void RRKnobLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    // tbH = text box height JUCE reserves at the bottom of the component
    constexpr int tbH = 16;
    const float w = (float)width;
    const float h = (float)(height - tbH);
    const float cx = x + w * 0.5f;
    const float cy = y + h * 0.5f;
    const float radius = juce::jmin(w, h) * 0.5f - 4.0f;

    // ── Shadow ───────────────────────────────────────────────────────────────
    g.setColour(juce::Colours::black.withAlpha(0.45f));
    g.fillEllipse(cx - radius + 1.0f, cy - radius + 2.0f, radius * 2.0f, radius * 2.0f);

    // ── Knob body ────────────────────────────────────────────────────────────
    g.setColour(RRColors::knobBody);
    g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
    g.setColour(juce::Colour(0xff0a0a0a));
    g.drawEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, 1.5f);

    // ── Track arc (grey, 275° sweep) ─────────────────────────────────────────
    juce::Path track;
    track.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f,
        rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(RRColors::knobTrack);
    g.strokePath(track, juce::PathStrokeType(3.0f,
        juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // ── Indicator line (12 o'clock to halfway down) ──────────────────────────
    // Color is set per-slider via setColour(rotarySliderFillColourId, ...)
    const juce::Colour lineCol = slider.findColour(juce::Slider::rotarySliderFillColourId);
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float lineInner = radius * 0.18f;
    const float lineOuter = radius * 0.55f;
    g.setColour(lineCol);
    g.drawLine(cx + std::sin(angle) * lineInner,
               cy - std::cos(angle) * lineInner,
               cx + std::sin(angle) * lineOuter,
               cy - std::cos(angle) * lineOuter,
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

// ── RRToggleLAF — vertical pill, SERIES top / RANDOM bottom ─────────────────
void RRToggleLAF::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour&, bool, bool)
{
    const bool isRandom = button.getToggleState();   // false=SERIES, true=RANDOM
    const auto b = button.getLocalBounds().toFloat();

    // Text areas (top/bottom thirds) and pill in middle
    const float textH = b.getHeight() * 0.28f;
    const float pillH = b.getHeight() * 0.5f;
    const float pillW = 18.0f;
    const float pillX = b.getCentreX() - pillW * 0.5f;
    const float pillY = textH + (b.getHeight() - textH * 2.0f - pillH) * 0.5f;

    // SERIES label
    g.setColour(isRandom ? juce::Colour(0xff555555) : juce::Colour(0xff8aba6a));
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.drawText("SERIES", b.withHeight(textH), juce::Justification::centred);

    // RANDOM label
    g.setColour(isRandom ? juce::Colour(0xff8aba6a) : juce::Colour(0xff555555));
    g.drawText("RANDOM", b.withY(b.getBottom() - textH).withHeight(textH),
               juce::Justification::centred);

    // Pill track
    g.setColour(juce::Colour(0xff0e1a0e));
    g.fillRoundedRectangle(pillX, pillY, pillW, pillH, pillW * 0.5f);
    g.setColour(juce::Colour(0xff3a5a3a));
    g.drawRoundedRectangle(pillX, pillY, pillW, pillH, pillW * 0.5f, 1.0f);

    // Knob inside pill
    const float knobD = pillW - 4.0f;
    const float knobX = pillX + 2.0f;
    const float knobY = isRandom
        ? pillY + pillH - 2.0f - knobD   // bottom = RANDOM
        : pillY + 2.0f;                   // top    = SERIES
    g.setColour(juce::Colour(0xffc87030));
    g.fillEllipse(knobX, knobY, knobD, knobD);
}

void RRToggleLAF::drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool)
{
    // Text drawn in drawButtonBackground — intentionally empty
}
