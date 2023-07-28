/*
  ==============================================================================

    Filter.h
    Created: 11 Mar 2023 2:33:01pm
    Author:  thoma

  ==============================================================================
*/

#pragma once

#pragma once



#ifndef FILTER
#define FILTER

#include <string>
#include <math.h>
#include <vector>
#include "Outils.h"
#include "Buffer.h"



namespace noi {

namespace Filter {
	
	class Allpass {
	private:
		noi::RingBuffer m_buffer{0.2};
		float m_gain;
		float m_looptime;
	public:
		inline Allpass(float time) {
			m_buffer = noi::RingBuffer(time);
			m_looptime = time;
		}
		inline void setReadSpeed(float ratio){
			m_buffer.setStep(ratio);
		} 
		inline void setGain(float rt60) {
			m_gain = -60. * m_looptime / rt60;
			m_gain = powf(10.0, (m_gain / 20.0));
		}
		inline void overrideFeedback(float feedback){
			m_gain = feedback;
		}
		inline float process(float input) {
			float delay = m_buffer.read();
			delay = noi::Outils::clipValue(delay, -1.f, 1.f);
			float y = ((input + delay * m_gain) * (-m_gain)) + delay;
			m_buffer.write(y);
			return y;
		}
		inline void resize (float time){
			m_buffer.computeStepSize(time);
			m_looptime = time;
		}
	};

	class Comb {
	private:
		float m_gain;
		float m_looptime;
		noi::RingBuffer m_buffer{2.f};
	public:
		inline Comb(float time) {
			m_buffer = noi::RingBuffer(time);
			m_looptime = time;
		}
		inline void setReadSpeed(float ratio){
			m_buffer.setStep(ratio);
		} 
		inline void setGain(float rt60) {
			m_gain = -60.f * m_looptime / rt60;
			m_gain = powf(10.f, (m_gain / 20.f));
		}
		inline void overrideFeedback(float feedback){
			m_gain = feedback;
		}
		inline float process(float input) {
			float delay = m_buffer.read();
			delay = noi::Outils::clipValue(delay, -1.f, 1.f);
			float y = delay * m_gain + input;
			m_buffer.write(y);
			return y;
		}
		inline float processFreezed(){
			return m_buffer.read();
		}
		inline void resize(float time){
			//if (m_looptime == time){return;}
			m_buffer.computeStepSize(time);
			m_looptime = time;
		}
		inline void setFreeze(bool statut){
			m_buffer.setFreeze(statut);
		}

	};/*Comb*/


}/*Filter*/

}/*noi*/

#endif /*FILTER*/