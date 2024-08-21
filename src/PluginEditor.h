/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <cmath>

#include "BackgroundComponent.hpp"
#include "LookAndFeel.hpp"
#include "PluginProcessor.h"
#include "demoComponent.h"

//==============================================================================
/**
 */

class HelleboreEditor : public juce::AudioProcessorEditor,
                        public juce::AudioProcessorParameter::Listener,
                        public juce::Timer {
 public:
  HelleboreEditor(HelleboreAudioProcessor&,
                  juce::AudioProcessorValueTreeState& vts);
  ~HelleboreEditor() override;

  //==============================================================================
  void paintOverChildren(juce::Graphics&) override;
  void resized() override;
  std::vector<juce::Slider*> getComps();
  //   void sliderValueChanged(Slider* slider) override;
  void parameterValueChanged(int parameterIndex, float newvalue) override;
  void parameterGestureChanged(int parameterIndex,
                               bool gestureIsStarting) override {}
  void timerCallback() override;
  void paintVariationWidget(juce::Graphics& g);
  void paintTimeWidget(juce::Graphics& g);

 private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  BackgroundComponent background_component;

  HelleboreAudioProcessor& audioProcessor;

  CentricKnob centricKnob;
  DryWetLookAndFeel drywet_look_and_feel;
  PlaceHolder place_holder_look_and_feel;
  EmptyKnob empty_knob_look_and_feel;
  RotationKnob rotationKnob;

  juce::Atomic<bool> parametersChanged{false};

  //   OtherLookAndFeel otherLookAndFeel;  // [2]

  using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

  juce::Slider variationSlider;
  std::unique_ptr<Attachment> variationAttachment;
  juce::Label variationLabel;

  juce::Slider timeSlider;
  std::unique_ptr<Attachment> timeAttachment;
  juce::Label timeLabel;

  juce::Slider combSizeSlider;
  std::unique_ptr<Attachment> combSizeAttachement;
  juce::Label combSizeLabel;

  juce::Slider dryWetSlider;
  std::unique_ptr<Attachment> dryWetAttachement;
  juce::Label dryWetLabel;

  // Animation variables
  const int width = 350;
  float elipseSize = 10.;
  // entre 0 et 1
  float variation = 0.7;
  bool freeze = false;
  float time = 0.25;
  bool repaint_ui=true;

  std::array<noi::Outils::TriangleWave, 12> lfos = {
      noi::Outils::TriangleWave(60, 0.2), noi::Outils::TriangleWave(60, 0.2),
      noi::Outils::TriangleWave(60, 0.2), noi::Outils::TriangleWave(60, 0.2),
      noi::Outils::TriangleWave(60, 0.2), noi::Outils::TriangleWave(60, 0.2),
      noi::Outils::TriangleWave(60, 0.2), noi::Outils::TriangleWave(60, 0.2),
      noi::Outils::TriangleWave(60, 0.2), noi::Outils::TriangleWave(60, 0.2),
      noi::Outils::TriangleWave(60, 0.2), noi::Outils::TriangleWave(60, 0.2)};

  noi::Outils::SawTooth rotationLfo{60.f, 0.03};
  float rotation_status;
  AudioProcessorValueTreeState* apvts;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelleboreEditor)
};
