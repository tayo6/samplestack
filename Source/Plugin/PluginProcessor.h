#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../Audio/SamplerEngine.h"
#include "../Parameters/SamplerParameters.h"

// Thin JUCE host adapter. DSP and sample ownership stay in Audio/;
// automatable values and their identifiers stay in Parameters/.
class SampleStackAudioProcessor final : public juce::AudioProcessor
{
public:
    SampleStackAudioProcessor();
    ~SampleStackAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>& buffer,
                       juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    SamplerEngine& getSamplerEngine() noexcept;
    const SamplerEngine& getSamplerEngine() const noexcept;
    juce::AudioProcessorValueTreeState& getParameterState() noexcept;

private:
    SamplerEngine samplerEngine;
    juce::AudioProcessorValueTreeState parameterState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleStackAudioProcessor)
};
