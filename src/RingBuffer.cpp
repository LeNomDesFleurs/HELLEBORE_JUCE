
#include "RingBuffer.hpp"
#include <iostream>
namespace noi{

/// @brief constructor for ring buffer 
/// @param max_size in seconds
/// @param initial_delay in seconds 
RingBuffer::RingBuffer(int max_size, int initial_delay){
    std::size_t size = static_cast<std::size_t>  (max_size * m_sampling_frequency);
    m_buffer.resize(size);
    m_buffer_size = static_cast<int>(size);
    m_buffer_size-- ;
    m_write = initial_delay * m_sampling_frequency;
    m_read = 0;
}

/// @brief increment pointer and set its int, incremented int and frac value
void RingBuffer::incrementReadPointer(){

   m_read += m_step_size;
   if (m_read > m_buffer_size) m_read -= m_buffer_size;
   else if (m_read < 0) m_read += m_buffer_size;
    m_i_read = static_cast<int>(trunc(m_read));
    m_frac = m_read - static_cast<float>(m_i_read);
    m_i_read_next = m_i_read + 1 > m_buffer_size ? 0 : m_i_read + 1;
}

/// @brief increment read pointer and return sample from interpolation
float RingBuffer::readSample(){
    incrementReadPointer();
    if (m_interpolation_mode == 1) noInterpolation();
    else if (m_interpolation_mode == 2) linearInterpolation();
    else if (m_interpolation_mode == 3) allpassInterpolation();

return m_output_sample;
}

void RingBuffer::noInterpolation(){
    m_output_sample = m_buffer[m_i_read];
}

/// @brief Interpolation lineaire du buffer a un index flottant donne
void RingBuffer::linearInterpolation(){
    // S[n]=frac * Buf[i+1]+(1-frac)*Buf[i]
    m_output_sample = (m_frac * m_buffer[m_i_read+1]) + ((1-m_frac) * m_buffer[m_i_read]);
}


/// @brief Interpolation passe-tout, recursion
void RingBuffer::allpassInterpolation(){
    //S[n]=Buf[i+1]+(1-frac)*Buf[i]-(1-frac)*S[n-1]
    m_output_sample = (m_buffer[m_i_read+1]) + ((1-m_frac) * m_buffer[m_i_read]) - ((1-m_frac) * m_output_sample);
}


/// @brief increment write pointer and write input sample in buffer
/// @param input_sample 
void RingBuffer::writeSample(float input_sample){
    m_write = m_write == m_buffer_size ? 0 : m_write + 1;
    m_buffer[m_write] = input_sample;
    // m_buffer[0] = input_sample;
}
}