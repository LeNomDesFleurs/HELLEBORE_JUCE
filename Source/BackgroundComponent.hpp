#pragma once

#include <JuceHeader.h>

#include <random>

#include "CustomColors.hpp"
#include "GraphicTools.hpp"
#include "LookAndFeel.hpp"

struct BackgroundComponent : juce::Component {
  //   BackgroundComponent(juce::AudioProcessorValueTreeState& apvts);
  void paint(juce::Graphics& g) override;
  //   void resized() override;

  juce::Path paintMotif(int size_x, int size_y, int block_size);
};