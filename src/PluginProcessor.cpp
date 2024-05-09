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
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
{
}

HelleboreAudioProcessor::~HelleboreAudioProcessor() {}

//==============================================================================
const juce::String HelleboreAudioProcessor::getName() const {
  return "hellebore";
}

bool HelleboreAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool HelleboreAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool HelleboreAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double HelleboreAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int HelleboreAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int HelleboreAudioProcessor::getCurrentProgram() { return 0; }

void HelleboreAudioProcessor::setCurrentProgram(int index) {}

const juce::String HelleboreAudioProcessor::getProgramName(int index) {
  return {};
}

void HelleboreAudioProcessor::changeProgramName(int index,
                                                const juce::String& newName) {}

//==============================================================================
void HelleboreAudioProcessor::prepareToPlay(double sampleRate,
                                            int samplesPerBlock) {
  hellebore.setSampleRate(sampleRate);
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void HelleboreAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HelleboreAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void HelleboreAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  hellebore_parameters = getSettings(apvts);
  hellebore.updateParameters(hellebore_parameters);
  // ringBuffer.setDelayTime(hellebore_parameters.comb_time * 1000);

  // hellebore.updateParameters(hellebore_parameters);

  // for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
  // to access the sample in the channel as a C-style array
  auto LeftChannelSamples = buffer.getWritePointer(0);
  auto RightChannelSamples = buffer.getWritePointer(1);
  // auto sample =

  for (auto n = 0; n < buffer.getNumSamples(); ++n) {
    std::array<float, 2> stereo_samples = {LeftChannelSamples[n],
                                           RightChannelSamples[n]};
    stereo_samples = hellebore.processStereo(stereo_samples);

    // ringBuffer.writeSample(LeftChannelSamples[n]);

    // LeftChannelSamples[n] = ringBuffer.readSample();

    LeftChannelSamples[n] = stereo_samples[0];
    RightChannelSamples[n] = stereo_samples[1];

    // ringBuffer.writeSample(RightChannelSamples[n]);
    // LeftChannelSamples[n] = LeftChannelSamples[n];
    // RightChannelSamples[n] = ringBuffer.readSample();
  }

  // }
}

noi::StereoMoorer::Parameters getSettings(
    juce::AudioProcessorValueTreeState& apvts) {
  noi::StereoMoorer::Parameters settings;

  settings.freeze = apvts.getRawParameterValue("freeze")->load() > 0.5;
  settings.dry_wet = apvts.getRawParameterValue("dry_wet")->load();
  settings.comb_time = apvts.getRawParameterValue("comb_time")->load();
  settings.variation = apvts.getRawParameterValue("variation")->load();
  settings.rt60 = apvts.getRawParameterValue("rt60")->load();

  return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout
HelleboreAudioProcessor::createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      "dry_wet", "dry_wet",
      juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 1.f));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      "variation", "variation",
      juce::NormalisableRange<float>(0.f, 1.f, 0.0001f, 0.3f), 0.1));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      "comb_time", "comb_time",
      juce::NormalisableRange<float>(0.f, 1.5f, 0.0001f, 0.2f), 1.f));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      "rt60", "rt60", juce::NormalisableRange<float>(0.f, 20.f, 0.1f, 1.f),
      5.f));
  layout.add(
      std::make_unique<juce::AudioParameterBool>("freeze", "freeze", false));

  return layout;
}

//==============================================================================
bool HelleboreAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HelleboreAudioProcessor::createEditor() {
  return new HelleboreAudioProcessorEditor(*this, apvts);
  // return new GenericAudioProcessorEditor(*this);
}

//==============================================================================
void HelleboreAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void HelleboreAudioProcessor::setStateInformation(const void* data,
                                                  int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new HelleboreAudioProcessor();
}
