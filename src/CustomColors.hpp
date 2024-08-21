#pragma once

#include <JuceHeader.h>

namespace CustomColors {

const juce::Colour black = juce::Colour::fromString("88000000");

// const juce::Colour pink = juce::Colour::fromString("FFEE3AAD");  // red
const juce::Colour pink = juce::Colour::fromString("FFEE3AAD");  // red
const juce::Colour red = juce::Colour::fromString("FFE2475B");
const juce::Colour orange = juce::Colour::fromString("FFEF8358");
const juce::Colour light_green = juce::Colour::fromString("FF46B26C");
const juce::Colour dark_green =
    juce::Colour::fromString("FF335C57");  // darker green

const std::vector<juce::Colour> custom_colors = {dark_green, light_green,
                                                 orange, red, pink};

juce::Colour getGradient(float value);
juce::Colour getGradientWithoutGreen(float value);
}  // namespace CustomColors
