/*
  ==============================================================================

    BandControl.h
    Created: 3 Apr 2023 9:15:51am
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "LookAndFeel.h"

struct BandControl : juce::Component {

    BandControl(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void resized() override;
    juce::Colour back_ground_color;

private:
    OtherLookAndFeel otherLookAndFeel;
    //SLIDER
    juce::Slider BandSelectorSlider, ratioSlider;
    juce::Label BandSelectorLabel, ratioLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        BandSelectorAttachement, ratioAttachement;
    //BUTTON
    juce::TextButton lowHighButton{ "Low | High" }, oddEvenButton{ "Odd | Even" }, peakButton{ "Peak" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        lowHighButtonAttachement, oddEvenButtonAttachement, peakButtonAttachement;
};