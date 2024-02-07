#pragma once

#include <JuceHeader.h>

class CentricKnob : public juce::LookAndFeel_V4 {
 public:
  CentricKnob() { setColour(juce::Slider::thumbColourId, juce::Colours::red); }
  //! [otherLookAndFeel]

  //! [drawRotarySlider]
  void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle, juce::Slider&) override;
  //! [drawRotarySlider]
};