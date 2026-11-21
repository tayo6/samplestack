#include "SamplerParameters.h"

juce::AudioProcessorValueTreeState::ParameterLayout SamplerParameters::createLayout()
{
    using FloatParameter = juce::AudioParameterFloat;
    using BoolParameter = juce::AudioParameterBool;
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::masterGain, "Master Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.8f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::mainSampleGain, "Main Sample Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f), 1.0f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::preSampleGain, "Pre-sample Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f), 1.0f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::preSampleDelay, "Pre-sample Delay",
        juce::NormalisableRange<float> (0.0f, 250.0f, 0.1f), 0.0f));

    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::mainStart, "Main Start",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::mainEnd, "Main End",
        juce::NormalisableRange<float> (0.0f, 1.0f), 1.0f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::mainLoopStart, "Main Loop Start",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.25f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::mainLoopEnd, "Main Loop End",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.75f));
    layout.add (std::make_unique<BoolParameter> (SamplerParameterIDs::mainLoopEnabled, "Main Loop", false));

    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::preStart, "Pre-sample Start",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::preEnd, "Pre-sample End",
        juce::NormalisableRange<float> (0.0f, 1.0f), 1.0f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::preLoopStart, "Pre-sample Loop Start",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.25f));
    layout.add (std::make_unique<FloatParameter> (SamplerParameterIDs::preLoopEnd, "Pre-sample Loop End",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.75f));
    layout.add (std::make_unique<BoolParameter> (SamplerParameterIDs::preLoopEnabled, "Pre-sample Loop", false));

    return layout;
}
