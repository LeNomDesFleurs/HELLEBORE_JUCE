/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
HelleboreEditor::HelleboreEditor(HelleboreAudioProcessor& p,
                                 juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  const float text_box_width = 50.0f;
  // juce::Slider comps [4]= {variationSlider, timeSlider, combSizeSlider,
  // dryWetSlider};
  apvts = &vts;

  for (auto* comp : getComps()) {
    comp->setTextBoxStyle(juce::Slider::NoTextBox, false, text_box_width,
                          variationSlider.getTextBoxHeight());
    //   comp.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  }
  //------------------------------------------------------

  variationSlider.setLookAndFeel(&empty_knob_look_and_feel);
  timeSlider.setLookAndFeel(&empty_knob_look_and_feel);
  combSizeSlider.setLookAndFeel(&centricKnob);
  dryWetSlider.setLookAndFeel(&drywet_look_and_feel);

  timeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  combSizeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  variationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalDrag);

  using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

  variationAttachment.reset(new Attachment(vts, "variation", variationSlider));
  timeAttachment.reset(new Attachment(vts, "rt60", timeSlider));
  combSizeAttachement.reset(new Attachment(vts, "comb_time", combSizeSlider));
  dryWetAttachement.reset(new Attachment(vts, "dry_wet", dryWetSlider));

  // Background BEFORE widgets
  addAndMakeVisible(background_component);

  for (auto* comp : getComps()) {
    addAndMakeVisible(comp);
  }

  const auto& params = audioProcessor.getParameters();
  for (auto param : params) {
    param->addListener(this);
  }

  startTimerHz(30);
  setSize(300, 560);

  // prepare lfos for the variation widget
  int random_order[12] = {1, 5, 8, 7, 11, 3, 12, 2, 10, 6, 9, 3};

  for (int i = 0; i < 12; i++) {
    float phase = i / 12.;
    lfos[random_order[i]].setPhase(phase);
  }

  // only paint background once
  background_component.setBufferedToImage(true);
}

HelleboreEditor::~HelleboreEditor() {
  const auto& params = audioProcessor.getParameters();
  for (auto param : params) {
    param->removeListener(this);
  }
}

//==============================================================================
void HelleboreEditor::paintOverChildren(juce::Graphics& g) {
  if (repaint_ui == true) {
    if (!freeze) {
      rotation_status = rotationLfo.getNextSample() * cheappi;
    }
  }
  paintVariationWidget(g);
  paintTimeWidget(g);
  repaint_ui = false;
}

void HelleboreEditor::resized() {
  background_component.setBounds(getBounds());

  combSizeSlider.setBounds({155, 65, 125, 125});
  variationSlider.setBounds({25, 250, 250, 250});
  timeSlider.setBounds({20, 65, 125, 125});
  dryWetSlider.setBounds({25, 205, 265, 40});
}

std::vector<juce::Slider*> HelleboreEditor::getComps() {
  return {
      &timeSlider,
      &variationSlider,
      &dryWetSlider,
      &combSizeSlider,
  };
}

void HelleboreEditor::parameterValueChanged(int parameterIndex,
                                            float newValue) {
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
    case 3:
      time = newValue;
      freeze = time >= 1.0;
      break;
  }
}

void HelleboreEditor::timerCallback() {
  repaint_ui = true;
  repaint();
}

void HelleboreEditor::paintVariationWidget(juce::Graphics& g) {
  // g.
  float centerx = 150;
  float centery = 375;

  //   float rotation[12] =

  for (int i = 0; i < 12; i++) {
    float rotation = (cheappi / 12) * i;
    rotation += rotation_status;
    float max_far = 10 + (i * 1.6);

    float lfo;
    lfo = repaint_ui ? lfos[i].getNextSample() : lfos[i].getSample();

    float far = max_far * pow(variation, 2.0) * (lfo * 2);

    float opp = std::sin(rotation) * far;
    float adj = std::cos(rotation) * far;

    opp += centerx;
    adj += centery;

    opp -= elipseSize / 2;
    adj -= elipseSize / 2;
    // sin theta * hypotenuse = Op
    // cos theta * hypo = adj
    g.setColour(CustomColors::dark_green);
    g.drawEllipse(Rectangle<float>(opp, adj, elipseSize, elipseSize), 2.);
  }
}

void HelleboreEditor::paintTimeWidget(juce::Graphics& g) {
  float centerx = 83;
  float centery = 128;
  float radius = 50;

  float bottom_offset = 0.08;
  float offset_time = time * (1.0 - bottom_offset) + bottom_offset;

  g.setColour(CustomColors::getGradientWithoutGreen(time));

  float radian_goal = cheappi * offset_time;

  juce::Path p;
  p.addCentredArc(centerx, centery, radius, radius, this->rotation_status, 0.0,
                  radian_goal, true);
  p.addCentredArc(centerx, centery, radius, radius,
                  this->rotation_status + cheappi, 0.0, radian_goal, true);
  juce::PathStrokeType stroke =
      PathStrokeType(2.0, PathStrokeType::curved, PathStrokeType::rounded);

  // the arrow disapear when the circle become complete
  float distance_complete_circle = pow(1.0 - offset_time, 0.3);
  float arrow_width = distance_complete_circle * 6.0;
  float arrow_height = distance_complete_circle * 10.0;

  stroke.PathStrokeType::createStrokeWithArrowheads(p, p, 0.0, 0.0, arrow_width,
                                                    arrow_height);

  g.strokePath(p, stroke);
}