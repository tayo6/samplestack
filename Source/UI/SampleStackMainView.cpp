#include "SampleStackMainView.h"
#include "SampleStackTheme.h"

namespace
{
    const std::array<const char*, 5> pageNames {{
        "SAMPLE", "PITCH / OSC", "FILTER", "MOD", "MIDI"
    }};
}

SampleStackMainView::SampleStackMainView()
{
    setLookAndFeel (&lookAndFeel);

    for (size_t i = 0; i < pageButtons.size(); ++i)
    {
        pageButtons[i] = std::make_unique<juce::TextButton> (pageNames[i]);
        pageButtons[i]->setClickingTogglesState (true);
        pageButtons[i]->setColour (juce::TextButton::buttonColourId, SampleStackTheme::panel());
        pageButtons[i]->onClick = [this, i] { selectPage ((Page) i); };
        addAndMakeVisible (*pageButtons[i]);
    }
    pageButtons[0]->setToggleState (true, juce::dontSendNotification);

    expandButton.setColour (juce::TextButton::buttonColourId, SampleStackTheme::panelRaised());
    expandButton.onClick = [this] { if (onExpandRequested) onExpandRequested(); };
    addAndMakeVisible (expandButton);

    loadMainButton.onClick = [this] { if (onLoadRequested) onLoadRequested (false); };
    loadPreButton.onClick = [this] { if (onLoadRequested) onLoadRequested (true); };
    mainLoopButton.setClickingTogglesState (true);
    preLoopButton.setClickingTogglesState (true);
    mainLoopButton.onClick = [this]
    {
        if (onLoopToggled) onLoopToggled (false, mainLoopButton.getToggleState());
    };
    preLoopButton.onClick = [this]
    {
        if (onLoopToggled) onLoopToggled (true, preLoopButton.getToggleState());
    };

    addAndMakeVisible (loadMainButton);
    addAndMakeVisible (loadPreButton);
    addAndMakeVisible (mainLoopButton);
    addAndMakeVisible (preLoopButton);

    waveformDisplay.onMarkerChanged = [this] (bool isPre, WaveformDisplay::Marker marker, float value)
    {
        if (onMarkerChanged) onMarkerChanged (isPre, marker, value);
    };

    addAndMakeVisible (waveformDisplay);
    addAndMakeVisible (envelopeEditor);
}

SampleStackMainView::~SampleStackMainView()
{
    setLookAndFeel (nullptr);
}

void SampleStackMainView::setWaveformSources (const juce::AudioBuffer<float>* preSample,
                                               double preSampleRate,
                                               const juce::AudioBuffer<float>* mainSample,
                                               double mainSampleRate)
{
    waveformDisplay.setWaveformSources (preSample, preSampleRate, mainSample, mainSampleRate);
}

void SampleStackMainView::setSampleNames (const juce::String& preName, const juce::String& mainName)
{
    loadPreButton.setButtonText (preName.isEmpty() ? "LOAD PRE" : "PRE: " + preName);
    loadMainButton.setButtonText (mainName.isEmpty() ? "LOAD MAIN" : "MAIN: " + mainName);
    waveformDisplay.setSampleNames (preName, mainName);
}

void SampleStackMainView::setMarkerValues (bool isPreSample, const WaveformDisplay::MarkerValues& values)
{
    waveformDisplay.setMarkerValues (isPreSample, values);
}

void SampleStackMainView::setLoopEnabled (bool isPreSample, bool enabled)
{
    (isPreSample ? preLoopButton : mainLoopButton).setToggleState (
        enabled, juce::dontSendNotification);
    waveformDisplay.setLoopEnabled (isPreSample, enabled);
}

void SampleStackMainView::setPlaybackPositions (float prePosition, float mainPosition)
{
    waveformDisplay.setPlaybackPositions (prePosition, mainPosition);
}

void SampleStackMainView::setExpanded (bool expanded)
{
    isExpanded = expanded;
    expandButton.setButtonText (isExpanded ? "RESTORE" : "EXPAND");
}

void SampleStackMainView::selectPage (Page page)
{
    selectedPage = page;
    for (size_t i = 0; i < pageButtons.size(); ++i)
        pageButtons[i]->setToggleState ((int) i == (int) selectedPage,
                                        juce::dontSendNotification);

    const bool samplePage = selectedPage == Page::Sample;
    waveformDisplay.setVisible (samplePage);
    envelopeEditor.setVisible (samplePage);
    loadMainButton.setVisible (samplePage);
    loadPreButton.setVisible (samplePage);
    mainLoopButton.setVisible (samplePage);
    preLoopButton.setVisible (samplePage);
    resized();
    repaint();
}

void SampleStackMainView::paint (juce::Graphics& graphics)
{
    graphics.fillAll (SampleStackTheme::background());
    auto bounds = getLocalBounds();
    auto header = bounds.removeFromTop (43);

    graphics.setColour (SampleStackTheme::panel());
    graphics.fillRect (header);
    graphics.setColour (SampleStackTheme::preSample());
    graphics.fillRoundedRectangle (14.0f, 13.0f, 17.0f, 17.0f, 4.0f);
    graphics.setColour (SampleStackTheme::text());
    graphics.setFont (juce::Font (18.0f, juce::Font::bold));
    graphics.drawText ("SampleStack", 40, 0, 170, 43, juce::Justification::centredLeft);
    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (9.5f));
    graphics.drawText ("DUAL-LAYER SAMPLER",
                       header.withTrimmedLeft (205).withTrimmedRight (108),
                       juce::Justification::centredRight, false);

    auto tabBar = bounds.removeFromTop (37);
    graphics.setColour (SampleStackTheme::background().brighter (0.035f));
    graphics.fillRect (tabBar);

    if (selectedPage == Page::Sample)
        return;

    auto panel = bounds.reduced (16);
    graphics.setColour (SampleStackTheme::panel());
    graphics.fillRoundedRectangle (panel.toFloat(), 6.0f);
    graphics.setColour (SampleStackTheme::text());
    graphics.setFont (juce::Font (17.0f, juce::Font::bold));

    juce::String title, description;
    switch (selectedPage)
    {
        case Page::PitchOsc:
            title = "PITCH / OSCILLATOR";
            description = "Pitch, tuning and playback direction controls will be placed here.";
            break;
        case Page::Filter:
            title = "FILTER / GLOBAL";
            description = "Filter modes, cutoff, resonance and global voice settings will be placed here.";
            break;
        case Page::Modulation:
            title = "MODULATION";
            description = "Modulation sources and destinations will be organised in this section.";
            break;
        case Page::Midi:
            title = "MIDI";
            description = "MIDI response, key range, velocity and trigger settings will be placed here.";
            break;
        case Page::Sample:
        default:
            break;
    }
    graphics.drawText (title, panel.reduced (24).removeFromTop (35),
                       juce::Justification::centredLeft, false);
    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (12.0f));
    graphics.drawText (description, panel.reduced (24).withTrimmedTop (42),
                       juce::Justification::topLeft, true);
}

void SampleStackMainView::layoutSamplePage (juce::Rectangle<int> area)
{
    auto content = area.reduced (12, 7);
    const int envelopeHeight = juce::jlimit (112, 165, content.getHeight() * 34 / 100);
    auto waveformArea = content.removeFromTop (content.getHeight() - envelopeHeight - 9);
    waveformDisplay.setBounds (waveformArea);
    content.removeFromTop (9);
    envelopeEditor.setBounds (content);
}

void SampleStackMainView::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop (43);
    auto tabBar = area.removeFromTop (37);
    const int buttonY = tabBar.getY() + 4;
    const int buttonHeight = tabBar.getHeight() - 8;
    const int buttonWidth = juce::jlimit (72, 112, (getWidth() - 145) / 5);

    int x = 10;
    for (auto& button : pageButtons)
    {
        button->setBounds (x, buttonY, buttonWidth, buttonHeight);
        x += buttonWidth + 4;
    }
    expandButton.setBounds (getWidth() - 94, buttonY, 84, buttonHeight);

    auto toolbar = area.removeFromTop (32);
    loadMainButton.setBounds (toolbar.getX() + 9, toolbar.getY() + 3, 142, 26);
    loadPreButton.setBounds (toolbar.getX() + 157, toolbar.getY() + 3, 132, 26);
    mainLoopButton.setBounds (toolbar.getX() + 297, toolbar.getY() + 3, 92, 26);
    preLoopButton.setBounds (toolbar.getX() + 395, toolbar.getY() + 3, 88, 26);

    if (selectedPage == Page::Sample)
        layoutSamplePage (area);
}
