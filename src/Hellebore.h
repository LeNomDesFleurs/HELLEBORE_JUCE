/*
  ==============================================================================

    Hellebore.h
    Created: 11 Mar 2023 2:28:59pm
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include <array>
#include <vector>

#include "Filter.hpp"

namespace noi {

/* Structure Moorer for each channel
 *
 * 					          -> |comb| -> |pan| +
 * 					          -> |comb| -> |pan| +
 * Input -> |allpass| -> |comb| -> |pan| + -> Sum -> Out
 * 					          -> |comb| -> |pan| +
 * 					          -> |comb| -> |pan| +
 * 					          -> |comb| -> |pan| +
 *
 * The pan is a cross fade with the corresponding comb
 * of the other channel
 */
class StereoMoorer {
 public:
  /// @brief Parameters of a stereoMoorer Reverb
  /// @param freeze
  /// @param drywet from 0 to 1
  /// @param comb_time
  /// @param variation
  /// @param feedback
  struct Parameters {
    bool freeze;
    float dry_wet, comb_time, variation, feedback;
  };
  StereoMoorer(noi::StereoMoorer::Parameters parameters, int sample_rate);
  bool variationHaventChange();
  bool timeHaventChange();
  bool combSizeHaventChange();
  bool freezeHaventChange();
  void updateParameters(noi::StereoMoorer::Parameters parameters);
  void setPan();
  void setTime();
  void setFreeze();
  void resizeComb();
  void processCombs(std::array<float, 2> inputs);
  void processAllPasses(std::array<float, 2> inputs);
  void spatializeCombs();
  void setSampleRate(float sample_rate);
  std::array<float, 2> processStereo(std::array<float, 2> inputs);

 private:
  // 6 comb par canal
  std::array<std::array<noi::Filter::Comb, 6>, 2> m_combs;
  // 1 allpass par canal
  std::array<noi::Filter::Allpass, 2> m_allpasses;

  noi::StereoMoorer::Parameters m_parameters;
  noi::StereoMoorer::Parameters m_old_parameters;
  std::array<float, 2> m_allpasses_status {0, 0};
  std::array<std::array<float, 6>, 2> m_combs_status;
  std::array<float, 6> m_pan_coefs;

}; /*StereoMoorer*/

}  // namespace noi