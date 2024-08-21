#pragma once

#include <JuceHeader.h>

#include <cmath>

#include "CustomColors.hpp"

class CentricKnob : public juce::LookAndFeel_V4 {
 public:
  CentricKnob() { setColour(juce::Slider::thumbColourId, juce::Colours::red); }
  //! [otherLookAndFeel]

  //! [drawRotarySlider]
  void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider& slider) override;
  //! [drawRotarySlider]
  void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        const Slider::SliderStyle style,
                        Slider& slider) override;

  void drawToggleButton(Graphics& g, ToggleButton& button,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override;
};

class RotationKnob : public juce::LookAndFeel_V4 {
 public:
  RotationKnob() { setColour(juce::Slider::thumbColourId, juce::Colours::red); }
  //! [otherLookAndFeel]

  //! [drawRotarySlider]
  void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider& slider) override;
};

class DryWetLookAndFeel : public juce::LookAndFeel_V4 {
 public:
  void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider& slider) override;
};

class EmptyKnob : public juce::LookAndFeel_V4 {
 public:
  EmptyKnob() {};
  void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider& slider) override {};
};

class PlaceHolder : public juce::LookAndFeel_V4 {
 public:
  PlaceHolder() {};
  void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider& slider) override {
    juce::Path p;
    p.addRectangle(x, y, width, height);
    g.setColour(juce::Colour::fromString("80800000"));
    g.fillPath(p);
  };
};