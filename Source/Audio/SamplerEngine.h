#pragma once

#include "SampleLayer.h"

// Audio-domain coordinator. Playback voices, trigger scheduling, pitch and
// routing will live here rather than in the JUCE plug-in wrapper.
class SamplerEngine
{
public:
    SamplerEngine() = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void reset();

    void processBlock (juce::AudioBuffer<float>& output,
                       juce::MidiBuffer& midiMessages);

    SampleLayer& getMainSample() noexcept;
    SampleLayer& getPreSample() noexcept;
    const SampleLayer& getMainSample() const noexcept;
    const SampleLayer& getPreSample() const noexcept;

private:
    SampleLayer mainSample;
    SampleLayer preSample;

    double playbackSampleRate = 44100.0;
    int maximumBlockSize = 0;
};
