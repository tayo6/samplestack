#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "WaveformDisplay.h"

class SampleStackMainView final : public juce::Component
{
public:
    SampleStackMainView();

    void setWaveformSources (const juce::AudioBuffer<float>* preSample,
                             const juce::AudioBuffer<float>* mainSample);

    void paint (juce::Graphics& graphics) override;
    void resized() override;

private:
    WaveformDisplay waveformDisplay;
    juce::Rectangle<int> waveformBounds;
};
