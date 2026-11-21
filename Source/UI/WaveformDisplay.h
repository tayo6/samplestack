#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>

class WaveformDisplay final : public juce::Component
{
public:
    enum class Marker { Start, End, LoopStart, LoopEnd };

    struct MarkerValues
    {
        float start = 0.0f;
        float end = 1.0f;
        float loopStart = 0.25f;
        float loopEnd = 0.75f;
        bool loopEnabled = false;
    };

    std::function<void (bool isPreSample, Marker marker, float normalizedValue)> onMarkerChanged;

    void setWaveformSources (const juce::AudioBuffer<float>* preSample, double preSampleRate,
                             const juce::AudioBuffer<float>* mainSample, double mainSampleRate);
    void setMarkerValues (bool isPreSample, const MarkerValues& values);
    void setLoopEnabled (bool isPreSample, bool enabled);
    void setPlaybackPositions (float preSamplePosition, float mainSamplePosition);
    void setSampleNames (const juce::String& preSampleName, const juce::String& mainSampleName);

    void paint (juce::Graphics& graphics) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

private:
    struct LaneGeometry
    {
        juce::Rectangle<int> bounds;
        juce::Rectangle<int> waveform;
        double duration = 0.0;
    };

    LaneGeometry getLaneGeometry (bool isPreSample) const;
    void drawLane (juce::Graphics& graphics, const LaneGeometry& geometry,
                   const juce::AudioBuffer<float>* source, double sourceSampleRate,
                   double timelineDuration, juce::Colour colour, const juce::String& label,
                   const juce::String& sampleName, const MarkerValues& markers,
                   float playbackPosition) const;
    MarkerValues& getMarkerValues (bool isPreSample) noexcept;
    const MarkerValues& getMarkerValues (bool isPreSample) const noexcept;
    void notifyMarkerChanged (bool isPreSample, Marker marker, float value);

    const juce::AudioBuffer<float>* preSampleBuffer = nullptr;
    const juce::AudioBuffer<float>* mainSampleBuffer = nullptr;
    double preSampleRate = 0.0;
    double mainSampleRate = 0.0;
    juce::String preSampleName;
    juce::String mainSampleName;
    MarkerValues preMarkers;
    MarkerValues mainMarkers;
    float prePlaybackPosition = -1.0f;
    float mainPlaybackPosition = -1.0f;
    bool draggingPreSample = false;
    bool isDraggingMarker = false;
    Marker draggedMarker = Marker::Start;
};
