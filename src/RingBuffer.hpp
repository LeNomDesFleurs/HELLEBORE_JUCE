#pragma once

#include <vector>
#include "Outils.hpp"
#include <iostream>

namespace noi{
class RingBuffer{
public:
RingBuffer(int max_size, int initial_delay);
RingBuffer(float max_size);
float readSample();
void writeSample(float input_sample);
float interpolateSample();
void linearInterpolation();
void noInterpolation();
void allpassInterpolation();
void incrementReadPointer();
void setStepSize(float step_size);
void setDelayTime(float delay_time);
void updateStepSize();
void setSampleRate(float sample_rate);

private:
 enum InterpolationMode { none, linear, allpass };
 InterpolationMode interpolation_mode = linear;
 int sample_rate = 48000;
 std::vector<float> m_buffer;
 float m_read, m_write;
 int m_i_read;
 int m_i_read_next;
 float m_step_size = 1;
 // m_buffer_size en base 0
 int m_size_goal, m_actual_size, m_buffer_size;
 float sample;
 float m_frac;
 float m_output_sample;

};
}


