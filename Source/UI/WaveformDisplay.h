#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>

// Shared time ruler with two independent waveform lanes.
// Audio buffers are non-owning references and must remain stable while displayed.
class WaveformDisplay final : public juce::Component
{
public:
    WaveformDisplay() = default;

    void setWaveformSources (const juce::AudioBuffer<float>* preSample,
                             double preSampleRate,
                             const juce::AudioBuffer<float>* mainSample,
                             double mainSampleRate);

    void paint (juce::Graphics& graphics) override;

private:
    void drawLane (juce::Graphics& graphics,
                   const juce::AudioBuffer<float>* source,
                   double sourceSampleRate,
                   double timelineDuration,
                   juce::Colour colour,
                   const juce::String& label,
                   const juce::Rectangle<int>& laneBounds) const;

    const juce::AudioBuffer<float>* preSampleBuffer = nullptr;
    const juce::AudioBuffer<float>* mainSampleBuffer = nullptr;
    double preSampleRate = 0.0;
    double mainSampleRate = 0.0;
};
