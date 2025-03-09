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


void DryWetLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y,
                                        int width, int height, float sliderPos,
                                        const float rotaryStartAngle,
                                        const float rotaryEndAngle,
                                        juce::Slider& slider)
//! [drawRotarySlider]
{
//  auto radius = (float)juce::jmin(width / 2, width / 2) - 10.0f;
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
