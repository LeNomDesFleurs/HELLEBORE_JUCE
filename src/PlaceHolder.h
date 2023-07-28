/*
  ==============================================================================

    PlaceHolder.h
    Created: 3 Apr 2023 10:04:12am
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct Placeholder : juce::Component
{
    Placeholder();

    void paint(juce::Graphics& g) override;

    juce::Colour customColor;
};

struct RotarySlider : juce::Slider
{
    RotarySlider();
};