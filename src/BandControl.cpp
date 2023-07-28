/*
  ==============================================================================

    BandControl.cpp
    Created: 3 Apr 2023 9:15:51am
    Author:  thoma

  ==============================================================================
*/

#include "BandControl.h"


BandControl::BandControl(juce::AudioProcessorValueTreeState& apvts)
{
    
    ratioSlider.setLookAndFeel(&otherLookAndFeel);
    BandSelectorSlider.setLookAndFeel(&otherLookAndFeel);

    ratioSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    BandSelectorSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);

    ratioAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, "RATIO", ratioSlider));
    BandSelectorAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, "band_selector", BandSelectorSlider));

    ratioSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, ratioSlider.getTextBoxHeight());
    BandSelectorSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0,  BandSelectorSlider.getTextBoxHeight());

    BandSelectorLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    ratioLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    BandSelectorLabel.setText("Band Selector", juce::dontSendNotification);
    ratioLabel.setText("Ratio", juce::dontSendNotification);


    back_ground_color = juce::Colours::darkgrey;


    addAndMakeVisible(ratioSlider);
    addAndMakeVisible(ratioLabel);
    addAndMakeVisible(BandSelectorSlider);
    addAndMakeVisible(BandSelectorLabel);

    lowHighButtonAttachement.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, "LOWHIGH", lowHighButton));
    oddEvenButtonAttachement.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, "ODDEVEN", oddEvenButton));
    peakButtonAttachement.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, "PEAK", peakButton));

    peakButton.setLookAndFeel(&otherLookAndFeel);
    lowHighButton.setLookAndFeel(&otherLookAndFeel);
    oddEvenButton.setLookAndFeel(&otherLookAndFeel);

    lowHighButton.setClickingTogglesState(true);
    oddEvenButton.setClickingTogglesState(true);
    peakButton.setClickingTogglesState(true);

    addAndMakeVisible(lowHighButton);
    addAndMakeVisible(oddEvenButton);
    addAndMakeVisible(peakButton);

    lowHighButton.setRadioGroupId(2);
    oddEvenButton.setRadioGroupId(2);
    peakButton.setRadioGroupId(2);
}

void BandControl::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    bounds.reduce(10, 10);
    g.setColour(back_ground_color);
    g.fillRoundedRectangle(bounds.toFloat(), 5);

}

void BandControl::resized() {

    auto bounds = getLocalBounds().reduced(20);
    
    using namespace juce;

    auto createBandButtonControlBox = [](std::vector<Component*> comps)
    {
        FlexBox flexBox;
        flexBox.flexDirection = FlexBox::Direction::column;
        flexBox.flexWrap = FlexBox::Wrap::noWrap;
        auto marge = FlexItem().withHeight(2);
        auto spacer = FlexItem().withHeight(2);
        flexBox.items.add(marge);
        for (auto* comp : comps)
        {
           // flexBox.items.add(spacer);
            flexBox.items.add(FlexItem(*comp).withFlex(1.f));
        }

        flexBox.items.add(spacer);
        flexBox.items.add((marge));

        return flexBox;
    };

    auto bandModeControlBox = createBandButtonControlBox({ &lowHighButton, &oddEvenButton, &peakButton });


    FlexBox flexBox;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.flexWrap = FlexBox::Wrap::noWrap;

    auto spacer = FlexItem().withWidth(4);
    auto endCap = FlexItem().withWidth(6);

    flexBox.items.add(endCap);
    flexBox.items.add(FlexItem(ratioSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(BandSelectorSlider).withFlex(1.f));
    flexBox.items.add(spacer);
    flexBox.items.add(FlexItem(bandModeControlBox).withFlex(0.5f));
    //flexBox.items.add(FlexItem(bandModeControlBox).withWidth(100));
    flexBox.items.add(spacer);
    flexBox.items.add(endCap);

    flexBox.performLayout(bounds);
}