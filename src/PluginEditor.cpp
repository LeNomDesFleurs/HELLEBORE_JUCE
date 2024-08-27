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
  combSizeSlider.setLookAndFeel(&empty_knob_look_and_feel);
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
  paintSizeWidget(g);
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
    case 0:
      dry_wet = newValue;
      break;
    case 2:
      size = newValue;
      break;
    case 1:
      variation = newValue;
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
  float distance = size*9.0+1;
  for (int i = 0; i < 12; i++) {
    float rotation = (cheappi / 12) * i;
    rotation += rotation_status;
    float max_far = 10 + (i * 1.6);

    float lfo;
    lfos[i].setFrequency(2/(distance));
    lfo = repaint_ui ? lfos[i].getNextSample() : lfos[i].getSample();

    float far = (lfo * 2) * cheappi;
    // max_far* pow(variation * 1.5, 2.0) * (lfo * 2);

    float elipseSize = size * 10 + 4;

    float opp = std::sin(rotation) * far;
    float adj = std::cos(rotation) * far;
    float x = (distance * cos(far)) - (elipseSize / 2.0);
    float y = (distance * sin(far)) - (elipseSize / 2.0);

    distance *= 1 + variation/4.;
    x += centerx;
    y += centery;
    opp += centerx;
    adj += centery;
    opp -= elipseSize / 2;
    adj -= elipseSize / 2;
    // sin theta * hypotenuse = Op
    // cos theta * hypo = adj
    juce::Colour colour = CustomColors::getGradientWithoutGreen(variation);
    colour = CustomColors::fadeToDefault(colour, dry_wet);
    g.setColour(colour);
    g.drawEllipse(Rectangle<float>(x, y, elipseSize, elipseSize), 2.);
  }
}

void HelleboreEditor::paintTimeWidget(juce::Graphics& g) {
  float centerx = 83;
  float centery = 128;
  float radius = 50;

  float bottom_offset = 0.08;
  float offset_time = time * (1.0 - bottom_offset) + bottom_offset;

  juce::Colour colour = CustomColors::getGradientWithoutGreen(time);
  colour = CustomColors::fadeToDefault(colour, dry_wet);
  g.setColour(colour);

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

void HelleboreEditor::paintSizeWidget(juce::Graphics& g) {
  int x = 155;
  int y = 65;
  int width = 125;
  int height = 125;

  float centreX = (float)x + (float)width * 0.5f;
  float centreY = (float)y + (float)height * 0.5f;

  float sliderPos = size;
  sliderPos = std::pow(sliderPos, 4);
  sliderPos += 0.1;
  sliderPos *= 1.3;

  float diameter = static_cast<float>(width) * sliderPos;
  float radius = diameter / 2.f;

  juce::Path p;
  float circlex = centreX - radius;
  float circley = centreY - radius;
  float circlew = diameter;
  float circleh = diameter;
  p.addEllipse(circlex, circley, circlew, circleh);

  // warning, need linear value for color gradient
  juce::Colour colour = CustomColors::getGradientWithoutGreen(sliderPos);
  colour = CustomColors::fadeToDefault(colour, dry_wet);
  g.setColour(colour);

  Path vignette;
  vignette.addRoundedRectangle(x, y, width, height, 5);

  g.reduceClipRegion(vignette);

  g.fillPath(p);
  //! [pointer]
}