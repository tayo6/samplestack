#include "PluginEditor.h"

SampleStackAudioProcessorEditor::SampleStackAudioProcessorEditor (
    SampleStackAudioProcessor& processor)
    : juce::AudioProcessorEditor (&processor), audioProcessor (processor)
{
    juce::ignoreUnused (audioProcessor);
    setResizable (true, true);
    setResizeLimits (680, 380, 1920, 1200);
    setSize (840, 500);
}

void SampleStackAudioProcessorEditor::paint (juce::Graphics& g)
{
    const auto bg = juce::Colour::fromRGB (35, 37, 39);
    const auto panel = juce::Colour::fromRGB (47, 50, 53);
    const auto amber = juce::Colour::fromRGB (235, 166, 55);
    const auto text = juce::Colour::fromRGB (238, 240, 242);
    const auto muted = juce::Colour::fromRGB (164, 169, 174);

    g.fillAll (bg);
    auto area = getLocalBounds();
    auto header = area.removeFromTop (54);
    g.setColour (panel);
    g.fillRect (header);
    g.setColour (amber);
    g.fillEllipse (16.0f, 17.0f, 19.0f, 19.0f);
    g.setColour (text);
    g.setFont (juce::Font (22.0f, juce::Font::bold));
    g.drawText ("SampleStack", 48, 0, 190, 54, juce::Justification::centredLeft);
    g.setColour (muted);
    g.setFont (juce::Font (12.0f));
    g.drawText ("MILESTONE 1  /  PLUGIN FOUNDATION",
                header.withTrimmedLeft (230).withTrimmedRight (18),
                juce::Justification::centredRight, false);

    auto content = area.reduced (22);
    auto status = content.removeFromTop (116);
    g.setColour (panel);
    g.fillRoundedRectangle (status.toFloat(), 8.0f);
    g.setColour (amber);
    g.fillRoundedRectangle (juce::Rectangle<float> (
        (float) status.getX() + 18.0f, (float) status.getY() + 20.0f, 4.0f, 74.0f), 2.0f);
    g.setColour (text);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawText ("Plugin loaded successfully",
                status.withTrimmedLeft (36).withTrimmedTop (12).withHeight (38),
                juce::Justification::centredLeft, false);
    g.setColour (muted);
    g.setFont (juce::Font (13.0f));
    g.drawText ("The sampler engine and dual-waveform editor come next.",
                status.withTrimmedLeft (36).withTrimmedTop (48).withHeight (34),
                juce::Justification::centredLeft, false);
    g.setFont (juce::Font (12.0f));
    g.drawText ("JUCE 7.0.12  |  VST3 64-bit  |  Windows 7 API baseline requested",
                content.withTrimmedTop (status.getHeight() + 14),
                juce::Justification::centredLeft, false);
}

void SampleStackAudioProcessorEditor::resized()
{
    // Responsive Ableton-inspired controls will be added after the first host test.
}
