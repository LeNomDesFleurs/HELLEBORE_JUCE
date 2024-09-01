/*
  ==============================================================================

    Filter.h
    Created: 11 Mar 2023 2:33:01pm
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include <math.h>

#include <string>
#include <vector>

#include "Outils.hpp"
#include "RingBuffer.hpp"

namespace noi {

namespace Filter {

class Filter {
 public:
  Filter(float max_time, float initial_delay, int sample_rate);
  void setReadSpeed(float ratio);
  void setGain(float rt60);
  void overrideFeedback(float feedback);
  virtual float process(float input);
  void resize(float time);
  float processFreezed();
  void setFreeze(bool status);
  void setSampleRate(float sample_rate);

 protected:
  noi::RingBuffer m_buffer;
  float m_gain;
  float m_looptime;
};

class Allpass : public Filter {
 public:
  using Filter::Filter;
  // Allpass(float max_time, float initial_delay, int sample_rate);
  float process(float input);
};

class Comb : public Filter {
 public:
 using Filter::Filter;
  // Comb(float max_time, float initial_delay, int sample_rate);
  float process(float input);
}; /*Comb*/

}  // namespace Filter

}  // namespace noi