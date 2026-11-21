#pragma once

#include <array>
#include <functional>
#include <memory>
#include <juce_gui_basics/juce_gui_basics.h>
#include "CustomLookAndFeel.h"
#include "EnvelopeEditor.h"
#include "WaveformDisplay.h"

class SampleStackMainView final : public juce::Component
{
public:
    SampleStackMainView();
    ~SampleStackMainView() override;

    std::function<void()> onExpandRequested;
    std::function<void (bool isPreSample)> onLoadRequested;
    std::function<void (bool isPreSample, WaveformDisplay::Marker marker, float value)> onMarkerChanged;
    std::function<void (bool isPreSample, bool enabled)> onLoopToggled;

    void setWaveformSources (const juce::AudioBuffer<float>* preSample, double preSampleRate,
                             const juce::AudioBuffer<float>* mainSample, double mainSampleRate);
    void setSampleNames (const juce::String& preSampleName, const juce::String& mainSampleName);
    void setMarkerValues (bool isPreSample, const WaveformDisplay::MarkerValues& values);
    void setLoopEnabled (bool isPreSample, bool enabled);
    void setPlaybackPositions (float preSamplePosition, float mainSamplePosition);
    void setExpanded (bool expanded);

    void paint (juce::Graphics& graphics) override;
    void resized() override;

private:
    enum class Page { Sample = 0, PitchOsc, Filter, Modulation, Midi };
    void selectPage (Page page);
    void layoutSamplePage (juce::Rectangle<int> area);

    SampleStackLookAndFeel lookAndFeel;
    std::array<std::unique_ptr<juce::TextButton>, 5> pageButtons;
    juce::TextButton expandButton { "EXPAND" };
    juce::TextButton loadMainButton { "LOAD MAIN" };
    juce::TextButton loadPreButton { "LOAD PRE" };
    juce::TextButton mainLoopButton { "MAIN LOOP" };
    juce::TextButton preLoopButton { "PRE LOOP" };
    WaveformDisplay waveformDisplay;
    EnvelopeEditor envelopeEditor;
    Page selectedPage = Page::Sample;
    bool isExpanded = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleStackMainView)
};
