#include "WaveformDisplay.h"
#include "SampleStackTheme.h"

void WaveformDisplay::setWaveformSources (const juce::AudioBuffer<float>* preSample,
                                          const juce::AudioBuffer<float>* mainSample)
{
    preSampleBuffer = preSample;
    mainSampleBuffer = mainSample;
    repaint();
}

void WaveformDisplay::paint (juce::Graphics& graphics)
{
    auto bounds = getLocalBounds();

    graphics.setColour (SampleStackTheme::panel());
    graphics.fillRoundedRectangle (bounds.toFloat(), 6.0f);

    auto waveformArea = bounds.reduced (10);
    waveformArea.removeFromTop (15);
    waveformArea.removeFromBottom (8);

    const float centreY = (float) waveformArea.getCentreY();
    graphics.setColour (SampleStackTheme::border());
    graphics.drawHorizontalLine (juce::roundToInt (centreY),
                                 (float) waveformArea.getX(),
                                 (float) waveformArea.getRight());

    // Draw the main sample first so the pre-sample remains visible above it.
    drawWaveform (graphics, mainSampleBuffer, SampleStackTheme::mainSample(), waveformArea);
    drawWaveform (graphics, preSampleBuffer, SampleStackTheme::preSample(), waveformArea);
}

void WaveformDisplay::drawWaveform (juce::Graphics& graphics,
                                    const juce::AudioBuffer<float>* source,
                                    juce::Colour colour,
                                    const juce::Rectangle<int>& area) const
{
    if (source == nullptr || source->getNumChannels() == 0
        || source->getNumSamples() < 2 || area.getWidth() < 2 || area.getHeight() < 2)
        return;

    const int sampleCount = source->getNumSamples();
    const int pixelCount = area.getWidth();
    const float centreY = (float) area.getCentreY();
    const float amplitudeScale = (float) area.getHeight() * 0.46f;

    juce::Path waveform;
    waveform.preallocateSpace (pixelCount * 3);

    for (int pixel = 0; pixel < pixelCount; ++pixel)
    {
        const int start = juce::jlimit (0, sampleCount - 1,
            (int) ((int64) pixel * sampleCount / pixelCount));
        const int end = juce::jlimit (start + 1, sampleCount,
            (int) ((int64) (pixel + 1) * sampleCount / pixelCount));

        float minimum = 0.0f;
        float maximum = 0.0f;

        for (int channel = 0; channel < source->getNumChannels(); ++channel)
        {
            const float* samples = source->getReadPointer (channel);
            for (int sample = start; sample < end; ++sample)
            {
                minimum = juce::jmin (minimum, samples[sample]);
                maximum = juce::jmax (maximum, samples[sample]);
            }
        }

        const float x = (float) (area.getX() + pixel);
        waveform.startNewSubPath (x, centreY - maximum * amplitudeScale);
        waveform.lineTo (x, centreY - minimum * amplitudeScale);
    }

    graphics.setColour (colour);
    graphics.strokePath (waveform, juce::PathStrokeType (1.25f));
}
