#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>

// Draws both sample layers in one shared time display.
// The buffers are non-owning references; their owning SampleLayer objects
// must outlive this component. Replace/update them from the message thread.
class WaveformDisplay final : public juce::Component
{
public:
    WaveformDisplay() = default;

    void setWaveformSources (const juce::AudioBuffer<float>* preSample,
                             const juce::AudioBuffer<float>* mainSample);

    void paint (juce::Graphics& graphics) override;

private:
    void drawWaveform (juce::Graphics& graphics,
                       const juce::AudioBuffer<float>* source,
                       juce::Colour colour,
                       const juce::Rectangle<int>& area) const;

    const juce::AudioBuffer<float>* preSampleBuffer = nullptr;
    const juce::AudioBuffer<float>* mainSampleBuffer = nullptr;
};
