/*
  ==============================================================================

    LookAndFeel.h
    Created: 3 Apr 2023 9:49:06am
    Author:  thoma

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel();
    //! [otherLookAndFeel]

    //! [drawRotarySlider]
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;

    void drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& toggleButton,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;
};