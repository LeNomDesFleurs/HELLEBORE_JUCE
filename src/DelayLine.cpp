#include "DelayLine.h"

namespace noi{

    // DelayLine::DelayLine(){}
    DelayLine::DelayLine(int max_size, int initial_delay){
        m_buffer = RingBuffer(max_size, initial_delay);
    }

    float DelayLine::processSampleComb(float input_sample){
        


        float output_sample = m_buffer.readSample();

        m_buffer.writeSample(input_sample + (output_sample * m_feedback_ratio));

        return output_sample;
    }

    DelayLine::updateLenght(){
        
        slewValue()

    m_buffer.setStepSize();
    }

}