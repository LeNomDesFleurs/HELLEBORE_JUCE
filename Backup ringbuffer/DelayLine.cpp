#include "DelayLine.hpp"

namespace noi{

    // DelayLine::DelayLine(){}
    DelayLine::DelayLine(int max_size, int initial_delay){
        m_buffer = RingBuffer(max_size, initial_delay);
    }

    float DelayLine::processSampleComb(float input_sample){
      updateLenght();

      float output_sample = m_buffer.readSample();

      m_buffer.writeSample(input_sample + (output_sample * m_feedback_ratio));

      return output_sample;
    }

    void DelayLine::updateLenght(){
         Outils::slewValue(m_step_size, m_step_size, 0.97)
    // using -2 and +2 to avoid oscillation, gotta try without
    if (m_goal_delay > m_actual_delay + 2){
        m_actual_delay
      m_buffer.setStepSize(1.5);
    } else if (m_goal_delay < m_actual_delay - 2){
      m_buffer.setStepSize(0.5);
    }
    else
      m_buffer.setStepSize(1);

    //     slewValue()

    // m_buffer.setStepSize();
    }

}