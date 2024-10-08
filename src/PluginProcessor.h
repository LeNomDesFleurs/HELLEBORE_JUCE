/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include <array>
#include <iostream>

#include "Hellebore.h"
#include "RingBuffer.hpp"
#include <optional>

//==============================================================================
/**
 */

noi::StereoMoorer::Parameters getSettings(
    juce::AudioProcessorValueTreeState& apvts);

class HelleboreAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    ,
                                public juce::AudioProcessorARAExtension
#endif
{
 public:
  //==============================================================================
  HelleboreAudioProcessor();
  ~HelleboreAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  static juce::AudioProcessorValueTreeState::ParameterLayout
  createParameterLayout();
  juce::AudioProcessorValueTreeState apvts{*this, nullptr, "Parameters",
                                           createParameterLayout()};
  //==============================================================================
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

 private:
  //==============================================================================
  noi::StereoMoorer::Parameters hellebore_parameters{false, 0.5F, 0.01f, 0.1f,
                                                     0.1f};
  noi::StereoMoorer hellebore;

  // std::array<float, 2> stereo_samples = { 0, 0 };

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelleboreAudioProcessor)
};
