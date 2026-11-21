#include "EnvelopeEditor.h"
#include "SampleStackTheme.h"

EnvelopeEditor::EnvelopeEditor()
    : points {
        { 0.00f, 1.00f },
        { 0.14f, 0.08f },
        { 0.58f, 0.30f },
        { 1.00f, 1.00f }
    }
{
    setMouseCursor (juce::MouseCursor::NormalCursor);
}

juce::Rectangle<float> EnvelopeEditor::getGraphBounds() const
{
    return getLocalBounds().toFloat().reduced (15.0f, 30.0f);
}

juce::Point<float> EnvelopeEditor::toGraphPoint (juce::Point<float> normalized) const
{
    const auto graph = getGraphBounds();
    return {
        graph.getX() + normalized.x * graph.getWidth(),
        graph.getY() + normalized.y * graph.getHeight()
    };
}

void EnvelopeEditor::paint (juce::Graphics& graphics)
{
    const auto bounds = getLocalBounds().toFloat();
    const auto graph = getGraphBounds();

    graphics.setColour (SampleStackTheme::panel());
    graphics.fillRoundedRectangle (bounds, 6.0f);

    graphics.setColour (SampleStackTheme::text());
    graphics.setFont (juce::Font (11.0f, juce::Font::bold));
    graphics.drawText ("AMP ENVELOPE", 14, 7, 150, 17,
                       juce::Justification::centredLeft);

    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (9.5f));
    graphics.drawText ("DRAG NODES TO SHAPE", 150, 7,
                       juce::jmax (80, getWidth() - 164), 17,
                       juce::Justification::centredRight);

    graphics.setColour (SampleStackTheme::border());
    for (int i = 0; i <= 4; ++i)
    {
        const float x = graph.getX() + graph.getWidth() * (float) i / 4.0f;
        const float y = graph.getY() + graph.getHeight() * (float) i / 4.0f;
        graphics.drawVerticalLine (juce::roundToInt (x), graph.getY(), graph.getBottom());
        graphics.drawHorizontalLine (juce::roundToInt (y), graph.getX(), graph.getRight());
    }

    juce::Path curve;
    if (! points.empty())
    {
        curve.startNewSubPath (toGraphPoint (points.front()));
        for (size_t i = 1; i < points.size(); ++i)
            curve.lineTo (toGraphPoint (points[i]));
    }

    graphics.setColour (SampleStackTheme::preSample().withAlpha (0.13f));
    juce::Path fill = curve;
    fill.lineTo (graph.getRight(), graph.getBottom());
    fill.lineTo (graph.getX(), graph.getBottom());
    fill.closeSubPath();
    graphics.fillPath (fill);

    graphics.setColour (SampleStackTheme::preSample());
    graphics.strokePath (curve, juce::PathStrokeType (2.0f));

    for (size_t i = 0; i < points.size(); ++i)
    {
        const auto point = toGraphPoint (points[i]);
        const float size = (int) i == draggedPoint ? 10.0f : 8.0f;

        graphics.setColour (SampleStackTheme::background());
        graphics.fillEllipse (point.x - size * 0.5f, point.y - size * 0.5f, size, size);
        graphics.setColour (SampleStackTheme::text());
        graphics.drawEllipse (point.x - size * 0.5f, point.y - size * 0.5f,
                              size, size, 1.5f);
    }
}

void EnvelopeEditor::mouseDown (const juce::MouseEvent& event)
{
    const auto mouse = event.position;
    float nearestDistance = 15.0f;
    draggedPoint = -1;

    for (size_t i = 1; i + 1 < points.size(); ++i)
    {
        const auto point = toGraphPoint (points[i]);
        const float distance = point.getDistanceFrom (mouse);

        if (distance < nearestDistance)
        {
            nearestDistance = distance;
            draggedPoint = (int) i;
        }
    }
}

void EnvelopeEditor::mouseDrag (const juce::MouseEvent& event)
{
    if (draggedPoint < 0 || draggedPoint >= (int) points.size())
        return;

    const auto graph = getGraphBounds();
    if (graph.isEmpty())
        return;

    auto point = event.position;
    point.x = juce::jlimit (graph.getX(), graph.getRight(), point.x);
    point.y = juce::jlimit (graph.getY(), graph.getBottom(), point.y);

    const float leftLimit = points[(size_t) draggedPoint - 1].x + 0.015f;
    const float rightLimit = points[(size_t) draggedPoint + 1].x - 0.015f;

    points[(size_t) draggedPoint] = {
        juce::jlimit (leftLimit, rightLimit, (point.x - graph.getX()) / graph.getWidth()),
        juce::jlimit (0.0f, 1.0f, (point.y - graph.getY()) / graph.getHeight())
    };

    repaint();
}

const std::vector<juce::Point<float>>& EnvelopeEditor::getPoints() const noexcept
{
    return points;
}
