#include "LookAndFeel.hpp"


void CentricKnob::drawRotarySlider(juce::Graphics& g, int x, int y, int width,
                              int height, float sliderPos,
                              const float rotaryStartAngle,
                              const float rotaryEndAngle, juce::Slider&) {
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
    g.setColour(juce::Colours::black);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);
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
    p.addEllipse(centreX - (width * sliderPos / 2),
                 centreY - (width * sliderPos / 2), width * sliderPos,
                 width * sliderPos);
    //! [pointer]
    // pointer
    g.setColour(juce::Colours::white);
    g.fillPath(p);
    //! [pointer]
}