/*
  ==============================================================================

    Outils.h
    Created: 11 Mar 2023 5:41:09pm
    Author:  thoma

  ==============================================================================
*/

#pragma once

#ifndef OUTILS
#define OUTILS
#include <vector>
const float cheappi{ 3.14159265359 };

namespace noi {

	namespace Outils {
	/// @brief Slow value change of a parameter, slew factor working best between 0.8 - 0.99
	/// @param new_value 
	/// @param old_value 
	/// @param slew_factor a bigger slew factor means a slower change, must be <1 to keep stability
	/// @return 
	float slewValue(float new_value, float old_value, float slew_factor){
	return (new_value * (1.0-slew_factor)) + (old_value * (slew_factor));
	}


		inline float convertMsToSample(float time) { 
			float temp = truncf(48000.f * time); 
			return temp; }
	
		inline int mapValueFloatToInt(float inMin, float inMax, float value, int outMin, int outMax) {
			float ratio = ((outMax - outMin) / (inMax - inMin));
			float offset{ outMin - (inMin * ratio) };
			int output = static_cast<int> (value * ratio + offset);
			return output;
		}

		inline float mapValue(float value, float inMin, float inMax, float outMin, float outMax) {
			float ratio{ (outMax - outMin) / (inMax - inMin) };
			float offset{ outMin - (inMin * ratio) };
			float output{ value * ratio + offset };
			return output;
		}
		
		inline float clipValue(float value, float min, float max) {
			if (value > max) value = max;
			if (value < min) value = min;
			return value;
		}

		inline float spliter(float target, float state, float diff) {
			if (state < target) { state += (target - state) * 2; }
			float slope = 0 - (1 / diff);
			state -= target;
			float coef = 1 + slope * state;
			if (coef < 0) { return 0; }
			else return coef;
		}
		/// @brief take two signals and return the linear crossfade
		/// @param dry signal
		/// @param wet signal
		/// @param parameter 0 full dry / 1 full wet
		/// @return 
		inline float dryWet(float dry, float wet, float parameter){
			return (dry * (1.0 - parameter)) + (wet * parameter);
		}
	


	}/*Outils*/
}/*noi*/

#endif /*OUTILS*/