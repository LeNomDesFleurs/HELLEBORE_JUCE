#pragma once

#include <JuceHeader.h>

namespace CustomColors {



// const juce::Colour brown = juce::Colour::fromString("FF50322a");
// const juce::Colour green = juce::Colour::fromString("FF608537");
// const juce::Colour paler_green = juce::Colour::fromString("FF798c53");
// const juce::Colour beige = juce::Colour::fromString("FFedb98f");
// const juce::Colour orange = juce::Colour::fromString("FFcd995a");

const juce::Colour black = juce::Colour::fromString("88000000");

const juce::Colour brown = juce::Colour::fromString("FFBC4749"); //red
const juce::Colour beige = juce::Colour::fromString("FFF2E8CF");
const juce::Colour paler_green = juce::Colour::fromString("FFA7C957");
const juce::Colour green = juce::Colour::fromString("FF6A994E");
const juce::Colour orange = juce::Colour::fromString("FF386641"); //darker green

const std::vector<juce::Colour> custom_colors = {orange, green, paler_green, beige, brown};

juce::Colour getGradient(float value);
juce::Colour getGradientWithoutGreen(float value);
}  // namespace CustomColors
