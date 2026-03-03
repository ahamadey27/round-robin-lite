#include "RRLookAndFeel.h"

RRLookAndFeel::RRLookAndFeel()
{
    setColour(juce::Slider::textBoxTextColourId, RRColors::labelText);
    setColour(juce::Slider::textBoxBackgroundColourId, RRColors::valueBox);
    setColour(juce::Slider::textBoxOutlineColourId, RRColors::arcTrack);
    setColour(juce::Label::textColourId, RRColors::labelText);
}

void RRLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle,
    juce::Slider& slider)
{
    const float radius = (float)juce::jmin(width / 2, height / 2) - 6.0f;
    const float centreX = (float)x + (float)width * 0.5f;
    const float centreY = (float)y + (float)height * 0.5f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float thickness = 0.25f;

    // --- Background track arc ---
    {
        juce::Path track;
        track.addArc(centreX - radius, centreY - radius,
            radius * 2.0f, radius * 2.0f,
            rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(RRColors::arcTrack);
        g.strokePath(track, juce::PathStrokeType(radius * thickness));
    }

    // --- Negative arc (blue, left of center) ---
    {
        juce::Path negArc;
        const float midAngle = rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * 0.5f;
        if (angle < midAngle)
        {
            negArc.addArc(centreX - radius, centreY - radius,
                radius * 2.0f, radius * 2.0f,
                angle, midAngle, true);
            g.setColour(RRColors::arcNeg);
            g.strokePath(negArc, juce::PathStrokeType(radius * thickness));
        }
    }

    // --- Positive arc (red, right of center) ---
    {
        juce::Path posArc;
        const float midAngle = rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * 0.5f;
        if (angle > midAngle)
        {
            posArc.addArc(centreX - radius, centreY - radius,
                radius * 2.0f, radius * 2.0f,
                midAngle, angle, true);
            g.setColour(RRColors::arcPos);
            g.strokePath(posArc, juce::PathStrokeType(radius * thickness));
        }
    }

    // --- Knob body ---
    const float bodyRadius = radius * 0.65f;
    g.setColour(RRColors::knobBody);
    g.fillEllipse(centreX - bodyRadius, centreY - bodyRadius,
        bodyRadius * 2.0f, bodyRadius * 2.0f);

    // --- Pointer line ---
    juce::Path pointer;
    const float pointerLength = bodyRadius * 0.6f;
    const float pointerThickness = 2.5f;
    pointer.addRectangle(-pointerThickness * 0.5f, -bodyRadius, pointerThickness, pointerLength);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    g.setColour(RRColors::labelText);
    g.fillPath(pointer);
}

void RRLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(RRColors::labelText);
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.drawFittedText(label.getText(), label.getLocalBounds(),
        label.getJustificationType(), 1);
}