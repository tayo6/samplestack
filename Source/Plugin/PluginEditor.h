#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"
#include "../UI/SampleStackMainView.h"

class SampleStackAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SampleStackAudioProcessorEditor (SampleStackAudioProcessor& processor);
    ~SampleStackAudioProcessorEditor() override = default;

    void resized() override;

private:
    SampleStackAudioProcessor& audioProcessor;
    SampleStackMainView mainView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleStackAudioProcessorEditor)
};
