/*
  ==============================================================================

    MidiControl.h
    Created: 3 Apr 2023 9:16:17am
    Author:  thoma

  ==============================================================================
*/

#pragma once


#include "JuceHeader.h"
#include "LookAndFeel.h"

struct MidiControl : juce::Component {

    MidiControl(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void resized() override;
    juce::Colour back_ground_color;

private:
    juce::Component Frequency;
    juce::Component Envelope;
    OtherLookAndFeel otherLookAndFeel;
    //SLIDER
    juce::Slider cutoffFrequencySlider, attackSlider, decaySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        cutoffFrequencyAttachement, attackAttachement, decayAttachement;
    juce::Label cutoffFrequencyLabel, attackLabel, decayLabel;
    //BUTTON
    juce::TextButton midiOffButton{ "Off" }, midiMonoButton{ "Mono" }, midiPolyButton{ "Poly" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        midiOffButtonAttachement, midiMonoButtonAttachement, midiPolyButtonAttachement;
};