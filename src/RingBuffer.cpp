#include "RingBuffer.hpp"

namespace noi {

/// @brief constructor for ring buffer
/// @param max_size in seconds
/// @param initial_delay in seconds
RingBuffer::RingBuffer(int max_size, int initial_delay, double _sample_rate) {
  sample_rate = _sample_rate;
  std::size_t size = static_cast<std::size_t>(max_size * sample_rate);
  m_buffer.resize(size);
  m_buffer_size = static_cast<int>(size);
  m_buffer_size--;
  m_write = initial_delay * sample_rate;
  m_actual_size = m_write;
  m_read = 0;
  m_frac = 0;
}
RingBuffer::RingBuffer() {}

/// @brief constructor for ring buffer, initial delay is half of max_size by
/// default
/// @param max_size in seconds
RingBuffer::RingBuffer(float max_size) {
  std::size_t size = static_cast<std::size_t>(max_size * sample_rate);
  m_buffer.resize(size);
  m_buffer_size = static_cast<int>(size);
  m_buffer_size--;
  m_write = m_buffer_size / 2.f;
  m_read = 0;
}

/// @brief increment pointer and set its int, incremented int and frac value
void RingBuffer::incrementReadPointer() {
  m_read += m_step_size;
  checkForReadIndexOverFlow();
  if (m_read > m_buffer_size) m_read -= m_buffer_size;
  // in case of reverse read
  else if (m_read < 0)
    m_read += m_buffer_size;
}

/// @brief increment read pointer and return sample from interpolation
float RingBuffer::readSample() {
  if (reverse) {
    m_step_size = 0. - m_step_size;
  }

  if (freezed) {
    freezeIncrementReadPointer();
    freezedUpdateStepSize();
  } else {
    incrementReadPointer();
    updateStepSize();
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
  if (!freezed) {
    m_write = m_write > m_buffer_size ? 0 : m_write + 1;
    m_buffer[m_write] = input_sample;
    // m_buffer[0] = input_sample;
  }
}

void RingBuffer::setStepSize(float step_size) { m_step_size = step_size; }

/// @brief Triggered at each sample, update the step size and the m_actual_size
/// to keep up with change of size goal
void RingBuffer::updateStepSize() {
  float correction_offset;
  if (m_actual_size > (m_size_goal - 5) && m_actual_size < (m_size_goal + 5)) {
    correction_offset = 1.0;
  } else if (m_actual_size > m_size_goal) {
    correction_offset = 0.5;
    // update the step size but with slew for clean repitch
  } else if (m_actual_size < m_size_goal) {
    correction_offset = 1.5;
  }

  m_step_size = noi::Outils::slewValue(correction_offset, m_step_size, 0.999);

  // if (!freezed){
  if (m_step_size > 1) {
    m_actual_size += m_step_size - 1;

  } else if (m_step_size < 1) {
    m_actual_size -= 1 - m_step_size;
  }
  // update the step size and update the actual delay time
  // }
}

/// @brief Take a delay time in milliseconds, clip it within the defined max
/// buffer size and set the goal to reach.
/// @param delay_time in milliseconds
void RingBuffer::setDelayTime(float delay_time) {
  float delay_in_samples =
      noi::Outils::convertMsToSample(delay_time, sample_rate);
  //   adding some 4 samples padding just to be sure.
  m_size_goal = noi::Outils::clipValue(delay_in_samples, 4, m_buffer_size - 4);
}

void RingBuffer::setSampleRate(float sample_rate) { sample_rate = sample_rate; }

void RingBuffer::setFreezed(bool _freezed) {
  // avoid updating the m_size_on_freeze
  if (!freezed) {
    m_size_on_freeze = m_actual_size;
  }
  freezed = _freezed;
}

void RingBuffer::freezedUpdateStepSize() {
  m_step_size = m_size_on_freeze / m_size_goal;
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
  m_actual_size -= m_step_size;

  // In freezed case, m_read only iterate on the last buffer size,
  //  hence it's like a little ringBuffer in the bigger ringBuffer
  //  so more buffer over and under flow
  if (m_actual_size < 0.) {
    m_read -= m_write - m_size_on_freeze;
    checkForReadIndexOverFlow();
    m_actual_size = m_size_on_freeze;

  } else if (m_actual_size > m_size_on_freeze) {
    m_read = m_write;
    m_actual_size = 0;
  }
}
}  // namespace noi