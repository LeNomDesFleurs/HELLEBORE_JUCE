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
  BackgroundComponent background_component;


  juce::Slider variationSlider;
  juce::Slider feedbackSlider;
  juce::Slider combSizeSlider;
  juce::Slider dryWetSlider;

  using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
  std::unique_ptr<Attachment> variationAttachment;
  std::unique_ptr<Attachment> feedbackAttachment;
  std::unique_ptr<Attachment> combSizeAttachement;
  std::unique_ptr<Attachment> dryWetAttachement;
  // -----------Animation variables------------
  const int width{350};
  // entre 0 et 1
  float variation{0.5};
  bool freeze{false};
  float feedback{0.089};
  float dry_wet{1.};
  float size{0.66};
  bool repaint_ui{true};
  float rotation_status{};
  juce::Atomic<bool> parametersChanged{false};

  // animation lfos
  std::array<noi::Outils::LFO, 12> variationWidgetLfos;
  noi::Outils::SawTooth timeWidgetLfo;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelleboreEditor)
};
