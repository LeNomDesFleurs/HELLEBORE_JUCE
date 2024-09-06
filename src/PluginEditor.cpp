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
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      variationWidgetLfos{{noi::Outils::LFO(60, 0.15f), noi::Outils::LFO(60, 0.15f),
                           noi::Outils::LFO(60, 0.15f), noi::Outils::LFO(60, 0.15f),
                           noi::Outils::LFO(60, 0.15f), noi::Outils::LFO(60, 0.15f),
                           noi::Outils::LFO(60, 0.15f), noi::Outils::LFO(60, 0.15f),
                           noi::Outils::LFO(60, 0.15f), noi::Outils::LFO(60, 0.15f),
                           noi::Outils::LFO(60, 0.15f),
                           noi::Outils::LFO(60, 0.15f)}},
      timeWidgetLfo(60.f, 0.2),
      apvts{&vts} {

  const float text_box_width = 50.0f;
  for (auto* comp : getComps()) {
    comp->setTextBoxStyle(juce::Slider::NoTextBox, false, text_box_width,
                          variationSlider.getTextBoxHeight());
  }
  //------------------------------------------------------

  variationSlider.setLookAndFeel(&empty_knob_look_and_feel);
  feedbackSlider.setLookAndFeel(&empty_knob_look_and_feel);
  combSizeSlider.setLookAndFeel(&empty_knob_look_and_feel);
  dryWetSlider.setLookAndFeel(&drywet_look_and_feel);

  feedbackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  combSizeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  variationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalDrag);

  using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

  variationAttachment.reset(new Attachment(vts, "variation", variationSlider));
  feedbackAttachment.reset(new Attachment(vts, "feedback", feedbackSlider));
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
  int random_order[12] = {1, 5, 8, 7, 11, 4, 0, 2, 10, 6, 9, 3};

  for (int i = 0; i < 12; i++) {
    float phase = i / 12.;
    variationWidgetLfos[random_order[i]].setPhase(phase);
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
  dry_wet = apvts->getRawParameterValue("dry_wet")->load();
  size = apvts->getRawParameterValue("comb_time")->load();
  variation = apvts->getRawParameterValue("variation")->load();
  feedback = apvts->getRawParameterValue("feedback")->load();
  freeze = feedback >= 1.0;
  feedback = pow(feedback, 2);
  size = size / 3.9f;

  if (repaint_ui == true) {
    if (!freeze) {
      rotation_status = timeWidgetLfo.getNextSample() * cheappi;
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
  feedbackSlider.setBounds({20, 65, 125, 125});
  dryWetSlider.setBounds({25, 205, 265, 40});
}

std::vector<juce::Slider*> HelleboreEditor::getComps() {
  return {
      &feedbackSlider,
      &variationSlider,
      &dryWetSlider,
      &combSizeSlider,
  };
}

void HelleboreEditor::parameterValueChanged(int parameterIndex,
                                            float newValue) {
  // switch (parameterIndex) {
  //   case 0:
  //     dry_wet = newValue;
  //     break;
  //   case 2:
  //     size = newValue;
  //     break;
  //   case 1:
  //     variation = newValue;
  //     break;
  //   case 4:
  //     freeze = newValue > 0.5;
  //     break;
  //   case 3:
  //     feedback = newValue;
  //     freeze = feedback >= 1.0;
  //     break;
  // }
}

void HelleboreEditor::timerCallback() {
  repaint_ui = true;
  repaint();
}

void HelleboreEditor::paintVariationWidget(juce::Graphics& g) {
  // g.
  float centerx = 150;
  float centery = 375;

  juce::Colour colour = CustomColors::getGradientWithoutGreen(variation);
  colour = CustomColors::fadeToDefault(colour, dry_wet);
  g.setColour(colour);

  //   float rotation[12] =
  float max_base_distance = 60;
  float max_size = 4;
  float base_distance = (size + 0.01) * max_base_distance;
  float elipseDiameter = (size * 10 + 4) + 0.05;
  float elipseRadius = elipseDiameter / 2.f;
  float variation_increment = variation / 12.f;
  float variation_i = 0;
  float lfo = 0;

  for (int i = 0; i < 12; i++) {
    lfo = repaint_ui && !freeze ? variationWidgetLfos[i].getNextSample()
                                : variationWidgetLfos[i].getSample();

    float angle = (lfo * 2.f) * cheappi;
    float distance = noi::Outils::mapValue(variation_i, 0.f, 1.f, base_distance,
                                           base_distance + 60.f * variation);

    // polar to cartesian
    float x = (distance * cos(angle));
    float y = (distance * sin(angle));
    // was center on (0,0), offset to correct location + offset the radius of
    // the circle
    x += centerx - elipseRadius;
    y += centery - elipseRadius;

    variation_i += variation_increment;

    g.drawEllipse(Rectangle<float>(x, y, elipseDiameter, elipseDiameter), 2.);
  }
}

void HelleboreEditor::paintTimeWidget(juce::Graphics& g) {
  float centerx = 83;
  float centery = 128;
  float radius = 50;

  float bottom_offset = 0.08;
  float offset_time = feedback * (1.0 - bottom_offset) + bottom_offset;

  juce::Colour colour = CustomColors::getGradientWithoutGreen(feedback);
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

  float paramValue = size;
  // paramValue = std::pow(paramValue, 3);
  paramValue += 0.1;
  paramValue *= 1.3;

  float diameter = static_cast<float>(width) * paramValue;
  float radius = diameter / 2.f;

  juce::Path p;
  float circlex = centreX - radius;
  float circley = centreY - radius;
  float circlew = diameter;
  float circleh = diameter;
  p.addEllipse(circlex, circley, circlew, circleh);

  // warning, need linear value for color gradient
  juce::Colour colour = CustomColors::getGradientWithoutGreen(paramValue);
  colour = CustomColors::fadeToDefault(colour, dry_wet);
  g.setColour(colour);

  Path vignette;
  vignette.addRoundedRectangle(x, y, width, height, 5);

  g.reduceClipRegion(vignette);

  g.fillPath(p);
  //! [pointer]
}