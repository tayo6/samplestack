#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"
#include "../UI/SampleStackMainView.h"

class SampleStackAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                               private juce::Timer
{
public:
    explicit SampleStackAudioProcessorEditor (SampleStackAudioProcessor& processor);
    ~SampleStackAudioProcessorEditor() override;
    void resized() override;

private:
    void toggleExpandedMode();
    void chooseSampleFile (bool isPreSample);
    void refreshWaveformSnapshots();
    void refreshMarkerValues();
    void timerCallback() override;

    SampleStackAudioProcessor& audioProcessor;
    SampleStackMainView mainView;
    juce::AudioBuffer<float> preWaveformSnapshot;
    juce::AudioBuffer<float> mainWaveformSnapshot;
    double preSnapshotRate = 0.0;
    double mainSnapshotRate = 0.0;
    juce::String preSnapshotName;
    juce::String mainSnapshotName;
    std::unique_ptr<juce::FileChooser> fileChooser;
    int normalWidth = 900;
    int normalHeight = 540;
    bool expandedMode = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleStackAudioProcessorEditor)
};
