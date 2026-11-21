#include "SamplerEngine.h"

void SamplerEngine::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    playbackSampleRate = sampleRate;
    maximumBlockSize = samplesPerBlock;
}

void SamplerEngine::releaseResources()
{
    reset();
}

void SamplerEngine::reset()
{
    // Active voices and scheduled pre-sample triggers will be reset here.
}

void SamplerEngine::processBlock (juce::AudioBuffer<float>& output,
                                  juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages, playbackSampleRate, maximumBlockSize);

    // The modular engine is intentionally silent until the voice renderer
    // is introduced in the sample-playback milestone.
    output.clear();
}

SampleLayer& SamplerEngine::getMainSample() noexcept
{
    return mainSample;
}

SampleLayer& SamplerEngine::getPreSample() noexcept
{
    return preSample;
}

const SampleLayer& SamplerEngine::getMainSample() const noexcept
{
    return mainSample;
}

const SampleLayer& SamplerEngine::getPreSample() const noexcept
{
    return preSample;
}
