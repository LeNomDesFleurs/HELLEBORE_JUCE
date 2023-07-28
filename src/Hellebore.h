/*
  ==============================================================================

    Hellebore.h
    Created: 11 Mar 2023 2:28:59pm
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include "Filter.h"
#include <vector>
#include <array>

namespace noi {

		class StereoMoorer {
		public:
			/// @brief Parameters of a stereoMoorer Reverb
			/// @param freeze 
			/// @param drywet from 0 to 1
			/// @param comb_time
			/// @param variation
			/// @param rt60
			struct Parameters {
				bool freeze;
				float dry_wet,
					comb_time,
					variation,
					rt60;
			};
			StereoMoorer(noi::StereoMoorer::Parameters parameters);
			void updateParameters(noi::StereoMoorer::Parameters parameters);
			void setPan();
			void setTime();
			void setFreeze();
			void resizeComb();
			void processCombs(std::array<float, 2> inputs);
			std::array<float, 2> processStereo(std::array<float, 2> inputs);

		private:
			//6 comb par canal
			std::array<std::array<noi::Filter::Comb, 6>, 2> m_combs = { {{
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f)},
						{noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f),
						noi::Filter::Comb(2.f)}}
			};
			//1 allpass par canal
			std::array<noi::Filter::Allpass, 2> m_allpasses{
					noi::Filter::Allpass(0.006),
					noi::Filter::Allpass(0.006) };

			noi::StereoMoorer::Parameters m_parameters;
			std::array<std::array<float, 6>, 2> m_combs_status;
			std::array<float, 6> m_pan_coefs;
		};/*StereoMoorer*/


		/* Structure Moorer
		* 
		* |comb| ->
		* |comb| ->
		* |comb| ->	Sum -> Allpass -> Out
		* |comb| ->
		* |comb| ->
		* |comb| ->
		* 
		*/

}/*noi*/