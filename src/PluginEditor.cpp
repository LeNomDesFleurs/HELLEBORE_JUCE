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
  variationSlider.setLookAndFeel(&rotationKnob);
  variationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  variationAttachment.reset(
      new juce::AudioProcessorValueTreeState::SliderAttachment(
          vts, "variation", variationSlider));
  variationSlider.setTextBoxStyle(juce::Slider::NoTextBox, false,
                                  text_box_width,
                                  variationSlider.getTextBoxHeight());

  variationLabel.setText("variation", juce::dontSendNotification);
  //-------------------------------------------------------
  timeSlider.setLookAndFeel(&centricKnob);
  timeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  timeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
      vts, "rt60", timeSlider));
  timeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, text_box_width,
                             timeSlider.getTextBoxHeight());

  timeLabel.setText("time", juce::dontSendNotification);
  //-------------------------------------------------------
  combSizeSlider.setLookAndFeel(&centricKnob);
  combSizeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  combSizeAttachement.reset(
      new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "comb_time",
                                                               combSizeSlider));
  combSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, text_box_width,
                                 combSizeSlider.getTextBoxHeight());

  combSizeLabel.setText("Comb Size", juce::dontSendNotification);
  //-------------------------------------------------------
  dryWetSlider.setLookAndFeel(&drywet_look_and_feel);
  dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
  dryWetAttachement.reset(
      new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "dry_wet",
                                                               dryWetSlider));
  dryWetSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, text_box_width,
                               dryWetSlider.getTextBoxHeight());

  dryWetLabel.setText("DryWet", juce::dontSendNotification);
  //--------------------------------------------------------
  freezeSlider.setLookAndFeel(&centricKnob);
  // freezeSlider.setSliderStyle(juce::Button);
  freezeAttachement.reset(
      new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "freeze",
                                                               freezeSlider));
  // freezeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false,
  //                              text_box_width,
  //                              freezeSlider.getTextBoxHeight());

  freezeLabel.setText("freeze", juce::dontSendNotification);

  addAndMakeVisible(background_component);

  for (auto* comp : getComps()) {
    addAndMakeVisible(comp);
    //   comp.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  }
  addAndMakeVisible(openGLcomponent);
  addAndMakeVisible(demo2D);

  const auto& params = audioProcessor.getParameters();
  for (auto param : params) {
    param->addListener(this);
  }
  startTimerHz(30);
  setSize(350, 550);

  int random_order[12] = {1, 5, 8, 7, 11, 3, 12, 2, 10, 6, 9, 3};

  for (int i = 0; i < 12; i++) {
    float phase = i / 12.;
    lfos[random_order[i]].setPhase(phase);
  }

  background_component.setBufferedToImage(true);
}

HelleboreAudioProcessorEditor::~HelleboreAudioProcessorEditor() {
  const auto& params = audioProcessor.getParameters();
  for (auto param : params) {
    param->removeListener(this);
  }
}

//==============================================================================
void HelleboreAudioProcessorEditor::paintOverChildren(juce::Graphics& g) {
  // (Our component is opaque, so we must completely fill the background with a

  // g.
  float centerx = this->width / 2;
  float centery = 370;

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
}

void HelleboreAudioProcessorEditor::resized() {
  background_component.setBounds(0, 0, 350, 550);

  const int marge_haute_slider = 100;

  combSizeSlider.setBounds({this->width / 4 - 30, marge_haute_slider, 100, 60});
  // combSizeLabel.setBounds(
  //     {combSizeSlider.getX() + 10, combSizeSlider.getY() - 30, 200, 50});

  freezeSlider.setBounds(
      {(this->width / 2) - 15, marge_haute_slider + 30 - 15, 30, 30});
  // freezeLabel.setBounds(
  //     {freezeSlider.getX() + 10, freezeSlider.getY() - 30, 200, 50});

  variationSlider.setBounds(
      {(this->width / 4) * 3 - 30, marge_haute_slider, 60, 60});
  // variationLabel.setBounds(
  //     {variationSlider.getX(), variationSlider.getY() - 30, 200, 50});

  timeSlider.setBounds({100, 200, 300, 25});
  // timeLabel.setBounds(
  //     {timeSlider.getX() + 30, timeSlider.getY() - 30, 200, 50});

  dryWetSlider.setBounds({20, 60, 310, 25});

  demo2D.setBounds(0, 0, 200, 200);
  // dryWetLabel.setBounds(
  //     {dryWetSlider.getX() + 10, dryWetSlider.getY() - 30, 200, 50});

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
      elipseSize = newValue * 10 + 4;
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