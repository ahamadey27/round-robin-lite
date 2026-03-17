#include "RRLookAndFeel.h"

//==============================================================================
// RRLookAndFeel.cpp

void RRKnobLAF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    // IMPORTANT: JUCE already subtracts the text box height before calling here.
    // 'height' is the knob-only area. Do NOT subtract tbH again — that was the bug.

    const float cx = x + width  * 0.5f;
    const float cy = y + height * 0.5f;   // now matches paintOverChildren cy exactly

    // paintOverChildren arc: knobRadius = jmin(w, h_inner)*0.5 - 4, arc at knobRadius + 5
    // Arc inner edge = (knobRadius + 5) - trackW/2 = knobRadius + 2.5
    // Body fills to just inside arc inner edge: bodyRadius = knobRadius + 1.5
    //   = jmin(width, height)*0.5 - 4 + 1.5 = jmin(width, height)*0.5 - 2.5
    const float bodyRadius  = juce::jmin((float)width, (float)height) * 0.5f - 2.5f;
    const float trackRadius = bodyRadius * 0.96f;   // grey groove slightly inside body edge

    // ── Shadow ───────────────────────────────────────────────────────────────
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.fillEllipse(cx - bodyRadius + 1.0f, cy - bodyRadius + 2.0f,
                  bodyRadius * 2.0f, bodyRadius * 2.0f);

    // ── Knob body ────────────────────────────────────────────────────────────
    g.setColour(juce::Colour(0xff1c1c1c));
    g.fillEllipse(cx - bodyRadius, cy - bodyRadius,
                  bodyRadius * 2.0f, bodyRadius * 2.0f);
    g.setColour(juce::Colour(0xff0a0a0a));
    g.drawEllipse(cx - bodyRadius, cy - bodyRadius,
                  bodyRadius * 2.0f, bodyRadius * 2.0f, 1.2f);

    // ── Track arc (grey, 275° sweep) ─────────────────────────────────────────
    juce::Path track;
    track.addArc(cx - trackRadius, cy - trackRadius,
                 trackRadius * 2.0f, trackRadius * 2.0f,
                 rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(juce::Colour(0xff2a2a2a));
    g.strokePath(track, juce::PathStrokeType(3.0f,
        juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // ── Indicator line ────────────────────────────────────────────────────────
    const juce::Colour lineCol = slider.findColour(juce::Slider::rotarySliderFillColourId);
    const float angle     = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float lineInner = bodyRadius * 0.18f;
    const float lineOuter = bodyRadius * 0.55f;
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
// RRLookAndFeel.cpp

void RRToggleLAF::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour&, bool, bool)
{
    const bool isRandom = button.getToggleState();
    const auto b = button.getLocalBounds().toFloat();

    // Fixed sizes that fit comfortably in the button bounds
    constexpr float textH = 16.0f;
    constexpr float pillH = 44.0f;
    const float totalContent = textH * 2.0f + pillH;
    const float topPad = (b.getHeight() - totalContent) * 0.5f;

    const float pillW = 18.0f;
    const float pillX = b.getCentreX() - pillW * 0.5f;
    const float pillY = b.getY() + topPad + textH;

    // SERIES label — bright when active (top = not random)
    g.setFont(juce::Font(juce::FontOptions(9.0f)).boldened());
    g.setColour(isRandom ? juce::Colour(0xff555555) : juce::Colour(0xff8aba6a));
    g.drawText("SERIES", b.getX(), b.getY() + topPad, b.getWidth(), textH,
               juce::Justification::centred);

    // RANDOM label — bright when active (bottom = random)
    g.setColour(isRandom ? juce::Colour(0xff8aba6a) : juce::Colour(0xff555555));
    g.drawText("RANDOM", b.getX(), b.getY() + topPad + textH + pillH, b.getWidth(), textH,
               juce::Justification::centred);

    // Pill track
    g.setColour(juce::Colour(0xff0e1a0e));
    g.fillRoundedRectangle(pillX, pillY, pillW, pillH, pillW * 0.5f);
    g.setColour(juce::Colour(0xff3a5a3a));
    g.drawRoundedRectangle(pillX, pillY, pillW, pillH, pillW * 0.5f, 1.0f);

    // Knob inside pill — top = SERIES, bottom = RANDOM
    const float knobD = pillW - 4.0f;
    const float knobX = pillX + 2.0f;
    const float knobY = isRandom
        ? pillY + pillH - 2.0f - knobD   // bottom
        : pillY + 2.0f;                   // top
    g.setColour(juce::Colour(0xffc87030));
    g.fillEllipse(knobX, knobY, knobD, knobD);
}

void RRToggleLAF::drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool)
{
    // Text drawn in drawButtonBackground — intentionally empty
}
