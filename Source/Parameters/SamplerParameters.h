#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace SamplerParameterIDs
{
    inline constexpr const char* masterGain = "masterGain";
    inline constexpr const char* mainSampleGain = "mainSampleGain";
    inline constexpr const char* preSampleGain = "preSampleGain";
    inline constexpr const char* preSampleDelay = "preSampleDelay";
}

class SamplerParameters
{
public:
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
};
