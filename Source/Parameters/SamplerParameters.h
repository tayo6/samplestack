#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace SamplerParameterIDs
{
    inline constexpr const char* masterGain = "masterGain";
    inline constexpr const char* mainSampleGain = "mainSampleGain";
    inline constexpr const char* preSampleGain = "preSampleGain";
    inline constexpr const char* preSampleDelay = "preSampleDelay";

    inline constexpr const char* mainStart = "mainStart";
    inline constexpr const char* mainEnd = "mainEnd";
    inline constexpr const char* mainLoopStart = "mainLoopStart";
    inline constexpr const char* mainLoopEnd = "mainLoopEnd";
    inline constexpr const char* mainLoopEnabled = "mainLoopEnabled";

    inline constexpr const char* preStart = "preStart";
    inline constexpr const char* preEnd = "preEnd";
    inline constexpr const char* preLoopStart = "preLoopStart";
    inline constexpr const char* preLoopEnd = "preLoopEnd";
    inline constexpr const char* preLoopEnabled = "preLoopEnabled";
}

class SamplerParameters
{
public:
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
};
