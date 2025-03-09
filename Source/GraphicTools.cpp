#include "GraphicTools.hpp"

void GraphicTools::addLine(juce::Path& path, int initial_x, int initial_y,
                           int destination_x, int destination_y) {
  float f_initial_x, f_initial_y, f_destination_x, f_destination_y;
  f_initial_x = static_cast<float>(initial_x);
  f_initial_y = static_cast<float>(initial_y);
  f_destination_x = static_cast<float>(destination_x);
  f_destination_y = static_cast<float>(destination_y);

  juce::Point<float> begin{f_initial_x, f_initial_y};
  juce::Point<float> end{f_destination_x, f_destination_y};

  path.startNewSubPath(begin);
  path.lineTo(end);
}