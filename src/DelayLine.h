 
#include "RingBuffer.hpp"

 namespace noi{
 class DelayLine{
    public:
    // DelayLine();
    DelayLine(int max_size, int initial_delay);
    float processSampleComb(float input_sample);
    float readSampleAllpass();
    void updateLenght();


    private:
    int m_goal_delay;
    int m_actual_delay;
    float m_feedback_ratio = 0.5;
    RingBuffer m_buffer;
    float m_output_sample;



 };
 
 
 }