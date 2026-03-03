#pragma once
#include <JuceHeader.h>

namespace RRColors
{
    const juce::Colour background{ 0xFF272757 };
    const juce::Colour knobBody{ 0xFF1E1E42 };
    const juce::Colour arcNeg{ 0xFF4A7FC1 };  // blue - negative range
    const juce::Colour arcPos{ 0xFFE35336 };  // red  - positive range
    const juce::Colour arcTrack{ 0xFF3D3D6B };
    const juce::Colour labelText{ 0xFFC2B280 };
    const juce::Colour sectionLine{ 0xFF98A869 };
    const juce::Colour valueBox{ 0xFF1A1A3A };
}

class RRLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RRLookAndFeel();

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;

    void drawLabel(juce::Graphics&, juce::Label&) override;
};