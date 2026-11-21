#include "SamplerEngine.h"
#include <cmath>
#include <limits>
#include <utility>

SamplerEngine::SamplerEngine()
{
    formatManager.registerBasicFormats();
}

void SamplerEngine::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    playbackSampleRate = sampleRate;
    maximumBlockSize = samplesPerBlock;
}

void SamplerEngine::releaseResources()
{
    mainVoice = {};
    preVoice = {};
    mainPlaybackPosition.store (-1.0f);
    prePlaybackPosition.store (-1.0f);
}

bool SamplerEngine::loadSampleFromFile (bool isPreSample,
                                        const juce::File& file,
                                        juce::String& errorMessage)
{
    if (! file.existsAsFile())
    {
        errorMessage = "The selected sample file does not exist.";
        return false;
    }

    std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file));
    if (reader == nullptr)
    {
        errorMessage = "SampleStack could not read this audio format.";
        return false;
    }

    if (reader->lengthInSamples <= 0
        || reader->lengthInSamples > (juce::int64) std::numeric_limits<int>::max())
    {
        errorMessage = "This sample is empty or too large to load.";
        return false;
    }

    const int channelCount = juce::jlimit (1, 2, (int) reader->numChannels);
    const int sampleCount = (int) reader->lengthInSamples;
    juce::AudioBuffer<float> decoded (channelCount, sampleCount);

    if (! reader->read (&decoded, 0, sampleCount, 0, true, channelCount > 1))
    {
        errorMessage = "There was a problem decoding the selected sample.";
        return false;
    }

    {
        // Swap decoded data into the layer. The lock hold is short: no large
        // audio-buffer allocation or sample copy takes place while audio runs.
        const juce::ScopedLock lock (sampleDataLock);
        auto& destination = isPreSample ? preSample : mainSample;
        destination.setAudioData (std::move (decoded), reader->sampleRate, file.getFileName());

        if (isPreSample)
        {
            preVoice = {};
            prePlaybackPosition.store (-1.0f);
        }
        else
        {
            mainVoice = {};
            mainPlaybackPosition.store (-1.0f);
        }
    }

    errorMessage.clear();
    return true;
}

void SamplerEngine::copySampleForDisplay (bool isPreSample,
                                          juce::AudioBuffer<float>& destination,
                                          double& sourceSampleRate,
                                          juce::String& sourceName) const
{
    const juce::ScopedLock lock (sampleDataLock);
    const auto& source = isPreSample ? preSample : mainSample;
    const auto& audio = source.getAudioData();

    destination.setSize (audio.getNumChannels(), audio.getNumSamples(), false, false, true);
    for (int channel = 0; channel < audio.getNumChannels(); ++channel)
        destination.copyFrom (channel, 0, audio, channel, 0, audio.getNumSamples());

    sourceSampleRate = source.getSampleRate();
    sourceName = source.getSourceName();
}

void SamplerEngine::startVoice (Voice& voice, const SampleLayer& layer,
                                double playbackRate, int midiNote, float velocity,
                                float start, float end, float loopStart, float loopEnd,
                                bool loopEnabled, int delaySamples)
{
    if (! layer.hasAudio() || playbackRate <= 0.0 || layer.getSampleRate() <= 0.0)
    {
        voice.active = false;
        return;
    }

    const int length = layer.getAudioData().getNumSamples();
    const double startFrame = (double) juce::jlimit (0.0f, 1.0f, start) * length;
    const double endFrame = (double) juce::jlimit (0.0f, 1.0f, end) * length;

    voice.position = juce::jlimit (0.0, (double) (length - 1), startFrame);
    voice.endPosition = juce::jlimit (voice.position + 1.0, (double) length, endFrame);
    voice.loopStart = juce::jlimit (voice.position, voice.endPosition - 1.0,
                                    (double) juce::jlimit (0.0f, 1.0f, loopStart) * length);
    voice.loopEnd = juce::jlimit (voice.loopStart + 1.0, voice.endPosition,
                                  (double) juce::jlimit (0.0f, 1.0f, loopEnd) * length);

    const double semitoneRatio = std::pow (2.0, ((double) midiNote - 60.0) / 12.0);
    voice.increment = (layer.getSampleRate() / playbackRate) * semitoneRatio;
    voice.velocity = juce::jlimit (0.0f, 1.0f, velocity);
    voice.loopEnabled = loopEnabled && voice.loopEnd > voice.loopStart + 1.0;
    voice.delayRemaining = juce::jmax (0, delaySamples);
    voice.waitingForDelay = voice.delayRemaining > 0;
    voice.active = true;
}

void SamplerEngine::renderVoiceSample (Voice& voice, const SampleLayer& layer,
                                       float& left, float& right)
{
    left = 0.0f;
    right = 0.0f;

    if (! voice.active || voice.waitingForDelay || ! layer.hasAudio())
        return;

    const auto& audio = layer.getAudioData();
    const int length = audio.getNumSamples();

    // In loop mode, wrap at loopEnd rather than at the sample end marker.
    if (voice.loopEnabled && voice.position >= voice.loopEnd)
    {
        const double loopLength = voice.loopEnd - voice.loopStart;
        voice.position = voice.loopStart
            + std::fmod (juce::jmax (0.0, voice.position - voice.loopStart), loopLength);
    }
    else if (voice.position >= voice.endPosition || voice.position >= (double) length)
    {
        voice.active = false;
        return;
    }

    const int first = juce::jlimit (0, length - 1, (int) voice.position);
    const int second = juce::jmin (first + 1, length - 1);
    const float fraction = (float) (voice.position - (double) first);

    const float* channel0 = audio.getReadPointer (0);
    left = channel0[first] + (channel0[second] - channel0[first]) * fraction;

    if (audio.getNumChannels() > 1)
    {
        const float* channel1 = audio.getReadPointer (1);
        right = channel1[first] + (channel1[second] - channel1[first]) * fraction;
    }
    else
    {
        right = left;
    }

    left *= voice.velocity;
    right *= voice.velocity;
    voice.position += voice.increment;
}

void SamplerEngine::startTriggeredVoices (int midiNote, float velocity,
                                          const SamplerPlaybackSettings& settings)
{
    const int preDelaySamples = (int) juce::jmax (
        0.0, (double) settings.preDelayMs * playbackSampleRate / 1000.0);

    startVoice (mainVoice, mainSample, playbackSampleRate, midiNote, velocity,
                settings.mainStart, settings.mainEnd,
                settings.mainLoopStart, settings.mainLoopEnd,
                settings.mainLoopEnabled, 0);

    startVoice (preVoice, preSample, playbackSampleRate, midiNote, velocity,
                settings.preStart, settings.preEnd,
                settings.preLoopStart, settings.preLoopEnd,
                settings.preLoopEnabled, preDelaySamples);
}

void SamplerEngine::processBlock (juce::AudioBuffer<float>& output,
                                  juce::MidiBuffer& midiMessages,
                                  const SamplerPlaybackSettings& settings)
{
    output.clear();

    if (! sampleDataLock.tryEnter())
        return;

    const int numSamples = output.getNumSamples();
    const int numOutputChannels = output.getNumChannels();

    juce::MidiBuffer::Iterator midiIterator (midiMessages);
    juce::MidiMessage midiMessage;
    int midiSamplePosition = 0;
    bool hasMidiEvent = midiIterator.getNextEvent (midiMessage, midiSamplePosition);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        while (hasMidiEvent && midiSamplePosition <= sample)
        {
            if (midiMessage.isNoteOn())
                startTriggeredVoices (midiMessage.getNoteNumber(),
                                      midiMessage.getFloatVelocity(), settings);

            hasMidiEvent = midiIterator.getNextEvent (midiMessage, midiSamplePosition);
        }

        if (preVoice.waitingForDelay)
        {
            if (preVoice.delayRemaining > 0)
                --preVoice.delayRemaining;
            else
                preVoice.waitingForDelay = false;
        }

        float mainLeft = 0.0f, mainRight = 0.0f;
        float preLeft = 0.0f, preRight = 0.0f;
        renderVoiceSample (mainVoice, mainSample, mainLeft, mainRight);
        renderVoiceSample (preVoice, preSample, preLeft, preRight);

        const float master = juce::jlimit (0.0f, 1.0f, settings.masterGain);
        mainLeft *= settings.mainGain * master;
        mainRight *= settings.mainGain * master;
        preLeft *= settings.preGain * master;
        preRight *= settings.preGain * master;

        if (numOutputChannels == 1)
            output.addSample (0, sample, (mainLeft + mainRight + preLeft + preRight) * 0.5f);
        else if (numOutputChannels >= 2)
        {
            output.addSample (0, sample, mainLeft + preLeft);
            output.addSample (1, sample, mainRight + preRight);
        }
    }

    const float mainPosition = mainVoice.active && mainSample.hasAudio()
        ? juce::jlimit (0.0f, 1.0f,
            (float) (mainVoice.position / (double) mainSample.getAudioData().getNumSamples()))
        : -1.0f;
    const float prePosition = preVoice.active && preSample.hasAudio()
        ? juce::jlimit (0.0f, 1.0f,
            (float) (preVoice.position / (double) preSample.getAudioData().getNumSamples()))
        : -1.0f;

    mainPlaybackPosition.store (mainPosition);
    prePlaybackPosition.store (prePosition);
    sampleDataLock.exit();
}

float SamplerEngine::getMainPlaybackPosition() const noexcept { return mainPlaybackPosition.load(); }
float SamplerEngine::getPrePlaybackPosition() const noexcept { return prePlaybackPosition.load(); }
