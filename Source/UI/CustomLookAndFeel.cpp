#include "CustomLookAndFeel.h"
#include "SampleStackTheme.h"

SampleStackLookAndFeel::SampleStackLookAndFeel()
{
    setColour (juce::TextButton::buttonColourId, SampleStackTheme::panelRaised());
    setColour (juce::TextButton::textColourOffId, SampleStackTheme::mutedText());
    setColour (juce::TextButton::textColourOnId, SampleStackTheme::text());
    setColour (juce::Slider::textBoxTextColourId, SampleStackTheme::text());
    setColour (juce::Slider::textBoxBackgroundColourId, SampleStackTheme::panel());
}

void SampleStackLookAndFeel::drawButtonBackground (juce::Graphics& graphics,
                                                   juce::Button& button,
                                                   const juce::Colour& backgroundColour,
                                                   bool isHighlighted,
                                                   bool isDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (1.0f);
    auto fill = backgroundColour;

    if (button.getToggleState())
        fill = SampleStackTheme::panelRaised().brighter (0.12f);
    else if (isDown)
        fill = fill.darker (0.18f);
    else if (isHighlighted)
        fill = fill.brighter (0.10f);

    graphics.setColour (fill);
    graphics.fillRoundedRectangle (bounds, 4.0f);

    if (button.getToggleState())
    {
        graphics.setColour (SampleStackTheme::preSample());
        graphics.fillRoundedRectangle (
            juce::Rectangle<float> (bounds.getX() + 5.0f, bounds.getBottom() - 3.0f,
                                    bounds.getWidth() - 10.0f, 2.0f),
            1.0f);
    }
}

void SampleStackLookAndFeel::drawButtonText (juce::Graphics& graphics,
                                             juce::TextButton& button,
                                             bool isHighlighted,
                                             bool isDown)
{
    juce::ignoreUnused (isHighlighted, isDown);

    auto colour = button.getToggleState()
        ? SampleStackTheme::text()
        : SampleStackTheme::mutedText();

    graphics.setColour (colour);
    graphics.setFont (juce::Font (11.5f, juce::Font::bold));
    graphics.drawFittedText (button.getButtonText(),
                             button.getLocalBounds().reduced (5, 1),
                             juce::Justification::centred, 1);
}

void SampleStackLookAndFeel::drawRotarySlider (juce::Graphics& graphics,
                                               int x, int y, int width, int height,
                                               float sliderPosProportional,
                                               float rotaryStartAngle,
                                               float rotaryEndAngle,
                                               juce::Slider& slider)
{
    juce::ignoreUnused (slider);

    const float diameter = (float) juce::jmin (width, height) * 0.72f;
    const float centreX = (float) x + (float) width * 0.5f;
    const float centreY = (float) y + (float) height * 0.44f;
    const float radius = diameter * 0.5f;
    const float angle = rotaryStartAngle
        + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    graphics.setColour (SampleStackTheme::panelRaised());
    graphics.fillEllipse (centreX - radius, centreY - radius, diameter, diameter);

    graphics.setColour (SampleStackTheme::border());
    graphics.drawEllipse (centreX - radius, centreY - radius, diameter, diameter, 1.0f);

    juce::Path track;
    track.addCentredArc (centreX, centreY, radius * 0.78f, radius * 0.78f,
                         0.0f, rotaryStartAngle, rotaryEndAngle, true);
    graphics.setColour (SampleStackTheme::border());
    graphics.strokePath (track, juce::PathStrokeType (3.0f));

    juce::Path valueArc;
    valueArc.addCentredArc (centreX, centreY, radius * 0.78f, radius * 0.78f,
                            0.0f, rotaryStartAngle, angle, true);
    graphics.setColour (SampleStackTheme::preSample());
    graphics.strokePath (valueArc, juce::PathStrokeType (3.0f));

    juce::Path pointer;
    pointer.addRoundedRectangle (-1.5f, -radius * 0.62f, 3.0f, radius * 0.28f, 1.2f);
    graphics.setColour (SampleStackTheme::text());
    graphics.fillPath (pointer, juce::AffineTransform::rotation (angle).translated (centreX, centreY));
}
