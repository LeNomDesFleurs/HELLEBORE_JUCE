#include "RingBuffer.hpp"

namespace noi {

/// @brief constructor for ring buffer
/// @param max_time in seconds
/// @param initial_delay in seconds
RingBuffer::RingBuffer(float max_time, float initial_delay, int _sample_rate)
    : sample_rate{_sample_rate},
      m_write{(int)(initial_delay * (float)sample_rate)},
      m_actual_size{(float)m_write},
      m_crossfade_buffer(m_samples_for_fade, 0),
      m_buffer(max_time * (float)sample_rate, 0),
      m_buffer_size{(int)m_buffer.size() - 1} {}

/// @brief increment pointer and set its int, incremented int and frac value
void RingBuffer::incrementReadPointer() {
  m_read += m_step_size;
  checkForReadIndexOverFlow();
  if (m_read > m_buffer_size) m_read -= m_buffer_size;
  // in case of reverse read
  else if (m_read < 0)
    m_read += m_buffer_size;
}

float RingBuffer::getActualSize() {
  float temp_read = m_read;
  if (m_read > (float)m_write) temp_read -= m_buffer_size;
  float output = (float)m_write - temp_read;
  return output;
}


/// @brief increment read pointer and return sample from interpolation
float RingBuffer::readSample() {
  if (m_buffer_mode == reverse) {
    m_step_size = 0. - m_step_size;
  }

  if (m_buffer_mode == freeze) {
    freezeIncrementReadPointer();
    freezedUpdateStepSize();
  } else {
    updateStepSize();
    incrementReadPointer();
  }

  fractionalizeReadIndex();

  // those functions modify the m_output_sample value
  switch (interpolation_mode) {
    case none:
      noInterpolation();
      break;
    case linear:
      linearInterpolation();
      break;
    case allpass:
      allpassInterpolation();
      break;
  }

  if (m_buffer_mode == freeze) {
    m_output_sample *= noi::Outils::clipValue(1.f / pow(m_step_size, 0.5), 0.2, 3.);
  }
  return m_output_sample;
}

void RingBuffer::noInterpolation() { m_output_sample = m_buffer[m_i_read]; }

/// @brief Interpolation lineaire du buffer a un index flottant donne
void RingBuffer::linearInterpolation() {
  // S[n]=frac * Buf[i+1]+(1-frac)*Buf[i]
  m_output_sample =
      (m_frac * m_buffer[m_i_read_next]) + ((1 - m_frac) * m_buffer[m_i_read]);
}

/// @brief Interpolation passe-tout, recursion
void RingBuffer::allpassInterpolation() {
  // S[n]=Buf[i+1]+(1-frac)*Buf[i]-(1-frac)*S[n-1]
  m_output_sample = (m_buffer[m_i_read + 1]) +
                    ((1 - m_frac) * m_buffer[m_i_read]) -
                    ((1 - m_frac) * m_output_sample);
}

/// @brief increment write pointer and write input sample in buffer
/// @param input_sample
void RingBuffer::writeSample(float input_sample) {
  if (m_buffer_mode == normal || m_buffer_mode == reverse) {
    m_write = (m_write + 1) % m_buffer_size;
    m_buffer[m_write] = input_sample;
    // m_buffer[0] = input_sample;
  } else if (m_buffer_mode == accumulate) {
    if (accumulate_count != m_samples_for_fade) {
      m_crossfade_buffer[accumulate_count] = input_sample;
      accumulate_count++;
    } else {
      m_buffer_mode = freeze;
      crossfade();
      accumulate_count = 0;
    }
  }
}

void RingBuffer::crossfade() {
  for (int i = m_samples_for_fade -1; i >= 0; i--) {
    int buffer_index = (m_write + i +1) % m_buffer_size;
    float coef = (float)i / (float)(m_samples_for_fade - 1);

    // std::cout<< m_crossfade_buffer[i]<<'-' << m_buffer[buffer_index]<< '-' <<coef;
    m_buffer[buffer_index] = noi::Outils::linearCrossfade(
        m_crossfade_buffer[i], m_buffer[buffer_index], coef);
    // std::cout << '\n';
  }

}

void RingBuffer::setStepSize(float step_size) { m_step_size = step_size; }

/// @brief Triggered at each sample, update the step size and the m_actual_size
/// to keep up with change of size goal
void RingBuffer::updateStepSize() {
  float step_size_goal = 1.0;
  m_actual_size = getActualSize();
  // big sample limit to account for inertia
 if ((m_actual_size > m_size_goal) && new_size) {
    // m_step_size = 1.5;
    step_size_goal = 2.0;
    // update the step size but with slew for clean repitch
  } else if ((m_actual_size < m_size_goal) && new_size) {
    // m_step_size = 0.5;
    step_size_goal = 0.5;
  }


  m_step_size =
      noi::Outils::slewValue(step_size_goal, m_step_size,
                             0.999);  // should be modified by sample rate
 if (m_actual_size > (m_size_goal - 200) &&
      m_actual_size < (m_size_goal + 200)) {
    if (ready_to_lock)
    m_step_size = 1.0;
    new_size = false;
  } else {
    ready_to_lock = true;
  }
}

/// @brief Take a delay time in milliseconds, clip it within the defined max
/// buffer size and set the goal to reach.
/// @param delay_time in milliseconds
void RingBuffer::setDelayTime(float delay_time) {
  new_size = true;
  float delay_in_samples =
      noi::Outils::convertMsToSample(delay_time, sample_rate);
  m_size_goal = noi::Outils::clipValue(delay_in_samples, 2000, m_buffer_size - 1);
}

void RingBuffer::setSampleRate(float _sample_rate) {
  sample_rate = _sample_rate;
}

void RingBuffer::setFreezed(bool freezed) {
  // avoid updating the m_size_on_freeze
  if (m_buffer_mode != freeze && m_buffer_mode != accumulate && freezed) {
    m_size_on_freeze = getActualSize();
    m_buffer_mode = accumulate;
  }
  if (!freezed) m_buffer_mode = normal;
}

void RingBuffer::freezedUpdateStepSize() {
  float step_size_goal = m_size_on_freeze / m_size_goal;
  m_step_size = noi::Outils::slewValue(step_size_goal, m_step_size, 0.9999);
}

void RingBuffer::checkForReadIndexOverFlow() {
  if (m_read < 0) {
    m_read += m_buffer_size;
  }
  if (m_read > m_buffer_size) {
    m_read -= m_buffer_size;
  }
}

void RingBuffer::fractionalizeReadIndex() {
  // get sample
  m_i_read = static_cast<int>(trunc(m_read));
  // get fraction
  m_frac = m_read - static_cast<float>(m_i_read);
  // Get next sample
  m_i_read_next = (m_i_read + 1) > m_buffer_size ? 0 : (m_i_read + 1);
}

void RingBuffer::freezeIncrementReadPointer() {
  m_read += m_step_size;
  // buffer over and under flow
  checkForReadIndexOverFlow();
  // m_actual_size -= m_step_size;

  // In freezed case, m_read only iterate on the last buffer size,
  //  hence it's like a little ringBuffer in the bigger ringBuffer
  //  so more buffer over and under flow
  // if (m_actual_size < 0.) {
  //   m_read -= (float)m_write - m_size_on_freeze;
  //   checkForReadIndexOverFlow();
  //   m_actual_size = m_size_on_freeze;

  // } else if (m_actual_size > m_size_on_freeze) {
  //   m_read = (float)m_write;
  //   m_actual_size = 0;
  // }
}
}  // namespace noi