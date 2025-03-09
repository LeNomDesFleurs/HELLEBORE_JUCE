#include "BackgroundComponent.hpp"

void BackgroundComponent::paint(juce::Graphics& g) {
  float size_x = getParentWidth();
  float size_y = getParentHeight();

  g.fillAll(CustomColors::custom_colors[0]);
  g.setColour(CustomColors::black);
  juce::Path motif = paintMotif(size_x, size_y, 10);
  g.strokePath(motif, {0.5, PathStrokeType::curved, PathStrokeType::rounded});

 
  int left_margin = 20;
  int width = 260;
  int border_size = 1;
  int corner_size = 5;
  int half_width = 127;

  int sizes[5] = {125, 40, 250};
  // vertical position of the first block
  int y_position = 65;

  for (int i = 0; i < 3; i++) {
    g.setColour(CustomColors::black);

    int x = left_margin - border_size;
    int y = y_position - border_size;
    int w = width + border_size * 2;
    int h = sizes[i] + border_size * 2;
    if (i == 0){
    g.fillRoundedRectangle(x, y, half_width, h, corner_size);
    // x = left_margin - border_size
    g.fillRoundedRectangle(154, y, half_width, h, corner_size);
    }
    else {
    g.fillRoundedRectangle(x, y, w, h, corner_size);
    }

    if (i == 1) {
      g.setColour(CustomColors::dark_green);
    } else {
      g.setColour(CustomColors::light_green);
    }

    if(i==0){
      g.fillRoundedRectangle(left_margin, y_position, 125, sizes[i],
                           corner_size);
      g.fillRoundedRectangle(155, y_position, 125, sizes[i],
                           corner_size);
    //   g.fillRoundedRectangle(x+1, y+1, (w/2)-7, h-2, corner_size);
    // g.fillRoundedRectangle(x+w/2, y+1, (w/2)-5, h-2, corner_size);
    // g.fillRoundedRectangle(x, y, w, h, corner_size);
    // g.fillRoundedRectangle(x, y, w, h, corner_size);
    }
    else {
    g.fillRoundedRectangle(left_margin, y_position, width, sizes[i],
                           corner_size);
    }
    y_position += sizes[i];
    // vetical margin between block
    y_position += 10;
  }

  // Title color
  g.setColour(CustomColors::pink);
    g.setFont(juce::FontOptions("Times New Roman", 60.0f, juce::Font::italic));
  g.drawFittedText("Hellebore", 0, -10, 300, 80,
                   juce::Justification::centred, 1);

  // Draw Logo
  const auto svg = Drawable::createFromImageData(BinaryData::NOI_svg,
                                                 BinaryData::NOI_svgSize);

  // juce::AffineTransform scale = Set::scale(0.2);
  juce::Rectangle<float> position = {150.f - 36 / 2, size_y-45, 36.f, 36.f};
  juce::RectanglePlacement placement = (36);
  svg->setTransformToFit(position, placement);
  svg->draw(g, 1.0);
}

juce::Path BackgroundComponent::paintMotif(int size_x, int size_y,
                                           int block_size) {
  int void_centerx = 180;
  int void_centery = 370;

  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);

  juce::Path motif;
  for (int i = 0; i < size_x; i += block_size) {
    for (int j = 0; j < size_y; j += block_size) {
      int horizontal_distance = pow(void_centerx - i, 2);
      int vertical_distance = pow(void_centery - j, 2);
//      int total_distance = sqrt(horizontal_distance + vertical_distance);

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
