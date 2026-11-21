#include "WaveformDisplay.h"
#include "SampleStackTheme.h"
#include <array>
#include <cmath>
#include <utility>
#include <vector>

namespace
{
    namespace simpler = SampleStackTheme::simpler;

    // Round tick times (seconds) at least ~80 px apart, like Simpler's time ruler.
    std::vector<double> getRulerTicks (double durationSeconds, int widthPixels)
    {
        std::vector<double> ticks;
        if (durationSeconds <= 0.0 || widthPixels < 2)
            return ticks;

        static constexpr std::array<double, 15> steps {{
            0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 30.0, 60.0
        }};

        const double pixelsPerSecond = (double) widthPixels / durationSeconds;
        double step = steps.back();
        for (const double candidate : steps)
        {
            if (candidate * pixelsPerSecond >= 80.0)
            {
                step = candidate;
                break;
            }
        }

        const int count = (int) std::floor (durationSeconds / step + 1.0e-9);
        for (int i = 0; i <= count; ++i)
            ticks.push_back ((double) i * step);

        return ticks;
    }
}

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

void WaveformDisplay::setMarkerValues (bool isPreSample, const MarkerValues& values)
{
    getMarkerValues (isPreSample) = values;
    repaint();
}

void WaveformDisplay::setLoopEnabled (bool isPreSample, bool enabled)
{
    getMarkerValues (isPreSample).loopEnabled = enabled;
    repaint();
}

void WaveformDisplay::setPlaybackPositions (float prePosition, float mainPosition)
{
    prePlaybackPosition = prePosition;
    mainPlaybackPosition = mainPosition;
    repaint();
}

void WaveformDisplay::setSampleNames (const juce::String& preName, const juce::String& mainName)
{
    preSampleName = preName;
    mainSampleName = mainName;
    repaint();
}

WaveformDisplay::MarkerValues& WaveformDisplay::getMarkerValues (bool isPreSample) noexcept
{
    return isPreSample ? preMarkers : mainMarkers;
}

const WaveformDisplay::MarkerValues& WaveformDisplay::getMarkerValues (bool isPreSample) const noexcept
{
    return isPreSample ? preMarkers : mainMarkers;
}

WaveformDisplay::LaneGeometry WaveformDisplay::getLaneGeometry (bool isPreSample) const
{
    auto content = getLocalBounds().reduced (12, 9);
    content.removeFromTop (19);
    content.removeFromTop (23);
    auto preLane = content.removeFromTop (content.getHeight() / 2 - 4);
    content.removeFromTop (8);
    auto mainLane = content;

    LaneGeometry geometry;
    geometry.bounds = isPreSample ? preLane : mainLane;
    geometry.waveform = geometry.bounds.withTrimmedLeft (124).reduced (0, 7);

    const auto* buffer = isPreSample ? preSampleBuffer : mainSampleBuffer;
    const double rate = isPreSample ? preSampleRate : mainSampleRate;
    if (buffer != nullptr && rate > 0.0)
        geometry.duration = (double) buffer->getNumSamples() / rate;

    return geometry;
}

void WaveformDisplay::paint (juce::Graphics& graphics)
{
    // Flat black "screen" with a thin border, as in Simpler's sample display.
    graphics.setColour (simpler::display());
    graphics.fillRect (getLocalBounds().toFloat());
    graphics.setColour (simpler::fieldBorder());
    graphics.drawRect (getLocalBounds().toFloat(), 1.0f);

    auto content = getLocalBounds().reduced (12, 9);
    auto heading = content.removeFromTop (19);
    graphics.setColour (SampleStackTheme::text());
    graphics.setFont (juce::Font (10.5f, juce::Font::bold));
    graphics.drawText ("SAMPLE WAVEFORM", heading, juce::Justification::centredLeft, false);

    auto ruler = content.removeFromTop (23);
    auto timeline = ruler.withTrimmedLeft (124);
    const double timelineDuration = juce::jmax (getLaneGeometry (true).duration,
                                                 getLaneGeometry (false).duration);

    graphics.setFont (juce::Font (11.0f));
    for (const double tickTime : getRulerTicks (timelineDuration, timeline.getWidth()))
    {
        const float x = (float) timeline.getX()
            + (float) (tickTime / timelineDuration) * (float) timeline.getWidth();

        graphics.setColour (simpler::rulerTick());
        graphics.fillRect (x, (float) ruler.getY() + 4.0f, 1.0f, (float) ruler.getHeight() - 4.0f);

        graphics.setColour (simpler::rulerText());
        graphics.drawText (SampleStackTheme::formatRulerTime (tickTime),
                           juce::Rectangle<float> (x + 3.0f, (float) ruler.getY(),
                                                   70.0f, (float) ruler.getHeight()),
                           juce::Justification::centredLeft, false);
    }

    drawLane (graphics, getLaneGeometry (true), preSampleBuffer, preSampleRate,
              timelineDuration, SampleStackTheme::preSample(), "PRE-SAMPLE",
              preSampleName, preMarkers, prePlaybackPosition);
    drawLane (graphics, getLaneGeometry (false), mainSampleBuffer, mainSampleRate,
              timelineDuration, SampleStackTheme::mainSample(), "MAIN SAMPLE",
              mainSampleName, mainMarkers, mainPlaybackPosition);
}

void WaveformDisplay::drawLane (juce::Graphics& graphics, const LaneGeometry& geometry,
                                const juce::AudioBuffer<float>* source, double sourceSampleRate,
                                double timelineDuration, juce::Colour colour,
                                const juce::String& label, const juce::String& sampleName,
                                const MarkerValues& markers, float playbackPosition) const
{
    if (geometry.bounds.isEmpty())
        return;

    graphics.setColour (simpler::display().darker (0.18f));
    graphics.fillRect (geometry.bounds.toFloat());

    auto labelArea = geometry.bounds.withWidth (116).reduced (8, 0);
    auto waveArea = geometry.waveform;
    graphics.setColour (colour);
    graphics.setFont (juce::Font (9.5f, juce::Font::bold));
    graphics.drawText (label, labelArea.withTrimmedBottom (geometry.bounds.getHeight() / 2),
                       juce::Justification::centredLeft, false);
    graphics.setColour (SampleStackTheme::mutedText());
    graphics.setFont (juce::Font (8.5f));
    graphics.drawFittedText (sampleName.isEmpty() ? "No file loaded" : sampleName,
                             labelArea.withTrimmedTop (geometry.bounds.getHeight() / 2),
                             juce::Justification::centredLeft, 1);

    // Zero line in the layer colour, like the thin orange centre line in Simpler.
    graphics.setColour (colour.withAlpha (0.45f));
    graphics.drawHorizontalLine (waveArea.getCentreY(), (float) waveArea.getX(),
                                 (float) waveArea.getRight());

    // Grid lines share the ruler's tick positions so they line up with its labels.
    graphics.setColour (SampleStackTheme::border().withAlpha (0.45f));
    const auto gridTicks = getRulerTicks (timelineDuration, waveArea.getWidth());
    if (! gridTicks.empty())
    {
        for (const double tickTime : gridTicks)
        {
            const float x = (float) waveArea.getX()
                + (float) (tickTime / timelineDuration) * (float) waveArea.getWidth();
            graphics.drawVerticalLine (juce::roundToInt (x), (float) waveArea.getY(),
                                       (float) waveArea.getBottom());
        }
    }
    else
    {
        for (int tick = 0; tick <= 8; ++tick)
        {
            const float x = (float) waveArea.getX() + waveArea.getWidth() * (float) tick / 8.0f;
            graphics.drawVerticalLine (juce::roundToInt (x), (float) waveArea.getY(),
                                       (float) waveArea.getBottom());
        }
    }

    if (source == nullptr || source->getNumChannels() == 0 || source->getNumSamples() < 2)
    {
        graphics.setColour (SampleStackTheme::mutedText().withAlpha (0.8f));
        graphics.setFont (juce::Font (10.0f));
        graphics.drawText (label == "PRE-SAMPLE" ? "Load a pre-sample to begin"
                                                 : "Load a main sample to begin",
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
        const int start = juce::jlimit (0, sampleCount,
            (int) ((double) pixel * secondsPerPixel * sourceSampleRate));
        if (start >= sampleCount)
            continue;
        const int end = juce::jlimit (start + 1, sampleCount,
            (int) ((double) (pixel + 1) * secondsPerPixel * sourceSampleRate) + 1);

        float minimum = 0.0f, maximum = 0.0f;
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

    const double laneDuration = geometry.duration;
    const auto markerX = [&] (float normalized)
    {
        const double markerTime = (double) juce::jlimit (0.0f, 1.0f, normalized) * laneDuration;
        return (float) waveArea.getX()
            + (float) (markerTime / timelineDuration) * (float) waveArea.getWidth();
    };

    if (markers.loopEnabled)
    {
        const float loopStartX = markerX (markers.loopStart);
        const float loopEndX = markerX (markers.loopEnd);
        graphics.setColour (colour.withAlpha (0.10f));
        graphics.fillRect (juce::Rectangle<float> (loopStartX, (float) waveArea.getY(),
            juce::jmax (0.0f, loopEndX - loopStartX), (float) waveArea.getHeight()));
        graphics.setColour (colour);
        graphics.drawVerticalLine (juce::roundToInt (loopStartX), (float) waveArea.getY(),
                                   (float) waveArea.getBottom());
        graphics.drawVerticalLine (juce::roundToInt (loopEndX), (float) waveArea.getY(),
                                   (float) waveArea.getBottom());
        graphics.fillEllipse (loopStartX - 3.0f, (float) waveArea.getY() - 1.0f, 6.0f, 6.0f);
        graphics.fillEllipse (loopEndX - 3.0f, (float) waveArea.getY() - 1.0f, 6.0f, 6.0f);
    }

    const float startX = markerX (markers.start);
    const float endX = markerX (markers.end);

    // Dim everything outside Start/End, as Simpler does.
    const float laneLeft = (float) waveArea.getX();
    const float laneRight = (float) waveArea.getRight();
    const float clampedStartX = juce::jlimit (laneLeft, laneRight, startX);
    const float clampedEndX = juce::jlimit (laneLeft, laneRight, endX);
    graphics.setColour (juce::Colours::black.withAlpha (0.45f));
    graphics.fillRect (laneLeft, (float) waveArea.getY(),
                       clampedStartX - laneLeft, (float) waveArea.getHeight());
    graphics.fillRect (clampedEndX, (float) waveArea.getY(),
                       laneRight - clampedEndX, (float) waveArea.getHeight());

    // Start/End handles: grey side triangles on thin lines (start points right, end left).
    const float handleY = (float) waveArea.getY() + 10.0f;
    graphics.setColour (simpler::handle());
    graphics.drawVerticalLine (juce::roundToInt (startX), (float) waveArea.getY(),
                               (float) waveArea.getBottom());
    graphics.drawVerticalLine (juce::roundToInt (endX), (float) waveArea.getY(),
                               (float) waveArea.getBottom());
    juce::Path startHandle;
    startHandle.addTriangle (startX, handleY - 6.0f, startX, handleY + 6.0f,
                             startX + 9.0f, handleY);
    graphics.fillPath (startHandle);
    juce::Path endHandle;
    endHandle.addTriangle (endX, handleY - 6.0f, endX, handleY + 6.0f,
                           endX - 9.0f, handleY);
    graphics.fillPath (endHandle);

    if (playbackPosition >= 0.0f && playbackPosition <= 1.0f)
    {
        graphics.setColour (SampleStackTheme::playhead());
        graphics.drawLine (markerX (playbackPosition), (float) waveArea.getY(),
                           markerX (playbackPosition), (float) waveArea.getBottom(), 1.5f);
    }
}

void WaveformDisplay::mouseDown (const juce::MouseEvent& event)
{
    isDraggingMarker = false;
    const auto point = event.position;
    float nearestDistance = 11.0f;
    const double timelineDuration = juce::jmax (getLaneGeometry (true).duration,
                                                 getLaneGeometry (false).duration);

    for (bool isPre : { true, false })
    {
        const auto geometry = getLaneGeometry (isPre);
        if (! geometry.waveform.contains (event.getPosition()) || geometry.duration <= 0.0
            || timelineDuration <= 0.0)
            continue;

        const auto& markers = getMarkerValues (isPre);
        const auto markerX = [&] (float normalized)
        {
            return (float) geometry.waveform.getX()
                + (float) ((double) normalized * geometry.duration / timelineDuration)
                    * (float) geometry.waveform.getWidth();
        };

        const std::array<std::pair<Marker, float>, 4> markerList {{
            { Marker::Start, markerX (markers.start) },
            { Marker::End, markerX (markers.end) },
            { Marker::LoopStart, markerX (markers.loopStart) },
            { Marker::LoopEnd, markerX (markers.loopEnd) }
        }};

        for (const auto& item : markerList)
        {
            if ((item.first == Marker::LoopStart || item.first == Marker::LoopEnd)
                && ! markers.loopEnabled)
                continue;

            const float distance = std::abs (point.x - item.second);
            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                draggedMarker = item.first;
                draggingPreSample = isPre;
                isDraggingMarker = true;
            }
        }
    }

    if (isDraggingMarker)
        setMouseCursor (juce::MouseCursor::LeftRightResizeCursor);
}

void WaveformDisplay::mouseDrag (const juce::MouseEvent& event)
{
    if (! isDraggingMarker)
        return;

    const auto geometry = getLaneGeometry (draggingPreSample);
    const double timelineDuration = juce::jmax (getLaneGeometry (true).duration,
                                                 getLaneGeometry (false).duration);
    if (geometry.waveform.getWidth() < 2 || geometry.duration <= 0.0 || timelineDuration <= 0.0)
        return;

    const float relative = (event.position.x - (float) geometry.waveform.getX())
        / (float) geometry.waveform.getWidth();
    const float normalized = juce::jlimit (0.0f, 1.0f,
        (float) ((double) relative * timelineDuration / geometry.duration));

    auto& markers = getMarkerValues (draggingPreSample);
    constexpr float gap = 0.002f;

    switch (draggedMarker)
    {
        case Marker::Start:
            markers.start = juce::jmin (normalized, markers.end - gap);
            if (markers.loopStart < markers.start)
                markers.loopStart = markers.start;
            if (markers.loopEnd < markers.loopStart + gap)
                markers.loopEnd = juce::jmin (markers.end, markers.loopStart + gap);
            notifyMarkerChanged (draggingPreSample, Marker::Start, markers.start);
            notifyMarkerChanged (draggingPreSample, Marker::LoopStart, markers.loopStart);
            notifyMarkerChanged (draggingPreSample, Marker::LoopEnd, markers.loopEnd);
            break;

        case Marker::End:
            markers.end = juce::jmax (normalized, markers.start + gap);
            if (markers.loopEnd > markers.end)
                markers.loopEnd = markers.end;
            if (markers.loopStart > markers.loopEnd - gap)
                markers.loopStart = juce::jmax (markers.start, markers.loopEnd - gap);
            notifyMarkerChanged (draggingPreSample, Marker::End, markers.end);
            notifyMarkerChanged (draggingPreSample, Marker::LoopStart, markers.loopStart);
            notifyMarkerChanged (draggingPreSample, Marker::LoopEnd, markers.loopEnd);
            break;

        case Marker::LoopStart:
            markers.loopStart = juce::jlimit (markers.start, markers.loopEnd - gap, normalized);
            notifyMarkerChanged (draggingPreSample, Marker::LoopStart, markers.loopStart);
            break;

        case Marker::LoopEnd:
            markers.loopEnd = juce::jlimit (markers.loopStart + gap, markers.end, normalized);
            notifyMarkerChanged (draggingPreSample, Marker::LoopEnd, markers.loopEnd);
            break;
    }

    repaint();
}

void WaveformDisplay::mouseUp (const juce::MouseEvent&)
{
    isDraggingMarker = false;
    setMouseCursor (juce::MouseCursor::NormalCursor);
}

void WaveformDisplay::notifyMarkerChanged (bool isPreSample, Marker marker, float value)
{
    if (onMarkerChanged)
        onMarkerChanged (isPreSample, marker, value);
}
