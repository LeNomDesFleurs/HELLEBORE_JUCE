#include "CustomColors.hpp"

/// @brief return a gradient that goes from custom light_green to custom red by
/// going trought yellow and dark_green
/// @param value
/// @return
juce::Colour CustomColors::getGradient(float value) {
  if (value < 0.33) {
    value *= 3;
    return CustomColors::custom_colors[1].interpolatedWith(
        CustomColors::custom_colors[2], value);
  } else if (value > 0.33 && value < 0.66) {
    value -= 0.33;
    value *= 3;
    return CustomColors::custom_colors[2].interpolatedWith(
        CustomColors::custom_colors[3], value);
  } else if (value > 0.66) {
    value -= 0.66;
    value *= 3;
    return CustomColors::custom_colors[3].interpolatedWith(
        CustomColors::custom_colors[4], value);
  }
  return CustomColors::custom_colors[4];
}

/// @brief Gradient without light_green to avoid diseapearing widget on the
/// wrong background
/// @param value
/// @return
juce::Colour CustomColors::getGradientWithoutGreen(float value) {
  if (value < 0.5) {
    value *= 2;
    return CustomColors::custom_colors[2].interpolatedWith(
        CustomColors::custom_colors[3], value);
  } else {
    value -= 0.5;
    value *= 2;
    return CustomColors::custom_colors[3].interpolatedWith(
        CustomColors::custom_colors[4], value);
  }
  return CustomColors::custom_colors[4];
}

/// @brief get a colour and fade it to the background color, 0 if fully faded, 1 is unchanged input color
juce::Colour CustomColors::fadeToDefault(juce::Colour colour, float value)
{
  return CustomColors::custom_colors[0].interpolatedWith(colour, value);
}