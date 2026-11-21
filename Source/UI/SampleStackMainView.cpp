#include "SampleStackMainView.h"
#include "SampleStackTheme.h"

SampleStackMainView::SampleStackMainView()
{
    addAndMakeVisible (waveformDisplay);
}

void SampleStackMainView::setWaveformSources (const juce::AudioBuffer<float>* preSample,
                                              const juce::AudioBuffer<float>* mainSample)
{
    waveformDisplay.setWaveformSources (preSample, mainSample);
}

void SampleStackMainView::paint (juce::Graphics& graphics)
{
    graphics.fillAll (SampleStackTheme::background());

    auto area = getLocalBounds();
    auto header = area.removeFromTop (48);

    graphics.setColour (SampleStackTheme::panel());
    graphics.fillRect (header);

    graphics.setColour (SampleStackTheme::preSample());
    graphics.fillRoundedRectangle (16.0f, 15.0f, 18.0f, 18.0f, 4.0f);

    graphics.setColour (SampleStackTheme::text());
    graphics.setFont (juce::Font (21.0f, juce::Font::bold));
    graphics.drawText ("SampleStack", 46, 0, 190, 48,
                       juce::Justification::centredLeft);

    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (11.5f));
    graphics.drawText ("SAMPLE     PITCH / OSC     FILTER / GLOBAL     MOD     MIDI",
                       header.withTrimmedLeft (238).withTrimmedRight (14),
                       juce::Justification::centredRight, false);

    auto tabBar = area.removeFromTop (35);
    graphics.setColour (SampleStackTheme::panelRaised());
    graphics.fillRect (tabBar);
    graphics.setColour (SampleStackTheme::text());
    graphics.setFont (juce::Font (12.0f, juce::Font::bold));
    graphics.drawText ("SAMPLE", tabBar.withTrimmedLeft (16),
                       juce::Justification::centredLeft, false);

    auto legend = area.removeFromBottom (28).reduced (14, 0);
    graphics.setFont (juce::Font (11.0f));

    graphics.setColour (SampleStackTheme::preSample());
    graphics.fillEllipse ((float) legend.getX(), (float) legend.getCentreY() - 4.0f, 8.0f, 8.0f);
    graphics.setColour (SampleStackTheme::text());
    graphics.drawText ("PRE-SAMPLE / SNAPBACK", legend.withTrimmedLeft (14).withWidth (170),
                       juce::Justification::centredLeft, false);

    auto secondLegend = legend.withTrimmedLeft (196);
    graphics.setColour (SampleStackTheme::mainSample());
    graphics.fillEllipse ((float) secondLegend.getX(), (float) secondLegend.getCentreY() - 4.0f, 8.0f, 8.0f);
    graphics.setColour (SampleStackTheme::text());
    graphics.drawText ("MAIN SAMPLE", secondLegend.withTrimmedLeft (14),
                       juce::Justification::centredLeft, false);
}

void SampleStackMainView::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop (48);
    area.removeFromTop (35);
    area.removeFromBottom (28);
    waveformBounds = area.reduced (12, 8);
    waveformDisplay.setBounds (waveformBounds);
}
