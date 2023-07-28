#include <vector>

namespace noi{
class RingBuffer{
public:
RingBuffer(int max_size, int initial_delay);
float readSample();
void writeSample(float input_sample);
float interpolateSample();

private:
int m_sampling_frequency = 48000;
std::vector<float> m_buffer;
float m_read, m_write;
int m_size_goal, m_actual_size, m_buffer_size;
float sample;

};
}


