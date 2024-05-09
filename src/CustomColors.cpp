#include "CustomColors.hpp"

/// @brief return a gradient that goes from custom green to custom red by going
/// trought yellow and orange
/// @param value
/// @return
juce::Colour CustomColors::getGradient(float value) {
  if (value < 0.33) {
    value *= 3;
    return CustomColors::custom_colors[1].interpolatedWith(CustomColors::custom_colors[2], value);
  } else if (value > 0.33 && value < 0.66) {
    value -= 0.33;
    value *= 3;
    return CustomColors::custom_colors[2].interpolatedWith(CustomColors::custom_colors[3],
    value);
  } else if (value > 0.66) {
    value -= 0.66;
    value *= 3;
    return CustomColors::custom_colors[3].interpolatedWith(CustomColors::custom_colors[4], value);
  }
  return CustomColors::custom_colors[4];
}

/// @brief Gradient without green to avoid diseapearing widget on the wrong
/// background
/// @param value
/// @return
juce::Colour CustomColors::getGradientWithoutGreen(float value) {
  if (value < 0.5) {
    value *= 2;
    return CustomColors::custom_colors[2].interpolatedWith(CustomColors::custom_colors[3],
    value);
  } else {
    value -= 0.5;
    value *= 2;
    return CustomColors::custom_colors[3].interpolatedWith(CustomColors::custom_colors[4], value);
  }
  return CustomColors::custom_colors[4];
}