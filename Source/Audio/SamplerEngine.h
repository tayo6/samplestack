#pragma once

#include "SampleLayer.h"
#include <juce_audio_formats/juce_audio_formats.h>
#include <atomic>

struct SamplerPlaybackSettings
{
    float masterGain = 0.8f;
    float mainGain = 1.0f;
    float preGain = 1.0f;
    float preDelayMs = 0.0f;

    float mainStart = 0.0f;
    float mainEnd = 1.0f;
    float mainLoopStart = 0.25f;
    float mainLoopEnd = 0.75f;
    bool mainLoopEnabled = false;

    float preStart = 0.0f;
    float preEnd = 1.0f;
    float preLoopStart = 0.25f;
    float preLoopEnd = 0.75f;
    bool preLoopEnabled = false;
};

class SamplerEngine
{
public:
    SamplerEngine();

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    bool loadSampleFromFile (bool isPreSample,
                             const juce::File& file,
                             juce::String& errorMessage);

    void copySampleForDisplay (bool isPreSample,
                               juce::AudioBuffer<float>& destination,
                               double& sourceSampleRate,
                               juce::String& sourceName) const;

    void processBlock (juce::AudioBuffer<float>& output,
                       juce::MidiBuffer& midiMessages,
                       const SamplerPlaybackSettings& settings);

    float getMainPlaybackPosition() const noexcept;
    float getPrePlaybackPosition() const noexcept;

private:
    struct Voice
    {
        bool active = false;
        bool waitingForDelay = false;
        int delayRemaining = 0;
        double position = 0.0;
        double endPosition = 0.0;
        double loopStart = 0.0;
        double loopEnd = 0.0;
        double increment = 1.0;
        float velocity = 1.0f;
        bool loopEnabled = false;
    };

    static void startVoice (Voice& voice, const SampleLayer& layer,
                            double playbackRate, int midiNote, float velocity,
                            float start, float end, float loopStart, float loopEnd,
                            bool loopEnabled, int delaySamples);
    static void renderVoiceSample (Voice& voice, const SampleLayer& layer,
                                   float& left, float& right);
    void startTriggeredVoices (int midiNote, float velocity,
                               const SamplerPlaybackSettings& settings);

    SampleLayer mainSample;
    SampleLayer preSample;
    juce::AudioFormatManager formatManager;

    // File decoding and UI snapshots use this lock on the message thread.
    // processBlock uses tryEnter(), so the audio callback never waits for file I/O.
    mutable juce::CriticalSection sampleDataLock;

    Voice mainVoice;
    Voice preVoice;
    std::atomic<float> mainPlaybackPosition { -1.0f };
    std::atomic<float> prePlaybackPosition { -1.0f };

    double playbackSampleRate = 44100.0;
    int maximumBlockSize = 0;
};
