#include "PluginEditor.h"

SampleStackAudioProcessorEditor::SampleStackAudioProcessorEditor (
    SampleStackAudioProcessor& processor)
    : juce::AudioProcessorEditor (&processor),
      audioProcessor (processor)
{
    addAndMakeVisible (mainView);

    const auto& engine = audioProcessor.getSamplerEngine();
    mainView.setWaveformSources (&engine.getPreSample().getAudioData(),
                                 engine.getPreSample().getSampleRate(),
                                 &engine.getMainSample().getAudioData(),
                                 engine.getMainSample().getSampleRate());

    mainView.onExpandRequested = [this]
    {
        toggleExpandedMode();
    };

    setResizable (true, true);
    setResizeLimits (680, 380, 2400, 1600);
    setSize (normalWidth, normalHeight);
}

void SampleStackAudioProcessorEditor::toggleExpandedMode()
{
    if (expandedMode)
    {
        setSize (normalWidth, normalHeight);
        expandedMode = false;
    }
    else
    {
        normalWidth = getWidth();
        normalHeight = getHeight();

        const auto& displays = juce::Desktop::getInstance().getDisplays();
        const auto* display = displays.getDisplayForRect (getScreenBounds());

        if (display == nullptr)
            display = displays.getPrimaryDisplay();

        if (display != nullptr)
        {
            auto target = display->userArea.reduced (24);
            setSize (target.getWidth(), target.getHeight());
            expandedMode = true;
        }
    }

    mainView.setExpanded (expandedMode);
}

void SampleStackAudioProcessorEditor::resized()
{
    mainView.setBounds (getLocalBounds());
}
