/*
  ==============================================================================

    Q.h
    Created: 3 Apr 2023 11:40:00am
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "LookAndFeel.h"

struct QComponent : juce::Component {

    QComponent(juce::AudioProcessorValueTreeState& apvts);

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Colour back_ground_color;

private:
    OtherLookAndFeel otherLookAndFeel;

    juce::Slider QSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>QAttachement;
    juce::Label QLabel;


};