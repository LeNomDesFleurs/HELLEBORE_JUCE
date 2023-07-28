/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 3 Apr 2023 9:49:06am
    Author:  thoma

  ==============================================================================
*/

#include "LookAndFeel.h"

    OtherLookAndFeel::OtherLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::red);
    }
    //! [otherLookAndFeel]

    //! [drawRotarySlider]
    void OtherLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
        //! [drawRotarySlider]
    {
        //! [locals]
        auto outline = juce::Colours::black;
        auto fill = juce::Colours::lightgrey;

        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);

        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin(1.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);

        g.setColour(outline);
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(),
                bounds.getCentreY(),
                arcRadius,
                arcRadius,
                0.0f,
                rotaryStartAngle,
                toAngle,
                true);

            g.setColour(fill);
            g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        //auto thumbWidth = lineW * 2.0f;
        /*juce::Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - juce::MathConstants<float>::halfPi),
            bounds.getCentreY() + arcRadius * std::sin(toAngle - juce::MathConstants<float>::halfPi));*/

        g.setColour(slider.findColour(juce::Slider::thumbColourId));
        // g.fillEllipse(juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
    }


    void OtherLookAndFeel::drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& toggleButton,
        bool /*shouldDrawButtonAsHighlighted*/,
        bool /*shouldDrawButtonAsDown*/)
    {
        using namespace juce;
        /*
        if (auto* pb = dynamic_cast<PowerButton*>(&toggleButton))
        {
            Path powerButton;

            auto bounds = toggleButton.getLocalBounds();

            auto size = jmin(bounds.getWidth(), bounds.getHeight()) - 6;
            auto r = bounds.withSizeKeepingCentre(size, size).toFloat();

            float ang = 30.f; //30.f;

            size -= 6;

            powerButton.addCentredArc(r.getCentreX(),
                r.getCentreY(),
                size * 0.5f,
                size * 0.5f,
                0.f,
                degreesToRadians(ang),
                degreesToRadians(360.f - ang),
                true);

            powerButton.startNewSubPath(r.getCentreX(), r.getY());
            powerButton.lineTo(r.getCentre());

            PathStrokeType pst(2.f, PathStrokeType::JointStyle::curved);

            auto color = toggleButton.getToggleState() ? Colours::dimgrey : ColorScheme::getSliderRangeTextColor();

            g.setColour(color);
            g.strokePath(powerButton, pst);
            g.drawEllipse(r, 2);
        }
        else if (auto* analyzerButton = dynamic_cast<AnalyzerButton*>(&toggleButton))
        {
            auto color = !toggleButton.getToggleState() ? Colours::dimgrey : Colour(0u, 172u, 1u);

            g.setColour(color);

            auto bounds = toggleButton.getLocalBounds();
            g.drawRect(bounds);

            g.strokePath(analyzerButton->randomPath, PathStrokeType(1.f));
        }
        else
        {*/
            auto bounds = toggleButton.getLocalBounds().reduced(2);

            auto buttonIsOn = toggleButton.getToggleState();

            const int cornerSize = 4;

            g.setColour(buttonIsOn ?
                toggleButton.findColour(TextButton::ColourIds::buttonOnColourId) :
                toggleButton.findColour(TextButton::ColourIds::buttonColourId));
            g.fillRoundedRectangle(bounds.toFloat(), cornerSize);

            g.setColour(buttonIsOn ? Colours::white : juce::Colour(0xff2c92ca));
            g.drawRoundedRectangle(bounds.toFloat(), cornerSize, 1);
            g.setColour(buttonIsOn ? Colours::black : juce::Colour(0xff2c92ca));
            g.drawFittedText(toggleButton.getName(), bounds, Justification::centred, 1);
       // }
    }