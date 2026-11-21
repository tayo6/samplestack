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

    void setWaveformSources (const juce::AudioBuffer<float>* preSample,
                             double preSampleRate,
                             const juce::AudioBuffer<float>* mainSample,
                             double mainSampleRate);

    void setExpanded (bool expanded);

    // The host editor owns window resizing; this callback asks it to toggle.
    std::function<void()> onExpandRequested;

    void paint (juce::Graphics& graphics) override;
    void resized() override;

private:
    enum class Page
    {
        Sample = 0,
        PitchOsc,
        Filter,
        Modulation,
        Midi
    };

    void selectPage (Page page);
    void layoutSamplePage (juce::Rectangle<int> area);

    SampleStackLookAndFeel lookAndFeel;
    std::array<std::unique_ptr<juce::TextButton>, 5> pageButtons;
    juce::TextButton expandButton { "EXPAND" };

    WaveformDisplay waveformDisplay;
    EnvelopeEditor envelopeEditor;

    Page selectedPage = Page::Sample;
    bool isExpanded = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleStackMainView)
};
