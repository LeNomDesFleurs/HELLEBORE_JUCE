#include <vector>

namespace noi{
class RingBuffer{
public:
RingBuffer(int max_size, int initial_delay);
float readSample();
void writeSample(float input_sample);
float interpolateSample();
void linearInterpolation();
void noInterpolation();
void allpassInterpolation();
void incrementReadPointer();


private:
int m_sampling_frequency = 48000;
std::vector<float> m_buffer;
float m_read, m_write;
int m_i_read;
int m_i_read_next;
float m_step_size = 1;
int m_interpolation_mode = 1;
//m_buffer_size en base 0
int m_size_goal, m_actual_size, m_buffer_size;
float sample;
float m_frac;
float m_output_sample;

};
}


