#pragma once

#include <iostream>
#include <vector>

#include "Outils.hpp"

namespace noi {

class RingBuffer {
 public:
  RingBuffer(int max_size, int initial_delay, double _sample_rate);
  RingBuffer(float max_size);
  RingBuffer();
  float readSample();
  void writeSample(float input_sample);
  void linearInterpolation();
  void noInterpolation();
  void allpassInterpolation();
  void incrementReadPointer();
  void freezeIncrementReadPointer();
  void setStepSize(float step_size);
  void setDelayTime(float delay_time);
  void updateStepSize();
  void freezedUpdateStepSize();
  void setSampleRate(float _sample_rate);
  void setFreezed(bool _freezed);
  void checkForReadIndexOverFlow();
  /// @brief Split read index in m_i_read (previous sample) m_i_read_next (next
  /// sample) and m_frac (fractional offset)
  void fractionalizeReadIndex();

 private:
  enum InterpolationMode { none, linear, allpass };
  InterpolationMode interpolation_mode = linear;
  bool freezed;
  bool reverse;
  int sample_rate = 48000;
  std::vector<float> m_buffer;
  float m_read, m_write;
  int m_i_read;
  int m_i_read_next;
  float m_step_size = 1;
  // m_buffer_size en base 0
  int m_size_goal, m_buffer_size;
  float m_actual_size;
  float m_size_on_freeze;
  float m_frac;
  float m_output_sample;
};
}  // namespace noi
