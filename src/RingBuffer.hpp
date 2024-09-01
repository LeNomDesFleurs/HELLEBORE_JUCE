#pragma once

#include <JuceHeader.h>

#include <iostream>
#include <vector>

#include "Outils.hpp"

namespace noi {

class RingBuffer {
 public:
  RingBuffer(float max_time, float initial_delay, int _sample_rate);
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
  float getActualSize();
  void crossfade();

 private:
  // the accumulating status is used before freezing,
  // it collect some more sample before making a fade with the previous signal,
  // a simpe freeze whitout this fade would produce clicks when going from last
  // wrote to first wrote
  unsigned int m_samples_for_fade{100};
  int sample_rate;
  enum BufferMode { normal, accumulate, freeze, reverse };
  BufferMode m_buffer_mode{normal};
  enum InterpolationMode { none, linear, allpass };
  InterpolationMode interpolation_mode = linear;
  std::vector<float> m_buffer;
  std::vector<float> m_crossfade_buffer;
  int m_write;
  float m_read{};
  int m_i_read{};
  int m_i_read_next{};
  float m_frac{};
  float m_step_size{1.0};
  int m_size_goal;
  // m_buffer_size en base 0
  int m_buffer_size;
  float m_actual_size{};
  float m_size_on_freeze{};
  float m_output_sample{};
  bool new_size{false};
  // number of sample accumulated before fading and freezing
  int accumulate_count{};
};
}  // namespace noi
