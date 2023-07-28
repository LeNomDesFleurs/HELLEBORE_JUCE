/*
  ==============================================================================

    MidiControl.cpp
    Created: 3 Apr 2023 9:16:17am
    Author:  thoma

  ==============================================================================
*/

#include "MidiControl.h"

MidiControl::MidiControl(juce::AudioProcessorValueTreeState& apvts)
{

    back_ground_color = juce::Colours::darkgrey;
    //SLIDER
    attackAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, "ATTACK", attackSlider));
    decayAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, "DECAY", decaySlider));
    cutoffFrequencyAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, "root_frequency", cutoffFrequencySlider));

    attackSlider.setLookAndFeel(&otherLookAndFeel);
    cutoffFrequencySlider.setLookAndFeel(&otherLookAndFeel);
    decaySlider.setLookAndFeel(&otherLookAndFeel);

    cutoffFrequencySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    decaySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);

    cutoffFrequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, cutoffFrequencySlider.getTextBoxHeight());
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, attackSlider.getTextBoxHeight());
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, decaySlider.getTextBoxHeight());

    addAndMakeVisible(cutoffFrequencySlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);

    //LABEL
    cutoffFrequencyLabel.setText("Root Frequency", juce::dontSendNotification);
    attackLabel.setText("Attack", juce::dontSendNotification);
    decayLabel.setText("Decay", juce::dontSendNotification);

    cutoffFrequencyLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    attackLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    decayLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    addAndMakeVisible(cutoffFrequencyLabel);
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);

    //BUTTON
    midiOffButtonAttachement.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, "MIDIOFF", midiOffButton));
    midiMonoButtonAttachement.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, "MIDIMONO", midiMonoButton));
    midiPolyButtonAttachement.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, "MIDIPOLY", midiPolyButton));

    midiMonoButton.setLookAndFeel(&otherLookAndFeel);
    midiPolyButton.setLookAndFeel(&otherLookAndFeel);
    midiOffButton.setLookAndFeel(&otherLookAndFeel);

    midiOffButton.setClickingTogglesState(true);
    midiMonoButton.setClickingTogglesState(true);
    midiPolyButton.setClickingTogglesState(true);

    addAndMakeVisible(midiOffButton);
    addAndMakeVisible(midiMonoButton);
    addAndMakeVisible(midiPolyButton);

    midiOffButton.setRadioGroupId(1);
    midiMonoButton.setRadioGroupId(1);
    midiPolyButton.setRadioGroupId(1);

  
}

void MidiControl::resized() {

    auto bounds = getLocalBounds().reduced(10);
    using namespace juce;

    auto createBandButtonControlBox = [](std::vector<Component*> comps)
    {
        FlexBox flexBox;
        flexBox.flexDirection = FlexBox::Direction::row;
        flexBox.flexWrap = FlexBox::Wrap::noWrap;
        //auto marge = FlexItem().withHeight(10);
        auto spacer = FlexItem().withHeight(2);
        //flexBox.items.add(marge);
        for (auto* comp : comps)
        {
            flexBox.items.add(spacer);
            
            flexBox.items.add(FlexItem(*comp).withFlex(1.f));
        }
        flexBox.items.add(spacer);
        //flexBox.items.add((marge));
        return flexBox;
    };

    auto midiModeControlBox = createBandButtonControlBox({ &midiOffButton, &midiMonoButton, &midiPolyButton });

    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::column;
    flexBox.flexWrap = FlexBox::Wrap::noWrap;

    auto spacer = FlexItem().withWidth(4);
    auto endCap = FlexItem().withWidth(6);
    
    flexBox.items.add(endCap);
    auto buttonFlexBox = FlexItem(midiModeControlBox).withFlex(0.5);
    buttonFlexBox.withMaxWidth(10);
    flexBox.items.add(buttonFlexBox);
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(cutoffFrequencySlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(attackSlider).withFlex(1.f));
    //flexBox.items.add(FlexItem(Envelope).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(decaySlider).withFlex(1.f));
    flexBox.items.remove(9);
    flexBox.items.add(spacer);
    flexBox.items.add(endCap);

    flexBox.performLayout(bounds);

}

void MidiControl::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    bounds.reduce(10, 10);
    g.setColour(back_ground_color);
    g.fillRoundedRectangle(bounds.toFloat(), 5);
}

