
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
    m_write = initial_delay * m_sampling_frequency;
    m_read = 0;
}

/// @brief increment read pointer and read sample from buffer
/// @return 
float RingBuffer::readSample(){
float read_sample = 0;
m_read = m_read == m_buffer_size ? 0 : m_read + 1;
if (m_read == 1000) std::cout<<m_buffer[m_read];
read_sample = m_buffer[m_read];
// return read_sample;
return sample;
}

/// @brief increment write pointer and write input sample in buffer
/// @param input_sample 
void RingBuffer::writeSample(float input_sample){
    m_write = m_write == m_buffer_size ? 0 : m_write + 1;
    m_buffer[m_write] = input_sample;
    sample = input_sample;
}
}