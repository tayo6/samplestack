#include "PluginEditor.h"

SampleStackAudioProcessorEditor::SampleStackAudioProcessorEditor (
    SampleStackAudioProcessor& processor)
    : juce::AudioProcessorEditor (&processor),
      audioProcessor (processor)
{
    addAndMakeVisible (mainView);

    const auto& engine = audioProcessor.getSamplerEngine();
    mainView.setWaveformSources (&engine.getPreSample().getAudioData(),
                                 &engine.getMainSample().getAudioData());

    setResizable (true, true);
    setResizeLimits (680, 380, 1920, 1200);
    setSize (840, 500);
}

void SampleStackAudioProcessorEditor::resized()
{
    mainView.setBounds (getLocalBounds());
}
