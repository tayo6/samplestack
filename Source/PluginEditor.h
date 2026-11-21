#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"

class SampleStackAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SampleStackAudioProcessorEditor (SampleStackAudioProcessor& processor);
    ~SampleStackAudioProcessorEditor() override = default;
    void paint (juce::Graphics& graphics) override;
    void resized() override;

private:
    SampleStackAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleStackAudioProcessorEditor)
};
