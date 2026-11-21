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

SamplerPlaybackSettings SampleStackAudioProcessor::getPlaybackSettings() const noexcept
{
    const auto read = [this] (const char* id, float fallback)
    {
        if (auto* value = parameterState.getRawParameterValue (id))
            return value->load();
        return fallback;
    };

    SamplerPlaybackSettings s;
    s.masterGain = read (SamplerParameterIDs::masterGain, 0.8f);
    s.mainGain = read (SamplerParameterIDs::mainSampleGain, 1.0f);
    s.preGain = read (SamplerParameterIDs::preSampleGain, 1.0f);
    s.preDelayMs = read (SamplerParameterIDs::preSampleDelay, 0.0f);

    s.mainStart = read (SamplerParameterIDs::mainStart, 0.0f);
    s.mainEnd = read (SamplerParameterIDs::mainEnd, 1.0f);
    s.mainLoopStart = read (SamplerParameterIDs::mainLoopStart, 0.25f);
    s.mainLoopEnd = read (SamplerParameterIDs::mainLoopEnd, 0.75f);
    s.mainLoopEnabled = read (SamplerParameterIDs::mainLoopEnabled, 0.0f) > 0.5f;

    s.preStart = read (SamplerParameterIDs::preStart, 0.0f);
    s.preEnd = read (SamplerParameterIDs::preEnd, 1.0f);
    s.preLoopStart = read (SamplerParameterIDs::preLoopStart, 0.25f);
    s.preLoopEnd = read (SamplerParameterIDs::preLoopEnd, 0.75f);
    s.preLoopEnabled = read (SamplerParameterIDs::preLoopEnabled, 0.0f) > 0.5f;
    return s;
}

void SampleStackAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                               juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    samplerEngine.processBlock (buffer, midiMessages, getPlaybackSettings());
}

bool SampleStackAudioProcessor::loadSampleFile (bool isPreSample, const juce::File& file,
                                                juce::String& errorMessage)
{
    return samplerEngine.loadSampleFromFile (isPreSample, file, errorMessage);
}

void SampleStackAudioProcessor::copySampleForDisplay (bool isPreSample,
                                                       juce::AudioBuffer<float>& destination,
                                                       double& sourceSampleRate,
                                                       juce::String& sourceName) const
{
    samplerEngine.copySampleForDisplay (isPreSample, destination, sourceSampleRate, sourceName);
}

void SampleStackAudioProcessor::setParameterNormalized (const juce::String& parameterID,
                                                          float normalizedValue)
{
    if (auto* parameter = parameterState.getParameter (parameterID))
        parameter->setValueNotifyingHost (juce::jlimit (0.0f, 1.0f, normalizedValue));
}

float SampleStackAudioProcessor::getParameterValue (const juce::String& parameterID) const noexcept
{
    if (auto* value = parameterState.getRawParameterValue (parameterID))
        return value->load();
    return 0.0f;
}

float SampleStackAudioProcessor::getMainPlaybackPosition() const noexcept
{
    return samplerEngine.getMainPlaybackPosition();
}

float SampleStackAudioProcessor::getPrePlaybackPosition() const noexcept
{
    return samplerEngine.getPrePlaybackPosition();
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
    auto state = parameterState.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void SampleStackAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        if (xml->hasTagName (parameterState.state.getType().toString()))
            parameterState.replaceState (juce::ValueTree::fromXml (*xml));
    }
}

SamplerEngine& SampleStackAudioProcessor::getSamplerEngine() noexcept { return samplerEngine; }
const SamplerEngine& SampleStackAudioProcessor::getSamplerEngine() const noexcept { return samplerEngine; }
juce::AudioProcessorValueTreeState& SampleStackAudioProcessor::getParameterState() noexcept { return parameterState; }

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SampleStackAudioProcessor();
}
