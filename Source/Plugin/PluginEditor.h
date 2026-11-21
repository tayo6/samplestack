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
    void toggleExpandedMode();

    SampleStackAudioProcessor& audioProcessor;
    SampleStackMainView mainView;

    int normalWidth = 900;
    int normalHeight = 540;
    bool expandedMode = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleStackAudioProcessorEditor)
};
