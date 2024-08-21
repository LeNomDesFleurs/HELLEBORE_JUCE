#include "LookAndFeel.hpp"

void CentricKnob::drawRotarySlider(juce::Graphics& g, int x, int y, int width,
                                   int height, float sliderPos,
                                   const float rotaryStartAngle,
                                   const float rotaryEndAngle,
                                   juce::Slider& slider) {

  float centreX = (float)x + (float)width * 0.5f;
  float centreY = (float)y + (float)height * 0.5f;

  sliderPos = std::pow(sliderPos, 4);
  sliderPos += 0.1;
  sliderPos *= 1.3;

  float diameter = static_cast<float>(width) * sliderPos;
  float radius = diameter / 2.f;



  juce::Path p;
  float circlex = centreX - radius;
  float circley = centreY - radius;
  float circlew = diameter;
  float circleh = diameter;
  p.addEllipse(circlex, circley, circlew, circleh);

  //warning, need linear value for color gradient
  g.setColour(CustomColors::getGradientWithoutGreen(sliderPos));
  Path vignette;
  vignette.addRoundedRectangle(0, 0, width, height, 5);

    g.reduceClipRegion(vignette);

  g.fillPath(p);
  //! [pointer]

}

void CentricKnob::drawLinearSlider(Graphics& g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float minSliderPos, float maxSliderPos,
                                   const Slider::SliderStyle style,
                                   Slider& slider) {
                                    
}

void RotationKnob::drawRotarySlider(juce::Graphics& g, int x, int y, int width,
                                    int height, float sliderPos,
                                    const float rotaryStartAngle,
                                    const float rotaryEndAngle,
                                    juce::Slider& slider) {
  //! [locals]
  auto radius = (float)juce::jmin(width / 2, height / 2) - 10.0f;
  auto centreX = (float)x + (float)width * 0.5f;
  auto centreY = (float)y + (float)height * 0.5f;
  auto rx = centreX - radius;
  auto ry = centreY - radius;
  auto rw = radius * 2.0f;
  auto angle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);


  juce::Path p;
  sliderPos /= 1.6;
  p.addRectangle(0 - width / 20, -height / 2., width / 10, height);
  p.applyTransform(juce::AffineTransform::rotation(sliderPos * 2)
                       .translated(centreX, centreY));
  //! [pointer]
  // pointer
  g.setColour(juce::Colours::white);
  g.fillPath(p);
  //! [pointer]

  juce::Path d;
  d.addRectangle(0 - width / 20, -height / 2., width / 10, height);
  d.applyTransform(juce::AffineTransform::rotation(-sliderPos * 2)
                       .translated(centreX, centreY));
  //! [pointer]
  // pointer
  g.setColour(juce::Colours::white);
  g.fillPath(d);
  //! [pointer]
}

void CentricKnob::drawToggleButton(Graphics& g, ToggleButton& button,
                                   bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown) {
  auto height = button.getHeight();
  auto width = button.getWidth();
  g.setColour(juce::Colours::white);
  if (button.getToggleState()) {
    // if on draw play button
    Path roof;
    roof.addTriangle(0, 0, 0, height, width, width / 2.);
    g.fillPath(roof);
  } else {
    // draw stop button
    juce::Rectangle<int> house(0, 0, height, width);
    g.fillRect(house);
  }
  // auto fontSize = jmin(15.0f, (float)button.getHeight() * 0.75f);
  // auto tickWidth = fontSize * 1.1f;

  // drawTickBox(g, button, 4.0f, ((float)button.getHeight() - tickWidth) *
  // 0.5f,
  //             tickWidth, tickWidth, button.getToggleState(),
  //             button.isEnabled(), shouldDrawButtonAsHighlighted,
  //             shouldDrawButtonAsDown);

  // g.setColour(button.findColour(ToggleButton::textColourId));
  // g.setFont(fontSize);

  // if (!button.isEnabled()) g.setOpacity(0.5f);

  // g.drawFittedText(button.getButtonText(),
  //                  button.getLocalBounds()
  //                      .withTrimmedLeft(roundToInt(tickWidth) + 10)
  //                      .withTrimmedRight(2),
  //                  Justification::centredLeft, 10);
}


void DryWetLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y,
                                        int width, int height, float sliderPos,
                                        const float rotaryStartAngle,
                                        const float rotaryEndAngle,
                                        juce::Slider& slider)
//! [drawRotarySlider]
{
  auto radius = (float)juce::jmin(width / 2, width / 2) - 10.0f;
  auto centreX = (float)width * (float)sliderPos * 0.9;
  auto centreY = (float)height/2.f;
  
  //! [locals]

  g.setColour(CustomColors::getGradient(sliderPos));

  // sliderPos /= 1.6;

  juce::Path d;
  d.addRoundedRectangle(-5, -(height-10)/2, 10, height-10, 5);
  d.applyTransform(juce::AffineTransform::rotation(sliderPos * 3.14)
                       .translated(centreX+5, centreY-5));
  // d.applyTransform(juce::AffineTransform::translation(width*sliderPos*0.9, 0));
  //! [pointer]
  // pointer
  g.fillPath(d);
  //! [pointer]
}