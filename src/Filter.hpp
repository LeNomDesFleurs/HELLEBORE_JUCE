/*
  ==============================================================================

    Filter.h
    Created: 11 Mar 2023 2:33:01pm
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include <string>
#include <math.h>
#include <vector>
#include "Outils.hpp"
#include "RingBuffer.hpp"



namespace noi {

namespace Filter {

	class Filter{
		public:
        //   Filter(float time);
          void setReadSpeed(float ratio);
          void setGain(float rt60);
          void overrideFeedback(float feedback);
          virtual float process(float input);
          void resize(float time);
          float processFreezed();
          void setFreeze(bool status);
		protected:
            noi::RingBuffer m_buffer{0.2};
			float m_gain;
          	float m_looptime;
        };

        class Allpass :public Filter{
         public:
          Allpass(float time);
          float process(float input); 
        };

        class Comb :public Filter{
		public:
         Comb(float time);
         float process(float input);
        };/*Comb*/


}/*Filter*/

}/*noi*/