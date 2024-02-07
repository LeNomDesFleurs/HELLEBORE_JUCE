/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
HelleboreAudioProcessorEditor::HelleboreAudioProcessorEditor(
    HelleboreAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  const float text_box_width = 50.0f;
  // juce::Slider comps [4]= {variationSlider, timeSlider, combSizeSlider,
  // dryWetSlider};
  apvts = &vts;
  //------------------------------------------------------
  variationSlider.setLookAndFeel(&centricKnob);
  variationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  variationAttachment.reset(
      new juce::AudioProcessorValueTreeState::SliderAttachment(
          vts, "variation", variationSlider));
  variationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,
                                  text_box_width,
                                  variationSlider.getTextBoxHeight());

  variationLabel.setText("variation", juce::dontSendNotification);
  //-------------------------------------------------------
  timeSlider.setLookAndFeel(&centricKnob);
  timeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  timeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
      vts, "rt60", timeSlider));
  timeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, text_box_width,
                             timeSlider.getTextBoxHeight());

  timeLabel.setText("time", juce::dontSendNotification);
  //-------------------------------------------------------
  combSizeSlider.setLookAndFeel(&centricKnob);
  combSizeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  combSizeAttachement.reset(
      new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "comb_time",
                                                               combSizeSlider));
  combSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,
                                 text_box_width,
                                 combSizeSlider.getTextBoxHeight());

  combSizeLabel.setText("Comb Size", juce::dontSendNotification);
  //-------------------------------------------------------
  dryWetSlider.setLookAndFeel(&centricKnob);
  dryWetSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  dryWetAttachement.reset(
      new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "dry_wet",
                                                               dryWetSlider));
  dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,
                               text_box_width, dryWetSlider.getTextBoxHeight());

  dryWetLabel.setText("DryWet", juce::dontSendNotification);
  //--------------------------------------------------------
  freezeSlider.setLookAndFeel(&centricKnob);
  freezeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  freezeAttachement.reset(
      new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "freeze",
                                                               freezeSlider));
  freezeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,
                               text_box_width, freezeSlider.getTextBoxHeight());

  freezeLabel.setText("freeze", juce::dontSendNotification);

  for (auto* comp : getComps()) {
    addAndMakeVisible(comp);
    //   comp.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  }

  const auto& params = audioProcessor.getParameters();
  for (auto param : params) {
    param->addListener(this);
  }
  startTimerHz(30);
  setSize(480, 500);

  int random_order[12] = {1, 5, 8, 7, 11, 3, 12, 2, 10, 6, 9, 3};

  for (int i = 0; i < 12; i++) {
    float phase = i / 12.;
    lfos[random_order[i]].setPhase(phase);
  }
}

HelleboreAudioProcessorEditor::~HelleboreAudioProcessorEditor() {
  const auto& params = audioProcessor.getParameters();
  for (auto param : params) {
    param->removeListener(this);
  }
}

//==============================================================================
void HelleboreAudioProcessorEditor::paint(juce::Graphics& g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(juce::Colour(47, 46, 45));
  g.setColour(juce::Colour(89, 106, 55));
  g.setFont(15.0f);
  // g.
  float centerx = 240;
  float centery = 300;

  //   float rotation[12] =

  for (int i = 0; i < 12; i++) {
    float rotation = (cheappi / 12) * i;

    if (!freeze) {
      rotation_status = rotationLfo.getNextSample();
    }
    rotation += cheappi * rotation_status;
    float max_far = 10 + (i * 3);
    // if (i > 5) {
    //   max_far = -max_far;
    // }
    float far = max_far * variation * ((lfos[i].getNextSample()) * 2);

    float opp = std::sin(rotation) * far;
    float adj = std::cos(rotation) * far;

    opp += centerx;
    adj += centery;

    opp -= elipseSize / 2;
    adj -= elipseSize / 2;
    // sin theta * hypotenuse = Op
    // cos theta * hypo = adj

    g.drawEllipse(Rectangle<float>(opp, adj, elipseSize, elipseSize), 2.);
  }
  // g.drawEllipse(Rectangle<float>(elipseSize, elipseSize), 2.);
  g.drawFittedText("Hellebore", 0, 0, getWidth(), 30,
                   juce::Justification::centred, 1);
}

void HelleboreAudioProcessorEditor::resized() {
  const int marge_haute_slider = 60;

  variationSlider.setBounds({10, marge_haute_slider, 100, 100});
  variationLabel.setBounds(
      {variationSlider.getX(), variationSlider.getY() - 30, 200, 50});

  timeSlider.setBounds({100, marge_haute_slider, 100, 100});
  timeLabel.setBounds(
      {timeSlider.getX() + 30, timeSlider.getY() - 30, 200, 50});

  combSizeSlider.setBounds({190, marge_haute_slider, 100, 100});
  combSizeLabel.setBounds(
      {combSizeSlider.getX() + 10, combSizeSlider.getY() - 30, 200, 50});

  dryWetSlider.setBounds({275, marge_haute_slider, 100, 100});
  dryWetLabel.setBounds(
      {dryWetSlider.getX() + 10, dryWetSlider.getY() - 30, 200, 50});

  freezeSlider.setBounds({360, marge_haute_slider, 100, 100});
  freezeLabel.setBounds(
      {freezeSlider.getX() + 10, freezeSlider.getY() - 30, 200, 50});

  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

std::vector<juce::Component*> HelleboreAudioProcessorEditor::getComps() {
  return {
      &timeSlider,   &variationSlider, &freezeSlider,
      &dryWetSlider, &combSizeSlider,
  };
}

// void HelleboreAudioProcessorEditor::sliderValueChanged(Slider* slider){
//     if(slider == &timeSlider){
//       elipseSize = apvts->getRawParameterValue("rt60")->load();
//       resized();
//     }
// }

void HelleboreAudioProcessorEditor::parameterValueChanged(int parameterIndex,
                                                          float newValue) {
  parametersChanged.set(true);
  switch (parameterIndex) {
    case 2:
      elipseSize = newValue * 4 + 4;
      break;
    case 1:
      variation = newValue * 1.5;
      break;
    case 4:
      freeze = newValue > 0.5;
      break;
  }
}

void HelleboreAudioProcessorEditor::timerCallback() {
  if (parametersChanged.compareAndSetBool(false, true)) {
  }
  repaint();
}