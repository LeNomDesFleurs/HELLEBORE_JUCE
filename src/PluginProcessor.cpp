/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelleboreAudioProcessor::HelleboreAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
        ),
#else
    :
#endif
parameters(*this, nullptr, juce::Identifier("PARAMETERS"), {
    std::make_unique<juce::AudioParameterFloat>(
        "variation",
        "Variation",
        juce::NormalisableRange{0.f, 0.2f, 0.001f}, 0.1f),

    std::make_unique<juce::AudioParameterFloat>(
        "time",
        "Time",
        juce::NormalisableRange{0.1f, 4.0f, 0.01f, 0.3f, false}, 2.0f),

    std::make_unique<juce::AudioParameterFloat>(
        "comb_time",
        "Comb Time",
        juce::NormalisableRange{0.010f, 1.0f, 0.001f, 0.3f, false}, 0.10f),

    std::make_unique<juce::AudioParameterFloat>(
        "freeze",
        "Freeze",
        juce::NormalisableRange{0.0f, 2.0f, 0.01f}, 0.0f) ,

    std::make_unique<juce::AudioParameterFloat>(
        "dry_wet",
        "Dry Wet",
        juce::NormalisableRange{0.0f, 1.0f, 0.01f}, 0.50f)
    })

{
    variationParameter = parameters.getRawParameterValue("variation");
    timeParameter = parameters.getRawParameterValue("time");
    combTimeParameter = parameters.getRawParameterValue("comb_time");
    freezeParameter = parameters.getRawParameterValue("freeze");
    dryWetParameter = parameters.getRawParameterValue("dry_wet");
}

HelleboreAudioProcessor::~HelleboreAudioProcessor()
{
}

//==============================================================================
const juce::String HelleboreAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HelleboreAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HelleboreAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HelleboreAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HelleboreAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HelleboreAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HelleboreAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HelleboreAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HelleboreAudioProcessor::getProgramName (int index)
{
    return {};
}

void HelleboreAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HelleboreAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void HelleboreAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HelleboreAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HelleboreAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    //retrieve param
    //Q
    hellebore_parameters.variation = variationParameter->load();
    //Number of band
    hellebore_parameters.freeze = freezeParameter->load() > 1 ;
    //Ratio
    hellebore_parameters.dry_wet = dryWetParameter->load();
    //frequence
    hellebore_parameters.comb_time = combTimeParameter->load();
    //link
    hellebore_parameters.rt60 = timeParameter->load();
    //band mode
   // sinensis_parameters.band_selector_mode = static_cast <int> (bandModeParameter->load());
    //hellebore_parameters.freeze = 0;


    hellebore.updateParameters(hellebore_parameters);


for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {

     // to access the sample in the channel as a C-style array
    auto LeftChannelSamples = buffer.getWritePointer(0);
    auto RightChannelSamples = buffer.getWritePointer(1);

    for (auto n = 0; n < buffer.getNumSamples(); ++n) {
        stereo_samples[0] = LeftChannelSamples[n];
        stereo_samples[1] = RightChannelSamples[n];
        stereo_samples = hellebore.processStereo(stereo_samples);
        LeftChannelSamples[n] = stereo_samples[0];
        RightChannelSamples[n] = stereo_samples[1];

    }

  }

}

//==============================================================================
bool HelleboreAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HelleboreAudioProcessor::createEditor()
{
    return new HelleboreAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void HelleboreAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HelleboreAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HelleboreAudioProcessor();
}
