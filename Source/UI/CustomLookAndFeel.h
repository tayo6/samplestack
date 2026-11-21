#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// Shared visual language for SampleStack controls.
// Keep this class independent from the processor and audio engine.
class SampleStackLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    SampleStackLookAndFeel();

    void drawButtonBackground (juce::Graphics& graphics,
                               juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    void drawButtonText (juce::Graphics& graphics,
                         juce::TextButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override;

    void drawRotarySlider (juce::Graphics& graphics,
                           int x, int y, int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider& slider) override;
};
