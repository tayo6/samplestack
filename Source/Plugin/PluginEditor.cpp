#include "PluginEditor.h"
#include "../Parameters/SamplerParameters.h"

SampleStackAudioProcessorEditor::SampleStackAudioProcessorEditor (
    SampleStackAudioProcessor& processor)
    : juce::AudioProcessorEditor (&processor), audioProcessor (processor)
{
    addAndMakeVisible (mainView);

    mainView.onExpandRequested = [this] { toggleExpandedMode(); };
    mainView.onLoadRequested = [this] (bool isPreSample) { chooseSampleFile (isPreSample); };

    mainView.onMarkerChanged = [this] (bool isPreSample, WaveformDisplay::Marker marker, float value)
    {
        const char* id = nullptr;
        if (isPreSample)
        {
            switch (marker)
            {
                case WaveformDisplay::Marker::Start:     id = SamplerParameterIDs::preStart; break;
                case WaveformDisplay::Marker::End:       id = SamplerParameterIDs::preEnd; break;
                case WaveformDisplay::Marker::LoopStart: id = SamplerParameterIDs::preLoopStart; break;
                case WaveformDisplay::Marker::LoopEnd:   id = SamplerParameterIDs::preLoopEnd; break;
            }
        }
        else
        {
            switch (marker)
            {
                case WaveformDisplay::Marker::Start:     id = SamplerParameterIDs::mainStart; break;
                case WaveformDisplay::Marker::End:       id = SamplerParameterIDs::mainEnd; break;
                case WaveformDisplay::Marker::LoopStart: id = SamplerParameterIDs::mainLoopStart; break;
                case WaveformDisplay::Marker::LoopEnd:   id = SamplerParameterIDs::mainLoopEnd; break;
            }
        }
        if (id != nullptr)
            audioProcessor.setParameterNormalized (id, value);
    };

    mainView.onLoopToggled = [this] (bool isPreSample, bool enabled)
    {
        audioProcessor.setParameterNormalized (
            isPreSample ? SamplerParameterIDs::preLoopEnabled
                        : SamplerParameterIDs::mainLoopEnabled,
            enabled ? 1.0f : 0.0f);
    };

    setResizable (true, true);
    setResizeLimits (680, 380, 2400, 1600);
    setSize (normalWidth, normalHeight);
    refreshWaveformSnapshots();
    refreshMarkerValues();
    startTimerHz (30);
}

SampleStackAudioProcessorEditor::~SampleStackAudioProcessorEditor()
{
    stopTimer();
    fileChooser.reset();
}

void SampleStackAudioProcessorEditor::chooseSampleFile (bool isPreSample)
{
    const auto title = isPreSample ? "Load pre-sample / Snapback" : "Load main sample";
    fileChooser = std::make_unique<juce::FileChooser> (
        title, juce::File(), "*.wav;*.aif;*.aiff;*.flac;*.ogg;*.mp3");

    juce::Component::SafePointer<SampleStackAudioProcessorEditor> safeThis (this);
    fileChooser->launchAsync (
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [safeThis, isPreSample] (const juce::FileChooser& chooser)
        {
            if (safeThis == nullptr)
                return;

            const auto file = chooser.getResult();
            if (! file.existsAsFile())
                return;

            juce::String error;
            if (! safeThis->audioProcessor.loadSampleFile (isPreSample, file, error))
            {
                juce::AlertWindow::showMessageBoxAsync (
                    juce::AlertWindow::WarningIcon, "SampleStack sample load failed", error);
                return;
            }

            safeThis->refreshWaveformSnapshots();
        });
}

void SampleStackAudioProcessorEditor::refreshWaveformSnapshots()
{
    audioProcessor.copySampleForDisplay (true, preWaveformSnapshot,
                                         preSnapshotRate, preSnapshotName);
    audioProcessor.copySampleForDisplay (false, mainWaveformSnapshot,
                                         mainSnapshotRate, mainSnapshotName);

    mainView.setWaveformSources (&preWaveformSnapshot, preSnapshotRate,
                                 &mainWaveformSnapshot, mainSnapshotRate);
    mainView.setSampleNames (preSnapshotName, mainSnapshotName);
}

void SampleStackAudioProcessorEditor::refreshMarkerValues()
{
    const auto read = [this] (const char* id)
    {
        return juce::jlimit (0.0f, 1.0f, audioProcessor.getParameterValue (id));
    };

    WaveformDisplay::MarkerValues mainMarkers;
    mainMarkers.start = read (SamplerParameterIDs::mainStart);
    mainMarkers.end = read (SamplerParameterIDs::mainEnd);
    mainMarkers.loopStart = read (SamplerParameterIDs::mainLoopStart);
    mainMarkers.loopEnd = read (SamplerParameterIDs::mainLoopEnd);
    mainMarkers.loopEnabled = audioProcessor.getParameterValue (
        SamplerParameterIDs::mainLoopEnabled) > 0.5f;

    WaveformDisplay::MarkerValues preMarkers;
    preMarkers.start = read (SamplerParameterIDs::preStart);
    preMarkers.end = read (SamplerParameterIDs::preEnd);
    preMarkers.loopStart = read (SamplerParameterIDs::preLoopStart);
    preMarkers.loopEnd = read (SamplerParameterIDs::preLoopEnd);
    preMarkers.loopEnabled = audioProcessor.getParameterValue (
        SamplerParameterIDs::preLoopEnabled) > 0.5f;

    mainView.setMarkerValues (false, mainMarkers);
    mainView.setMarkerValues (true, preMarkers);
    mainView.setLoopEnabled (false, mainMarkers.loopEnabled);
    mainView.setLoopEnabled (true, preMarkers.loopEnabled);
}

void SampleStackAudioProcessorEditor::timerCallback()
{
    mainView.setPlaybackPositions (audioProcessor.getPrePlaybackPosition(),
                                   audioProcessor.getMainPlaybackPosition());
    refreshMarkerValues();
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
            const auto target = display->userArea.reduced (24);
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
