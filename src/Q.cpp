/*
  ==============================================================================

    Q.cpp
    Created: 3 Apr 2023 11:40:00am
    Author:  thoma

  ==============================================================================
*/

#include "Q.h"

/*
  ==============================================================================

    BandControl.cpp
    Created: 3 Apr 2023 9:15:51am
    Author:  thoma

  ==============================================================================
*/

#include "BandControl.h"


QComponent::QComponent(juce::AudioProcessorValueTreeState& apvts)
{
    
    QSlider.setLookAndFeel(&otherLookAndFeel);
    QSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    QAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, "RESONANCE", QSlider));
    QSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, QSlider.getTextBoxHeight());

    addAndMakeVisible(QLabel);
    QLabel.setText("Resonance", juce::dontSendNotification);
    QLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    addAndMakeVisible(QSlider);
}

void QComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    bounds.reduce(10, 10);
    g.setColour(back_ground_color);
    g.fillRoundedRectangle(bounds.toFloat(), 5);

}

void QComponent::resized() {

    auto bounds = getLocalBounds().reduced(5);

    using namespace juce;

    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.flexWrap = FlexBox::Wrap::noWrap;

    auto spacer = FlexItem().withWidth(4);
    auto endCap = FlexItem().withWidth(6);

    flexBox.items.add(endCap);
    flexBox.items.add(FlexItem(QSlider).withFlex(1.f));
    /*
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem().withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(*midHighXoverSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(*outGainSlider).withFlex(1.f));
    */
    flexBox.items.add(endCap);

    flexBox.performLayout(bounds);
}
