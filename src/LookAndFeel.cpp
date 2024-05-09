#include "LookAndFeel.hpp"

void CentricKnob::drawRotarySlider(juce::Graphics& g, int x, int y, int width,
                                   int height, float sliderPos,
                                   const float rotaryStartAngle,
                                   const float rotaryEndAngle,
                                   juce::Slider& slider) {
  //! [locals]
  auto radius = (float)juce::jmin(width, height);
  // - 10.0f;
  auto centreX = (float)x + (float)width * 0.5f;
  auto centreY = (float)y + (float)height * 0.5f;
  auto rx = centreX - radius;
  auto ry = centreY - radius;
  auto rw = radius * 2.0f;
  auto angle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
  //! [locals]

  //! [outline]
  // fill
  // g.setColour(juce::Colours::whitesmoke);
  // g.fillEllipse(rx, ry, rw, rw);

  // outline
  // g.setColour(juce::Colours::black);
  // g.drawEllipse(rx, ry, rw, rw, 1.0f);
  //! [outline]

  //! [path]
  // juce::Path p;
  // auto pointerLength = radius * 0.33f;
  // auto pointerThickness = 7.0f;

  // p.addEllipse(-pointerThickness * 0.5f + 2.0f, -radius, pointerThickness,
  //              pointerThickness);
  // p.applyTransform(
  //     juce::AffineTransform::rotation(angle).translated(centreX, centreY));
  //! [path]

  juce::Path p;
  // sliderPos /= 1.6;
  p.addEllipse(centreX - (width * sliderPos ),
               centreY - (width * sliderPos ), width * sliderPos * 2,
               width * sliderPos * 2);
  //! [pointer]
  // pointer
  g.setColour(juce::Colours::white);
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
  if (slider.isBar()) {
    g.setColour(slider.findColour(Slider::trackColourId));
    g.fillRect(
        slider.isHorizontal()
            ? Rectangle<float>(static_cast<float>(x), (float)y + 0.5f,
                               sliderPos - (float)x, (float)height - 1.0f)
            : Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f,
                               (float)y + ((float)height - sliderPos)));
  } else {
    auto isTwoVal = (style == Slider::SliderStyle::TwoValueVertical ||
                     style == Slider::SliderStyle::TwoValueHorizontal);
    auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical ||
                       style == Slider::SliderStyle::ThreeValueHorizontal);

    auto trackWidth = jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f
                                                       : (float)width * 0.25f);

    Point<float> startPoint(
        slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
        slider.isHorizontal() ? (float)y + (float)height * 0.5f
                              : (float)(height + y));

    Point<float> endPoint(
        slider.isHorizontal() ? (float)(width + x) : startPoint.x,
        slider.isHorizontal() ? startPoint.y : (float)y);

    Path backgroundTrack;
    backgroundTrack.startNewSubPath(startPoint);
    backgroundTrack.lineTo(endPoint);
    g.setColour(slider.findColour(Slider::backgroundColourId));
    // g.strokePath(backgroundTrack,
    //              {trackWidth, PathStrokeType::curved,
    //              PathStrokeType::rounded});

    Path valueTrack;
    Point<float> minPoint, maxPoint, thumbPoint, controlPoint;

    if (isTwoVal || isThreeVal) {
      minPoint = {slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                  slider.isHorizontal() ? (float)height * 0.5f : minSliderPos};

      if (isThreeVal)
        thumbPoint = {slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                      slider.isHorizontal() ? (float)height * 0.5f : sliderPos};

      maxPoint = {slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                  slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos};
    } else {
      auto kx =
          slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
      auto ky =
          slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

      minPoint = startPoint;
      maxPoint = {kx, ky};
    }

    auto thumbWidth = getSliderThumbRadius(slider);

    valueTrack.startNewSubPath(minPoint);
    controlPoint = {static_cast<float>(width / 2),
                    static_cast<float>(height + 40)};
    // valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
    valueTrack.quadraticTo(controlPoint, isThreeVal ? thumbPoint : maxPoint);
    g.setColour(slider.findColour(Slider::trackColourId));
    g.strokePath(valueTrack,
                 {trackWidth, PathStrokeType::curved, PathStrokeType::rounded});

    if (!isTwoVal) {
      g.setColour(juce::Colours::green);
      g.fillEllipse(Rectangle<float>(static_cast<float>(thumbWidth),
                                     static_cast<float>(thumbWidth))
                        .withCentre(isThreeVal ? thumbPoint : maxPoint));
    }

    if (isTwoVal || isThreeVal) {
      auto sr =
          jmin(trackWidth,
               (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
      auto pointerColour = slider.findColour(Slider::thumbColourId);

      if (slider.isHorizontal()) {
        drawPointer(
            g, minSliderPos - sr,
            jmax(0.0f, (float)y + (float)height * 0.5f - trackWidth * 2.0f),
            trackWidth * 2.0f, pointerColour, 2);

        drawPointer(g, maxSliderPos - trackWidth,
                    jmin((float)(y + height) - trackWidth * 2.0f,
                         (float)y + (float)height * 0.5f),
                    trackWidth * 2.0f, pointerColour, 4);
      } else {
        drawPointer(
            g, jmax(0.0f, (float)x + (float)width * 0.5f - trackWidth * 2.0f),
            minSliderPos - trackWidth, trackWidth * 2.0f, pointerColour, 1);

        drawPointer(g,
                    jmin((float)(x + width) - trackWidth * 2.0f,
                         (float)x + (float)width * 0.5f),
                    maxSliderPos - sr, trackWidth * 2.0f, pointerColour, 3);
      }
    }
  }
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
  //! [locals]

  //! [outline]
  // fill
  // g.setColour(juce::Colours::whitesmoke);
  // g.fillEllipse(rx, ry, rw, rw);

  // outline
  // g.setColour(juce::Colours::black);
  // g.drawEllipse(rx, ry, rw, rw, 1.0f);
  //! [outline]

  //! [path]
  // juce::Path p;
  // auto pointerLength = radius * 0.33f;
  // auto pointerThickness = 7.0f;

  // p.addEllipse(-pointerThickness * 0.5f + 2.0f, -radius, pointerThickness,
  //              pointerThickness);
  // p.applyTransform(
  //     juce::AffineTransform::rotation(angle).translated(centreX, centreY));
  //! [path]

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