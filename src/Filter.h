/*
  ==============================================================================

    Filter.h
    Created: 28 Feb 2023 11:54:07am
    Author:  thoma

  ==============================================================================
*/

#pragma once



#ifndef FILTER
#define FILTER

#include <string>
#include <math.h>
#include <vector>
//#include "outils.hpp"


const float PI = 3.14159f;

namespace noi {

	namespace Filter {
		class BPF {

		private:
			//Parameters
			float m_fc{ 0.0 };
			float m_Q{ 0.0 };
			//Previous Values Memory
			float m_b[3]{ 0.0f, 0.0f , 0.0f };
			float m_a[3]{ 0.0f, 0.0f, 0.0f };
			//Biquad Coef
			float m_b_gain[3]{ 0.0f, 0.0f, 0.0f };
			float m_a_gain[3]{ 0.0f, 0.0f, 0.0f };
			//Compute Values
			float m_omega{0.0f};
			float m_cosomega{ 0.0f };
			float m_sinomega{ 0.0f };
			float m_alpha{ 0.0f };

			float m_sampling_frequency{ 48000.0 };

		public:
			inline BPF() {}
			inline BPF(float freq, float Q) { setParam(freq, Q); }
			//inline BPF() {}
			inline void setSamplingFrequency(float sampling_frequency) {
				m_sampling_frequency = sampling_frequency;
			}
			inline void computeBiquadCoef() {
				m_a_gain[0] = 1 + m_alpha;

				m_b_gain[0] = m_alpha * m_Q;
				m_b_gain[0] /= m_a_gain[0];

				m_b_gain[1] = 0;
				m_b_gain[1] /= m_a_gain[0];

				m_b_gain[2] = -m_Q * m_alpha;
				m_b_gain[2] /= m_a_gain[0];

				m_a_gain[1] = -2 * m_cosomega;
				m_a_gain[1] /= m_a_gain[0];

				m_a_gain[2] = 1 - m_alpha;
				m_a_gain[2] /= m_a_gain[0];
			}

			inline void setParam(float frequence, float Q) {
				if (m_fc == frequence && m_Q == Q) { return; }
				m_fc = frequence;
				m_Q = Q;
				m_omega = 2.f * PI * (m_fc / 48000.f);
				m_cosomega = cosf(m_omega);
				m_sinomega = sinf(m_omega);
				m_alpha = m_sinomega / (2 * m_Q);
				computeBiquadCoef();
			}


			inline float process(float b0) {
				//feedback & clipping
				float feedback = m_a[0];
				//1500 choosed by experimentation w/ sinensis, self osc around Q = 38
				feedback *= (m_Q / 1500.F);
				if (feedback < -0.95) {
					float overtaking = feedback + 0.95f;
					feedback = -0.95f - (overtaking / 5.f);
				}
				if (feedback > 0.95f) {
					float overtaking = feedback - 0.95f;
					feedback = 0.95f + (overtaking / 5.f);
				}
				if (feedback > 1.0f) { feedback = 1.0f; }
				if (feedback < -1.0f) { feedback =-1.0f; }
				b0 += feedback;
				//shift new value in
				m_b[2] = m_b[1];
				m_b[1] = m_b[0];
				m_b[0] = b0;
				m_a[2] = m_a[1];
				m_a[1] = m_a[0];

				m_a[0] = m_b[0] * m_b_gain[0]
					+ m_b[1] * m_b_gain[1]
					+ m_b[2] * m_b_gain[2]
					- m_a[1] * m_a_gain[1]
					- m_a[2] * m_a_gain[2];

				return m_a[0];
			}

		};/*BPF*/

	}/*Filter*/

}/*noi*/

#endif /*FILTER*/
