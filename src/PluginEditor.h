/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::red);
    }
    //! [otherLookAndFeel]

    //! [drawRotarySlider]
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
        //! [drawRotarySlider]
    {
        //! [locals]
        auto radius = (float)juce::jmin(width / 2, height / 2) - 10.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        //! [locals]

        //! [outline]
                // fill
        //g.setColour(juce::Colours::whitesmoke);
        //g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colours::black);
        g.drawEllipse(rx, ry, rw, rw, 15.0f);
        //! [outline]

        //! [path]
        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 7.0f;

        p.addEllipse(-pointerThickness * 0.5f + 2.0f, -radius, pointerThickness, pointerThickness);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        //! [path]

        //! [pointer]
                // pointer
        g.setColour(juce::Colours::white);
        g.fillPath(p);
        //! [pointer]
    }
};
//==============================================================================
/**
*/
class HelleboreAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HelleboreAudioProcessorEditor (HelleboreAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~HelleboreAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HelleboreAudioProcessor& audioProcessor;

    OtherLookAndFeel otherLookAndFeel; // [2]

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.


    juce::Slider variationSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        variationAttachment;
    juce::Label variationLabel;

    juce::Slider timeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        timeAttachment;
    juce::Label timeLabel;

    juce::Slider combSizeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        combSizeAttachement;
    juce::Label combSizeLabel;

    juce::Slider dryWetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        dryWetAttachement;
    juce::Label dryWetLabel;

    juce::Slider freezeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        freezeAttachement;
    juce::Label freezeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelleboreAudioProcessorEditor)
};
