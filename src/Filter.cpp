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
Filter::Filter(float max_time, float initial_delay, int sample_rate) 
  : m_buffer(max_time, initial_delay, sample_rate)
  , m_looptime {max_time}
{}

void Filter::reset(float max_time, float initial_delay, int sample_rate){
  m_buffer.reset(max_time, initial_delay, sample_rate);
  m_looptime = max_time;
}

void Filter::setReadSpeed(float ratio) { m_buffer.setStepSize(ratio); }

/// @brief Define the time needed to loose 60 dB
/// @param rt60 decay time in seconds
void Filter::setGain(float rt60) {
  m_gain = -60. * m_looptime / rt60;
  m_gain = powf(10.0, (m_gain / 20.0));
}

void Filter::overrideFeedback(float feedback) { m_gain = feedback; }

void Filter::setFreeze(bool status) { m_buffer.setFreezed(status); }

void Filter::setSampleRate(float sample_rate) {
  m_buffer.setSampleRate(sample_rate);
}

/// @brief Set new delay time
/// @param time in seconds
void Filter::resize(float time) {
  m_buffer.setDelayTime(time * 1000);
  m_looptime = time;
}

float Filter::process(float sample) { return sample; }

float Filter::processFreezed() { return m_buffer.readSample(); }

// Allpass::Allpass(float max_time, float initial_delay, int sample_rate) 
//   : m_buffer(max_time, initial_delay, sample_rate)
//   , m_looptime {max_time}
// {}

float Allpass::process(float input) {
  float delay = m_buffer.readSample();
  // delay = noi::Outils::clipValue(delay, -1.f, 1.f);
  // float buf_in = (delay * m_gain) + input;
  // float buf_out = delay + (input * -m_gain);
  // m_buffer.writeSample(buf_in);
  // return buf_out;
  float y = ((input + delay * m_gain) * (-m_gain)) + delay;
  m_buffer.writeSample(y);
  return y;
}

// Comb::Comb(float max_time, float initial_delay, int sample_rate)
//   : m_buffer(max_time, initial_delay, sample_rate)
//   , m_looptime {max_time}
// {}

float Comb::process(float input) {
  float delay = m_buffer.readSample();
  delay = noi::Outils::clipValue(delay, -1.f, 1.f);
  float output = noi::Outils::equalPowerCrossfade(input, delay, m_gain);
  m_buffer.writeSample(output);
  return output;
}

}  // namespace Filter

}  // namespace noi
