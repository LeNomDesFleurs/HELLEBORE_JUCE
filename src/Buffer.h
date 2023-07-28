/*
  ==============================================================================

    Buffer.h
    Created: 11 Mar 2023 2:32:38pm
    Author:  thoma

  ==============================================================================
*/

#pragma once


#include "Outils.h"
#include <math.h>

namespace noi {

	class RingBuffer {

	public:
		inline RingBuffer(float max_time);
		inline float read();
		inline void write(float new_sample);
		inline float computeBufferSize();
		inline void setStep(float step) { m_step = step; }
		inline void setSizeWoRepitch(float size);
		inline void computeStepSize(float new_time);
		inline float getActualSize();
		inline void setFreeze(bool statut);

	private:
		float m_read;
		float m_write;
		float m_buffer_size;
		float m_step;
		float m_time;
		bool m_freeze{ false };
		float m_actual_size;
		std::vector<float> m_buffer;
	};


	inline RingBuffer::RingBuffer(float max_time) {
		m_buffer_size = noi::Outils::convertMsToSample(max_time);
		size_t buffer_int = static_cast<size_t> (m_buffer_size);
		m_buffer.resize(buffer_int);
		m_read = 0.f;
		m_write = m_buffer_size / 2.f;
		m_actual_size = m_write;
		m_time = max_time / 2.f;
		m_step = 1.f;
		std::fill(m_buffer.begin(), m_buffer.end(), 0.);
	}

	inline float RingBuffer::read() {
		m_read += m_step;
		m_read = fmodf(m_read, m_buffer_size - 1.f);
		double tmp;
		float fractional;
		
		fractional = modf(m_read, &tmp);
		int int_read = static_cast<int> (tmp);
		float sample1 = m_buffer[int_read];
		float sample2 = m_buffer[int_read + 1];
		//linear interpolation
		float output = ((sample2 - sample1) * fractional) + sample1;
		return output;
		return 0.;
	}

	inline void RingBuffer::write(float new_sample) {
		m_write += 1.f;
		m_write = fmodf(m_write, m_buffer_size - 1.f);
		int int_write = static_cast<int>(m_write);
		m_buffer[int_write] = new_sample;
	}

	inline float RingBuffer::computeBufferSize() {
		float size = m_write - m_read;
		if (m_write < m_read) {
			size = m_write + (m_buffer_size - m_read);
		}
		return size;
	}

	inline void RingBuffer::computeStepSize(float new_time) {
		if (m_freeze) {
			m_step = m_time / new_time;
			return;
		}
		m_time = new_time;
		float size_goal = noi::Outils::convertMsToSample(new_time);
		size_goal = noi::Outils::clipValue(size_goal, 0.f, (m_buffer_size - 2.f));
		float actual_size = computeBufferSize();
		m_actual_size = actual_size;
		//ralenti si le buffer doit être plus petit
		//accelere si le buffer doit être plus grand
		if (actual_size < (size_goal - 2.f)) {
			m_step = 0.8;
		}
		else if (actual_size < (size_goal - 4800.f)) {
			m_step = 0.5;
		}
		else if (actual_size > (size_goal + 2.f)) {
			m_step = 1.2;
		}
		else if (actual_size > (size_goal + 4800.f)) {
			m_step = 2.f;
		}
		else { m_step = 1.f; }
	}

	inline void RingBuffer::setFreeze(bool statut) {
		m_freeze = statut;
	}

	inline void RingBuffer::setSizeWoRepitch(float size) {
		m_read = 0;
		m_write = noi::Outils::convertMsToSample(size);
	}

	inline float RingBuffer::getActualSize() {
		return m_actual_size;
	}

}