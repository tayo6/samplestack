#include "WaveformDisplay.h"
#include "SampleStackTheme.h"

void WaveformDisplay::setWaveformSources (const juce::AudioBuffer<float>* preSample,
                                          double newPreSampleRate,
                                          const juce::AudioBuffer<float>* mainSample,
                                          double newMainSampleRate)
{
    preSampleBuffer = preSample;
    mainSampleBuffer = mainSample;
    preSampleRate = newPreSampleRate;
    mainSampleRate = newMainSampleRate;
    repaint();
}

void WaveformDisplay::paint (juce::Graphics& graphics)
{
    const auto bounds = getLocalBounds();
    graphics.setColour (SampleStackTheme::panel());
    graphics.fillRoundedRectangle (bounds.toFloat(), 6.0f);

    auto content = bounds.reduced (12, 9);
    auto heading = content.removeFromTop (19);

    graphics.setColour (SampleStackTheme::text());
    graphics.setFont (juce::Font (10.5f, juce::Font::bold));
    graphics.drawText ("SAMPLE WAVEFORM", heading,
                       juce::Justification::centredLeft, false);

    auto ruler = content.removeFromTop (23);
    const int labelWidth = 116;
    auto timeline = ruler.withTrimmedLeft (labelWidth + 8);

    double duration = 0.0;
    if (preSampleBuffer != nullptr && preSampleRate > 0.0)
        duration = juce::jmax (duration, (double) preSampleBuffer->getNumSamples() / preSampleRate);
    if (mainSampleBuffer != nullptr && mainSampleRate > 0.0)
        duration = juce::jmax (duration, (double) mainSampleBuffer->getNumSamples() / mainSampleRate);

    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (9.5f));

    for (int tick = 0; tick <= 8; ++tick)
    {
        const float x = (float) timeline.getX()
            + timeline.getWidth() * (float) tick / 8.0f;

        const auto label = duration > 0.0
            ? juce::String (duration * (double) tick / 8.0, 2) + " s"
            : juce::String (tick * 12.5, 1) + "%";

        graphics.drawText (label,
                           juce::Rectangle<int> (juce::roundToInt (x - 22.0f),
                                                 ruler.getY(), 44, ruler.getHeight()),
                           juce::Justification::centred, false);
    }

    auto preLane = content.removeFromTop (content.getHeight() / 2 - 4);
    content.removeFromTop (8);
    auto mainLane = content;

    drawLane (graphics, preSampleBuffer, preSampleRate, duration,
              SampleStackTheme::preSample(), "PRE-SAMPLE", preLane);
    drawLane (graphics, mainSampleBuffer, mainSampleRate, duration,
              SampleStackTheme::mainSample(), "MAIN SAMPLE", mainLane);
}

void WaveformDisplay::drawLane (juce::Graphics& graphics,
                                const juce::AudioBuffer<float>* source,
                                double sourceSampleRate,
                                double timelineDuration,
                                juce::Colour colour,
                                const juce::String& label,
                                const juce::Rectangle<int>& laneBounds) const
{
    if (laneBounds.isEmpty())
        return;

    graphics.setColour (SampleStackTheme::background().brighter (0.025f));
    graphics.fillRoundedRectangle (laneBounds.toFloat(), 3.0f);

    auto labelArea = laneBounds.withWidth (116).reduced (8, 0);
    auto waveArea = laneBounds.withTrimmedLeft (124).reduced (0, 7);

    graphics.setColour (colour);
    graphics.setFont (juce::Font (9.5f, juce::Font::bold));
    graphics.drawText (label, labelArea,
                       juce::Justification::centredLeft, false);

    graphics.setColour (SampleStackTheme::border().withAlpha (0.75f));
    graphics.drawHorizontalLine (waveArea.getCentreY(),
                                 (float) waveArea.getX(),
                                 (float) waveArea.getRight());

    for (int tick = 0; tick <= 8; ++tick)
    {
        const float x = (float) waveArea.getX()
            + waveArea.getWidth() * (float) tick / 8.0f;
        graphics.setColour (SampleStackTheme::border().withAlpha (0.45f));
        graphics.drawVerticalLine (juce::roundToInt (x),
                                   (float) waveArea.getY(),
                                   (float) waveArea.getBottom());
    }

    if (source == nullptr || source->getNumChannels() == 0 || source->getNumSamples() < 2)
    {
        graphics.setColour (SampleStackTheme::mutedText().withAlpha (0.8f));
        graphics.setFont (juce::Font (10.0f));
        graphics.drawText (label == "PRE-SAMPLE" ? "No pre-sample loaded" : "No main sample loaded",
                           waveArea, juce::Justification::centred, false);
        return;
    }

    if (sourceSampleRate <= 0.0 || timelineDuration <= 0.0 || waveArea.getWidth() < 2)
        return;

    const int sampleCount = source->getNumSamples();
    const int pixelCount = waveArea.getWidth();
    const float centreY = (float) waveArea.getCentreY();
    const float amplitudeScale = (float) waveArea.getHeight() * 0.46f;
    const double secondsPerPixel = timelineDuration / (double) pixelCount;

    juce::Path waveform;
    waveform.preallocateSpace (pixelCount * 3);

    for (int pixel = 0; pixel < pixelCount; ++pixel)
    {
        const double pixelStartTime = (double) pixel * secondsPerPixel;
        const double pixelEndTime = (double) (pixel + 1) * secondsPerPixel;

        const int start = juce::jlimit (0, sampleCount,
            (int) (pixelStartTime * sourceSampleRate));
        if (start >= sampleCount)
            continue;

        const int end = juce::jlimit (start + 1, sampleCount,
            (int) (pixelEndTime * sourceSampleRate) + 1);

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

        const float x = (float) (waveArea.getX() + pixel);
        waveform.startNewSubPath (x, centreY - maximum * amplitudeScale);
        waveform.lineTo (x, centreY - minimum * amplitudeScale);
    }

    graphics.setColour (colour);
    graphics.strokePath (waveform, juce::PathStrokeType (1.35f));
}
