#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>

// A reusable, draggable envelope-curve editor.
// Values are normalized to the component's content rectangle (0..1).
// DSP parameter binding will be connected when the sampler voice engine lands.
class EnvelopeEditor final : public juce::Component
{
public:
    EnvelopeEditor();

    void paint (juce::Graphics& graphics) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;

    const std::vector<juce::Point<float>>& getPoints() const noexcept;

private:
    juce::Rectangle<float> getGraphBounds() const;
    juce::Point<float> toGraphPoint (juce::Point<float> normalized) const;

    std::vector<juce::Point<float>> points;
    int draggedPoint = -1;
};
