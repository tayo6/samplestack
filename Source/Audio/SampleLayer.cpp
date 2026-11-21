#include "SampleLayer.h"
#include <utility>

void SampleLayer::setAudioData (juce::AudioBuffer<float>&& source,
                                double sourceSampleRate,
                                const juce::String& newSourceName)
{
    audioData = std::move (source);
    sampleRate = sourceSampleRate;
    sourceName = newSourceName;
}

void SampleLayer::clear()
{
    audioData.setSize (0, 0);
    sampleRate = 0.0;
    sourceName.clear();
}

bool SampleLayer::hasAudio() const noexcept
{
    return audioData.getNumChannels() > 0 && audioData.getNumSamples() > 0;
}

double SampleLayer::getSampleRate() const noexcept { return sampleRate; }
const juce::String& SampleLayer::getSourceName() const noexcept { return sourceName; }
const juce::AudioBuffer<float>& SampleLayer::getAudioData() const noexcept { return audioData; }
