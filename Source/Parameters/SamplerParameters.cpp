#include "SamplerParameters.h"

juce::AudioProcessorValueTreeState::ParameterLayout SamplerParameters::createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        SamplerParameterIDs::masterGain,
        "Master Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f),
        0.8f));

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        SamplerParameterIDs::mainSampleGain,
        "Main Sample Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f),
        1.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        SamplerParameterIDs::preSampleGain,
        "Pre-sample Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f),
        1.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        SamplerParameterIDs::preSampleDelay,
        "Pre-sample Delay",
        juce::NormalisableRange<float> (0.0f, 250.0f, 0.1f),
        0.0f));

    return layout;
}
