#include "SampleLayer.h"

void SampleLayer::setAudioData (const juce::AudioBuffer<float>& source,
                                double sourceSampleRate)
{
    audioData.setSize (source.getNumChannels(), source.getNumSamples(), false, false, true);

    for (int channel = 0; channel < source.getNumChannels(); ++channel)
        audioData.copyFrom (channel, 0, source, channel, 0, source.getNumSamples());

    sampleRate = sourceSampleRate;
}

void SampleLayer::clear()
{
    audioData.setSize (0, 0);
    sampleRate = 0.0;
}

bool SampleLayer::hasAudio() const noexcept
{
    return audioData.getNumChannels() > 0 && audioData.getNumSamples() > 0;
}

double SampleLayer::getSampleRate() const noexcept
{
    return sampleRate;
}

const juce::AudioBuffer<float>& SampleLayer::getAudioData() const noexcept
{
    return audioData;
}
