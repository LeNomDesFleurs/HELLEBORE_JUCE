/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Filter.h"


//==============================================================================
SinensisAudioProcessor::SinensisAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
        ), parameters(*this, nullptr, "PARAMETERS", createParams())
#endif
    {}

SinensisAudioProcessor::~SinensisAudioProcessor()
{
}

//==============================================================================
const juce::String SinensisAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SinensisAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SinensisAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SinensisAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SinensisAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SinensisAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SinensisAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SinensisAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SinensisAudioProcessor::getProgramName (int index)
{
    return {};
}

void SinensisAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SinensisAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    for (int channel = 0; channel < 2; channel++){
            sinensis[channel].setSamplingFrequency(static_cast <float> (sampleRate));
      
    }
    //sinensis.setSamplingFrequency(static_cast <float> (sampleRate));

}

void SinensisAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SinensisAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SinensisAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuffer)
{

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    setParam();

   for (auto channel = 0; channel < 2; ++channel) {
        // to access the sample in the channel as a C-style array
        auto channelSamples = buffer.getWritePointer(channel);

        // for each sample in the channel
        for (auto n = 0; n < buffer.getNumSamples(); ++n) {
            const auto input = channelSamples[n];
            channelSamples[n] = sinensis[channel].processSinensis(input, midiBuffer);
        }
    }
 
}

//==============================================================================
bool SinensisAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SinensisAudioProcessor::createEditor()
{
    return new SinensisAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void SinensisAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SinensisAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout SinensisAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterChoice>("MIDIMODE", "Midi Mode", juce::StringArray{ "off", "mono", "poly" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("BANDMODE", "Band Selector Mode", juce::StringArray{ "Low/High", "Odd/Even", "Peak" }, 0));
   
    params.push_back(std::make_unique<juce::AudioParameterFloat>("root_frequency", "Root Frequency", juce::NormalisableRange{ 20.f, 20000.f, 0.1f, 0.2f, false }, 500.f)),
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RATIO", "Ratio", juce::NormalisableRange{ 0.5f, 2.0f, 0.001f }, 1.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RESONANCE","Resonance",juce::NormalisableRange{ 0.7f, 35.0f, 0.1f }, 20.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("band_selector", "Number Of Band", juce::NormalisableRange{ 0.0f, 1.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique <juce::AudioParameterFloat>("OUTPUTVOLUME", "Ouput Volume", juce::NormalisableRange{ 0.0f, 2.0f, 0.01f }, 0.6f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.1f }, 0.4f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.1f }, 0.4f));

    params.push_back(std::make_unique<juce::AudioParameterBool>("MIDIOFF", "Midi Off", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("MIDIMONO", "Midi Mono", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("MIDIPOLY", "Midi Poly", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>("LOWHIGH", "Low / High", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("ODDEVEN", "Odd / even", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("PEAK", "Peak", false));

    return { params.begin(), params.end() };
}

void SinensisAudioProcessor::setParam() {
    //MODE
    if (*parameters.getRawParameterValue("MIDIMONO")) sinensis_parameters.midi_mode = 1;
    else if (*parameters.getRawParameterValue("MIDIPOLY")) sinensis_parameters.midi_mode = 2;
    else sinensis_parameters.midi_mode = 0;

    if (*parameters.getRawParameterValue("LOWHIGH")) sinensis_parameters.band_selector_mode = 0;
    else if(*parameters.getRawParameterValue("ODDEVEN")) sinensis_parameters.band_selector_mode = 1;
    else sinensis_parameters.band_selector_mode = 2;
    //PARAM
    sinensis_parameters.root_frequency = *parameters.getRawParameterValue("root_frequency");
    sinensis_parameters.ratio = *parameters.getRawParameterValue("RATIO");
    sinensis_parameters.resonance = *parameters.getRawParameterValue("RESONANCE");
    sinensis_parameters.band_selector = *parameters.getRawParameterValue("band_selector");
    sinensis_parameters.output_volume = *parameters.getRawParameterValue("OUTPUTVOLUME");
    //ENVELOPPE
    sinensis_parameters.attack = *parameters.getRawParameterValue("ATTACK");
    sinensis_parameters.decay = *parameters.getRawParameterValue("DECAY");
    //SET OBJECT
    sinensis[0].setParameters(sinensis_parameters);
    sinensis[1].setParameters(sinensis_parameters);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SinensisAudioProcessor();
}
