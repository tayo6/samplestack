#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

// Owns the decoded audio data for one layer in the sampler.
// Main and pre-sample (Snapback) layers use the same class.
class SampleLayer
{
public:
    SampleLayer() = default;

    void setAudioData (const juce::AudioBuffer<float>& source, double sourceSampleRate);
    void clear();

    bool hasAudio() const noexcept;
    double getSampleRate() const noexcept;
    const juce::AudioBuffer<float>& getAudioData() const noexcept;

private:
    juce::AudioBuffer<float> audioData;
    double sampleRate = 0.0;
};
