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
        pageButtons[i]->setColour (juce::TextButton::buttonColourId,
                                   SampleStackTheme::panel());
        pageButtons[i]->onClick = [this, i]
        {
            selectPage ((Page) i);
        };
        addAndMakeVisible (*pageButtons[i]);
    }

    pageButtons[0]->setToggleState (true, juce::dontSendNotification);

    expandButton.setColour (juce::TextButton::buttonColourId,
                            SampleStackTheme::panelRaised());
    expandButton.onClick = [this]
    {
        if (onExpandRequested)
            onExpandRequested();
    };
    addAndMakeVisible (expandButton);

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
    waveformDisplay.setWaveformSources (preSample, preSampleRate,
                                        mainSample, mainSampleRate);
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

    waveformDisplay.setVisible (selectedPage == Page::Sample);
    envelopeEditor.setVisible (selectedPage == Page::Sample);

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
    graphics.drawText ("SampleStack", 40, 0, 170, 43,
                       juce::Justification::centredLeft);

    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (9.5f));
    graphics.drawText ("DUAL-LAYER SAMPLER",
                       header.withTrimmedLeft (205).withTrimmedRight (108),
                       juce::Justification::centredRight, false);

    auto tabBar = bounds.removeFromTop (37);
    graphics.setColour (SampleStackTheme::background().brighter (0.035f));
    graphics.fillRect (tabBar);

    if (selectedPage != Page::Sample)
    {
        auto panel = bounds.reduced (16);
        graphics.setColour (SampleStackTheme::panel());
        graphics.fillRoundedRectangle (panel.toFloat(), 6.0f);

        graphics.setColour (SampleStackTheme::text());
        graphics.setFont (juce::Font (17.0f, juce::Font::bold));

        juce::String title;
        juce::String description;

        switch (selectedPage)
        {
            case Page::PitchOsc:
                title = "PITCH / OSCILLATOR";
                description = "Pitch, tuning, playback direction and oscillator controls will live here.";
                break;
            case Page::Filter:
                title = "FILTER / GLOBAL";
                description = "Filter modes, cutoff, resonance and global voice settings will live here.";
                break;
            case Page::Modulation:
                title = "MODULATION";
                description = "Modulation sources and destinations will be organised in this section.";
                break;
            case Page::Midi:
                title = "MIDI";
                description = "MIDI response, key range, velocity and trigger settings will live here.";
                break;
            case Page::Sample:
            default:
                break;
        }

        graphics.drawText (title, panel.reduced (24).removeFromTop (35),
                           juce::Justification::centredLeft, false);

        graphics.setColour (SampleStackTheme::mutedText());
        graphics.setFont (juce::Font (12.0f));
        graphics.drawText (description,
                           panel.reduced (24).withTrimmedTop (42),
                           juce::Justification::topLeft, true);
        return;
    }

    auto content = bounds.reduced (13, 11);
    const int envelopeHeight = juce::jlimit (118, 182, content.getHeight() * 38 / 100);
    auto waveformArea = content.removeFromTop (content.getHeight() - envelopeHeight - 10);

    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (9.5f, juce::Font::bold));
    graphics.drawText ("WAVEFORM EDITOR",
                       waveformArea.withHeight (15).withTrimmedLeft (3),
                       juce::Justification::centredLeft, false);
}

void SampleStackMainView::layoutSamplePage (juce::Rectangle<int> area)
{
    auto content = area.reduced (13, 11);
    const int envelopeHeight = juce::jlimit (118, 182, content.getHeight() * 38 / 100);
    auto waveformArea = content.removeFromTop (content.getHeight() - envelopeHeight - 10);
    waveformArea.removeFromTop (17);
    waveformDisplay.setBounds (waveformArea);

    content.removeFromTop (10);
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

    if (selectedPage == Page::Sample)
        layoutSamplePage (area.reduced (0, 0));
}
