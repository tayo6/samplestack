#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class SampleLayer
{
public:
    SampleLayer() = default;

    void setAudioData (juce::AudioBuffer<float>&& source,
                       double sourceSampleRate,
                       const juce::String& sourceName);
    void clear();

    bool hasAudio() const noexcept;
    double getSampleRate() const noexcept;
    const juce::String& getSourceName() const noexcept;
    const juce::AudioBuffer<float>& getAudioData() const noexcept;

private:
    juce::AudioBuffer<float> audioData;
    double sampleRate = 0.0;
    juce::String sourceName;
};
