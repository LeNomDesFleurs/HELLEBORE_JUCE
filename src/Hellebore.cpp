/*
  ==============================================================================

    Hellebore.cpp
    Created: 11 Mar 2023 2:28:30pm
    Author:  thoma

  ==============================================================================
*/

#include "Hellebore.h"

namespace noi {

StereoMoorer::StereoMoorer(noi::StereoMoorer::Parameters parameters) {
  updateParameters(parameters);
  std::fill(begin(m_combs_status[0]), end(m_combs_status[0]), 0.0);
  std::fill(begin(m_combs_status[1]), end(m_combs_status[1]), 0.0);
  std::fill(begin(m_pan_coefs), end(m_pan_coefs), 0.0);
};

void StereoMoorer::updateParameters(noi::StereoMoorer::Parameters parameters) {
  m_parameters = parameters;
  setTime();
  setFreeze();
  resizeComb();
  setPan();
}

// more variation -> more bleeding between channels
void StereoMoorer::setPan() {
  float variation = m_parameters.variation;
  for (int i = 2; i < 6; i++) {
    m_pan_coefs[i] = variation;
    variation *= 2;
  }
}

// more variation -> more diffrence of gain between combs
void StereoMoorer::setTime() {
  float rt60 = m_parameters.rt60;
  float variation = m_parameters.variation + 1.f;
  for (int i = 0; i < 2; i++) {
    m_allpasses[i].setGain(rt60);
    for (int j = 0; j < 6; j++) {
      m_combs[i][j].setGain(rt60);
      ;
    }
  }
}

void StereoMoorer::setFreeze() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      m_combs[i][j].setFreeze(m_parameters.freeze);
    }
    for (auto& allpases : m_allpasses) {
      allpases.setFreeze(m_parameters.freeze);
    }
  }
}

// more variation -> more diffrence of gain between combs
void StereoMoorer::resizeComb() {
  float variation = m_parameters.variation + 1.f;
  for (int i = 0; i < 2; i++) {
    float time = m_parameters.comb_time;
    for (int j = 0; j < 6; j++) {
      m_combs[i][j].resize(time);
      time += time * (variation / 10.);
    }
  }
}

void StereoMoorer::setSampleRate(float sample_rate) {
  for (int i = 0; i < 2; i++) {
    // process combs
    m_allpasses[i].setSampleRate(sample_rate);
    for (int j = 0; j < 6; j++) {
      m_combs[i][j].setSampleRate(sample_rate);
    }
  }
}

void StereoMoorer::processCombs(std::array<float, 2> inputs) {
  for (int i = 0; i < 2; i++) {
    // process combs
    if (m_parameters.freeze) {
      for (int j = 0; j < 6; j++) {
        m_combs_status[i][j] = m_combs[i][j].processFreezed();
      }
    } else {
      for (int j = 0; j < 6; j++) {
        m_combs_status[i][j] = m_combs[i][j].process(inputs[i]);
      }
    }
  }
}

void StereoMoorer::spatializeCombs() {
  for (int i = 0; i < 6; i++) {
    // pan combs output, mix the two channels
    float in_left = m_combs_status[0][i];
    float in_right = m_combs_status[1][i];
    // out_left = in_left * pan + in_right * (1 - pan);
    m_combs_status[0][i] =
        noi::Outils::linearCrossfade(in_left, in_right, m_pan_coefs[i]);
    m_combs_status[1][i] =
        noi::Outils::linearCrossfade(in_left, in_right, (1.f - m_pan_coefs[i]));
  }
}

void StereoMoorer::processAllPasses(std::array<float, 2> inputs) {
  for (int i = 0; i < 2; i++) {
    m_allpasses_status[i] = m_allpasses[i].process(inputs[i]);
  }
}

std::array<float, 2> StereoMoorer::processStereo(std::array<float, 2> inputs) {
  std::array<float, 2> outputs = {0, 0};
  processAllPasses(inputs);
  processCombs(m_allpasses_status);
  spatializeCombs();

  // Sum all combs for each channels
  for (int i = 0; i < 2; i++) {
    float comb_sum = 0.f;
    for (auto j : m_combs_status[i]) {
      comb_sum += j;
    }
    comb_sum /= 6.f;
    // add input[i] to avoid the phased opposed feedforward
    if (!m_parameters.freeze) {
      comb_sum = (comb_sum + inputs[i]) / 2;
      comb_sum *= 2.0;
    }
    outputs[i] = 5 * noi::Outils::equalPowerCrossfade(inputs[i], comb_sum,
                                                      m_parameters.dry_wet);
  }
  return outputs;
}

}  // namespace noi
