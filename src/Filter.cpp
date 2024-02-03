/*
  ==============================================================================

    Filter.h
    Created: 11 Mar 2023 2:33:01pm
    Author:  thoma

  ==============================================================================
*/

#include "Filter.hpp"

namespace noi {

namespace Filter {

// Filter::Filter(float time){
//     m_buffer = noi::RingBuffer(time);
// 	m_looptime = time;
// }

void Filter::setReadSpeed(float ratio) { m_buffer.setStepSize(ratio); }

/// @brief Define the time needed to loose 60 dB
/// @param rt60 decay time in seconds
void Filter::setGain(float rt60) {
  m_gain = -60. * m_looptime / rt60;
  m_gain = powf(10.0, (m_gain / 20.0));
}

void Filter::overrideFeedback(float feedback) { m_gain = feedback; }

void Filter::setFreeze(bool status) { m_buffer.setFreezed(status); }

/// @brief Set new delay time
/// @param time in seconds
void Filter::resize(float time) {
  m_buffer.setDelayTime(time * 1000);
  m_looptime = time;
}

float Filter::process(float sample) { return sample; }

float Filter::processFreezed() { return m_buffer.readSample(); }

Allpass::Allpass(float time) {
  m_buffer = noi::RingBuffer(time);
  m_looptime = time;
}

float Allpass::process(float input) {
  float delay = m_buffer.readSample();
  delay = noi::Outils::clipValue(delay, -1.f, 1.f);
  float y = ((input + delay * m_gain) * (-m_gain)) + delay;
  m_buffer.writeSample(y);
  return y;
}

Comb::Comb(float time) {
  m_buffer = noi::RingBuffer(time);
  m_looptime = time;
}

float Comb::process(float input) {
  float delay = m_buffer.readSample();
  delay = noi::Outils::clipValue(delay, -1.f, 1.f);
  float y = delay * m_gain + input;
  m_buffer.writeSample(y);
  return y;
}

}  // namespace Filter

}  // namespace noi
