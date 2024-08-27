#pragma once

#include <JuceHeader.h>

#include <algorithm>
#include <cmath>

#include "BackgroundComponent.hpp"
#include "LookAndFeel.hpp"
#include "PluginProcessor.h"
#include "demoComponent.h"

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
  void paintSizeWidget(juce::Graphics& g);

 private:
  HelleboreAudioProcessor& audioProcessor;
  AudioProcessorValueTreeState* apvts;

  // Look and feel
  // CentricKnob centricKnob;
  DryWetLookAndFeel drywet_look_and_feel;
  PlaceHolder place_holder_look_and_feel;
  EmptyKnob empty_knob_look_and_feel;

  //----------Components---------------
  using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

  BackgroundComponent background_component;
  juce::Slider variationSlider;
  std::unique_ptr<Attachment> variationAttachment;

  juce::Slider timeSlider;
  std::unique_ptr<Attachment> timeAttachment;

  juce::Slider combSizeSlider;
  std::unique_ptr<Attachment> combSizeAttachement;

  juce::Slider dryWetSlider;
  std::unique_ptr<Attachment> dryWetAttachement;

  // -----------Animation variables------------
  const int width = 350;
  // entre 0 et 1
  float variation = 0.7;
  bool freeze = false;
  float time = 0.25;
  float dry_wet = 1.;
  float size = 0.92212;
  bool repaint_ui = true;
  float rotation_status;
  juce::Atomic<bool> parametersChanged{false};

  // animation lfos
  std::array<noi::Outils::LFO, 12> lfos = {
      noi::Outils::LFO(60, 0.2), noi::Outils::LFO(60, 0.2),
      noi::Outils::LFO(60, 0.2), noi::Outils::LFO(60, 0.2),
      noi::Outils::LFO(60, 0.2), noi::Outils::LFO(60, 0.2),
      noi::Outils::LFO(60, 0.2), noi::Outils::LFO(60, 0.2),
      noi::Outils::LFO(60, 0.2), noi::Outils::LFO(60, 0.2),
      noi::Outils::LFO(60, 0.2), noi::Outils::LFO(60, 0.2)};

  noi::Outils::SawTooth rotationLfo{60.f, 0.03};

  noi::Outils::SawTooth freeze_transition{60.f, 0.3};
  bool transition_done=false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelleboreEditor)
};
