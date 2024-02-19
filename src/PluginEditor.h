/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <cmath>

#include "LookAndFeel.hpp"
#include "PluginProcessor.h"

//==============================================================================
/**
 */
class HelleboreAudioProcessorEditor
    : public juce::AudioProcessorEditor,
      public juce::AudioProcessorParameter::Listener,
      public juce::Timer {
 public:
  HelleboreAudioProcessorEditor(HelleboreAudioProcessor&,
                                juce::AudioProcessorValueTreeState& vts);
  ~HelleboreAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;
  std::vector<juce::Component*> getComps();
  //   void sliderValueChanged(Slider* slider) override;
  void parameterValueChanged(int parameterIndex, float newvalue) override;
  void parameterGestureChanged(int parameterIndex,
                               bool gestureIsStarting) override {}
  void timerCallback() override;

 private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  HelleboreAudioProcessor& audioProcessor;

  CentricKnob centricKnob;

  RotationKnob rotationKnob;

  juce::Atomic<bool> parametersChanged{false};

  //   OtherLookAndFeel otherLookAndFeel;  // [2]

  juce::Slider variationSlider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      variationAttachment;
  juce::Label variationLabel;

  juce::Slider timeSlider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      timeAttachment;
  juce::Label timeLabel;

  juce::Slider combSizeSlider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      combSizeAttachement;
  juce::Label combSizeLabel;

  juce::Slider dryWetSlider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      dryWetAttachement;
  juce::Label dryWetLabel;

  juce::ToggleButton freezeSlider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
      freezeAttachement;
  juce::Label freezeLabel;
  const int width = 350;

  float elipseSize = 10.;

  // entre 0 et 1
  float variation = 0.5;

  bool freeze = false;

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

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelleboreAudioProcessorEditor)
};
