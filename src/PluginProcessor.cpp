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
      ,
      hellebore(hellebore_parameters, 44100) {
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
  hellebore.reset(hellebore_parameters, (int)sampleRate);
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

  if (totalNumInputChannels == 2) {
    // to access the sample in the channel as an array
    auto LeftChannelSamples = buffer.getWritePointer(0);
    auto RightChannelSamples = buffer.getWritePointer(1);

    for (auto n = 0; n < buffer.getNumSamples(); ++n) {
      std::array<float, 2> stereo_samples = {LeftChannelSamples[n],
                                             RightChannelSamples[n]};
      stereo_samples = hellebore.processStereo(stereo_samples);

      LeftChannelSamples[n] = stereo_samples[0];
      RightChannelSamples[n] = stereo_samples[1];
    }
  } else {
    auto LeftChannelSamples = buffer.getWritePointer(0);
    for (auto n = 0; n < buffer.getNumSamples(); ++n) {
      std::array<float, 2> stereo_samples = {LeftChannelSamples[n], 0.f};
      stereo_samples = hellebore.processStereo(stereo_samples);

      LeftChannelSamples[n] = stereo_samples[0];
    }
  }
}

noi::StereoMoorer::Parameters getSettings(
    juce::AudioProcessorValueTreeState& apvts) {
  noi::StereoMoorer::Parameters settings;

  settings.dry_wet = apvts.getRawParameterValue("dry_wet")->load();
  settings.comb_time = apvts.getRawParameterValue("comb_time")->load();
  settings.variation = apvts.getRawParameterValue("variation")->load();
  settings.feedback = apvts.getRawParameterValue("feedback")->load();
  // minimum feedback grows when comb feedback grows to keep some minimal
  // feedback max comb_time = 1.5 -> 1.5 * 5 settings.feedback =
  // noi::Outils::mapValue(feedback, 0.0, 1.0, settings.comb_time / 20.f, 20.);
  settings.freeze = settings.feedback >= 1.0;

  return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout
HelleboreAudioProcessor::createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;

  using FloatRange = juce::NormalisableRange<float>;
  using FloatParam = juce::AudioParameterFloat;

  layout.add(std::make_unique<FloatParam>(
      "dry_wet", "dry_wet", FloatRange(0.f, 1.f, 0.01f, 1.f), 1.f));
  layout.add(std::make_unique<FloatParam>(
      "variation", "variation", FloatRange(0.f, 1.f, 0.0001f, 0.3f), 0.1));
  layout.add(std::make_unique<FloatParam>(
      "comb_time", "comb_time", FloatRange(0.01f, 3.9f, 0.0001f, 0.3f), 1.f));
  layout.add(std::make_unique<FloatParam>(
      "feedback", "feedback", FloatRange(0.0f, 1.0f, 0.001f, 2.f), 0.3f));

  return layout;
}

//==============================================================================
bool HelleboreAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HelleboreAudioProcessor::createEditor() {
  return new HelleboreEditor(*this, apvts);
  // return new GenericAudioProcessorEditor(*this);
}

//==============================================================================
void HelleboreAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  auto state = apvts.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void HelleboreAudioProcessor::setStateInformation(const void* data,
                                                  int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));

  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(apvts.state.getType()))
      apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new HelleboreAudioProcessor();
}
