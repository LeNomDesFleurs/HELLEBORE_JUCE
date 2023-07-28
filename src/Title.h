/*
  ==============================================================================

    Title.h
    Created: 3 Apr 2023 10:14:03am
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include <string>

struct Title : juce::Component {

    Title();

    void paint(juce::Graphics& g) override;

    juce::Colour back_ground_color;

    juce::TextEditor title;

    std::string titleString {
    };
};