#include "PluginProcessor.h"
#include "PluginEditor.h"

SampleStackAudioProcessor::SampleStackAudioProcessor()
    : juce::AudioProcessor (BusesProperties()
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameterState (*this, nullptr, "SAMPLESTACK_PARAMETERS",
                      SamplerParameters::createLayout())
{
}

void SampleStackAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    samplerEngine.prepareToPlay (sampleRate, samplesPerBlock);
}

void SampleStackAudioProcessor::releaseResources()
{
    samplerEngine.releaseResources();
}

bool SampleStackAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto output = layouts.getMainOutputChannelSet();
    return output == juce::AudioChannelSet::mono()
        || output == juce::AudioChannelSet::stereo();
}

void SampleStackAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    samplerEngine.processBlock (buffer, midiMessages);
}

juce::AudioProcessorEditor* SampleStackAudioProcessor::createEditor()
{
    return new SampleStackAudioProcessorEditor (*this);
}

bool SampleStackAudioProcessor::hasEditor() const { return true; }
const juce::String SampleStackAudioProcessor::getName() const { return JucePlugin_Name; }
bool SampleStackAudioProcessor::acceptsMidi() const { return true; }
bool SampleStackAudioProcessor::producesMidi() const { return false; }
bool SampleStackAudioProcessor::isMidiEffect() const { return false; }
double SampleStackAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int SampleStackAudioProcessor::getNumPrograms() { return 1; }
int SampleStackAudioProcessor::getCurrentProgram() { return 0; }
void SampleStackAudioProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }

const juce::String SampleStackAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return "Default";
}

void SampleStackAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void SampleStackAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto state = parameterState.copyState(); auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void SampleStackAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        if (xml->hasTagName (parameterState.state.getType()))
            parameterState.replaceState (juce::ValueTree::fromXml (*xml));
    }
}

SamplerEngine& SampleStackAudioProcessor::getSamplerEngine() noexcept
{
    return samplerEngine;
}

const SamplerEngine& SampleStackAudioProcessor::getSamplerEngine() const noexcept
{
    return samplerEngine;
}

juce::AudioProcessorValueTreeState& SampleStackAudioProcessor::getParameterState() noexcept
{
    return parameterState;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SampleStackAudioProcessor();
}
