#include "BackgroundComponent.hpp"

void BackgroundComponent::paint(juce::Graphics& g) {
  g.fillAll(CustomColors::custom_colors[0]);
  g.setColour(CustomColors::black);
  juce::Path motif = paintMotif(350, 550, 10);
  g.strokePath(motif, {0.5, PathStrokeType::curved, PathStrokeType::rounded});

  int left_margin = 20;
  int right_margin = 20;
  int width = getLocalBounds().getWidth();
  width -= left_margin + right_margin;
  // .removeFromLeft(left_margin).removeFromRight(right_margin).getWidth();
  // int width = 260;
  int border_size = 1;
  int corner_size = 5;

  int sizes[4] = {100, 80, 80, 100};
  // vertical position of the first block
  int y_position = 75;

  for (int i = 0; i < 4; i++) {
    g.setColour(CustomColors::black);

    int x = left_margin - border_size;
    int y = y_position - border_size;
    int w = width + border_size * 2;
    int h = sizes[i] + border_size * 2;
    g.fillRoundedRectangle(x, y, w, h, corner_size);

    if (i == 0 || i == 3) {
      g.setColour(CustomColors::green);

    } else {
      g.setColour(CustomColors::brown);
    }

    g.fillRoundedRectangle(left_margin, y_position, width, sizes[i],
                           corner_size);

    y_position += sizes[i];
    // vetical margin between block
    y_position += 10;
  }

  // Title color
  g.setColour(CustomColors::green);
  g.setFont(juce::Font("Times New Roman", 60.0f, juce::Font::italic));
  g.drawFittedText("Hellebore", 0, -10, getWidth(), 80,
                   juce::Justification::centred, 1);
}

juce::Path BackgroundComponent::paintMotif(int size_x, int size_y,
                                           int block_size) {
  int void_centerx = 180;
  int void_centery = 370;

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);
  

  juce::Path motif;
  for (int i = 0; i < size_x; i += block_size) {
    for (int j = 0; j < size_y; j += block_size) {
      int horizontal_distance = pow(void_centerx - i, 2);
      int vertical_distance = pow(void_centery - j, 2);
      int total_distance = sqrt(horizontal_distance + vertical_distance);

      // if (total_distance > 150) {
        if (dis(gen) > 0.5) {
          GraphicTools::addLine(motif, i + block_size, j, i, j);
        } else {
          GraphicTools::addLine(motif, i, j, i + block_size, j + block_size);
        }
      // }
    }
  }
  return motif;
}