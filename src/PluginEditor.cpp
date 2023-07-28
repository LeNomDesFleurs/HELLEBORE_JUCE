/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelleboreAudioProcessorEditor::HelleboreAudioProcessorEditor(
    HelleboreAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    const float text_box_width = 50.0f;
    //------------------------------------------------------
    addAndMakeVisible(variationSlider);
    variationSlider.setLookAndFeel(&otherLookAndFeel);
    variationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    variationAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment
        (vts, "variation", variationSlider));
    variationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, text_box_width, 
        variationSlider.getTextBoxHeight());

    addAndMakeVisible(variationLabel);
    variationLabel.setText("variation",
        juce::dontSendNotification);
    //-------------------------------------------------------
    addAndMakeVisible(timeSlider);
    timeSlider.setLookAndFeel(&otherLookAndFeel);
    timeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    timeAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment
        (vts, "time", timeSlider));
    timeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, text_box_width, timeSlider.getTextBoxHeight());


    addAndMakeVisible(timeLabel);
    timeLabel.setText("time", juce::dontSendNotification);
    //-------------------------------------------------------
    addAndMakeVisible(combSizeSlider);
    combSizeSlider.setLookAndFeel(&otherLookAndFeel);
    combSizeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    combSizeAttachement.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment
        (vts, "comb_time", combSizeSlider));
    combSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, text_box_width, combSizeSlider.getTextBoxHeight());

    addAndMakeVisible(combSizeLabel);
    combSizeLabel.setText("Comb Size", juce::dontSendNotification);
    //-------------------------------------------------------
    addAndMakeVisible(dryWetSlider);
    dryWetSlider.setLookAndFeel(&otherLookAndFeel);
    dryWetSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryWetAttachement.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment
        (vts, "dry_wet", dryWetSlider));
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, text_box_width, dryWetSlider.getTextBoxHeight());


    addAndMakeVisible(dryWetLabel);
    dryWetLabel.setText("DryWet", juce::dontSendNotification);
    //--------------------------------------------------------
    addAndMakeVisible(freezeSlider);
    freezeSlider.setLookAndFeel(&otherLookAndFeel);
    freezeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    freezeAttachement.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment
        (vts, "freeze", freezeSlider));
    freezeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, text_box_width, freezeSlider.getTextBoxHeight());


    addAndMakeVisible(freezeLabel);
    freezeLabel.setText("freeze", juce::dontSendNotification);

    setSize(480, 190);
}

HelleboreAudioProcessorEditor::~HelleboreAudioProcessorEditor()
{
}

//==============================================================================
void HelleboreAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::whitesmoke);
    g.setFont(15.0f);

    g.drawFittedText("Hellebore", 0, 0, getWidth(), 30, juce::Justification::centred, 1);


}

void HelleboreAudioProcessorEditor::resized()
{
    const int marge_haute_slider = 60;

    variationSlider.setBounds({ 10, marge_haute_slider, 100, 100 });
    variationLabel.setBounds({ variationSlider.getX() ,
                                    variationSlider.getY() - 30,
                                    200, 50 });

    timeSlider.setBounds({ 100, marge_haute_slider, 100, 100 });
    timeLabel.setBounds({ timeSlider.getX() + 30,
                                    timeSlider.getY() - 30,
                                    200, 50 });

    combSizeSlider.setBounds({ 190, marge_haute_slider, 100, 100 });
    combSizeLabel.setBounds({ combSizeSlider.getX() + 10,
                                    combSizeSlider.getY() - 30,
                                    200, 50 });

    dryWetSlider.setBounds({ 275, marge_haute_slider, 100, 100 });
    dryWetLabel.setBounds({ dryWetSlider.getX() + 10,
                                    dryWetSlider.getY() - 30,
                                    200, 50 });

    freezeSlider.setBounds({ 360, marge_haute_slider, 100, 100 });
    freezeLabel.setBounds({ freezeSlider.getX() + 10,
                                    freezeSlider.getY() - 30,
                                    200, 50 });


    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
