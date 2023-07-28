/*
  ==============================================================================

    Sinensis.cpp
    Created: 4 Mar 2023 5:01:37pm
    Author:  thoma

  ==============================================================================
*/

#include "Sinensis.h"

Sinensis::Sinensis() {
    m_parameters = { 0, 0, 218.f, 0.707f, 0.f, 1.5f, 0.4f, 0.4f };
    m_sampling_frequency = 48000;
    computeFrequencyMidiOff();
    computeGain();
    computeQ();
    prepareBpf();
}


Sinensis::~Sinensis() {

}

void Sinensis::setParameters(Sinensis::Parameters parameters) {
    m_parameters = parameters;
}

float Sinensis::processSample(float input) {
    float output = 0;
    for (int i = 0; i < 6; i++) {
        output += m_bpf[i].process(input) * m_gain[i];
    }
    attenuate(output);
    //saturate(output);
    return output;
}

float Sinensis::processSinensis(float input, juce::MidiBuffer& midi_buffer) {
 
    switch (m_parameters.midi_mode) {
    case 0:prepareMidiOff(); break;
    case 1:prepareMidiMono(midi_buffer); break;
    case 2:prepareMidiPoly(midi_buffer); break;
    }
    return processSample(input);
}

void Sinensis::attenuate(float& input) {
    input *= m_parameters.output_volume;
}

void Sinensis::saturate(float& input) {
    float max = 1.f;
    float threshold = 0.9;
    if ((input < -threshold && m_saturation_memory < -threshold) || (input > threshold && m_saturation_memory > threshold)) {
        float slope = input - m_saturation_memory; //one sample period as unity time
        float offset = input < 0.f ? -threshold : threshold;
        slope *= 1.f - ((m_saturation_memory + offset) * 10.f); //higher input higher divider
        input = m_saturation_memory + slope;
    }
    //additionnal clipping
    if (input < -max) input = -max;
    if (input > max) input = max;

    m_saturation_memory = input;
}



void Sinensis::prepareMidiOff() {
    computeFrequencyMidiOff();
    computeGain();
    computeQ();
    prepareBpf();
}

void Sinensis::prepareMidiMono(juce::MidiBuffer& midi_buffer) {
    extractMidiMono(midi_buffer);
    computeFrequencyMidiMono();
    computeEnvelopesStep();
    processEnvelopeMono();
    computeGain();
    for (int i = 0; i < 6; i++) {
        m_gain[i] *= m_envelope_statut[0];
    }
    computeQ();
    prepareBpf();
}
void Sinensis::prepareMidiPoly(juce::MidiBuffer& midi_buffer) {
    extractMidiPoly(midi_buffer);
    computeFrequencyMidiPoly();
    computeEnvelopesStep();
    for (int i = 0; i < 6; i++) {
        processEnvelopePoly(i);
        m_gain[i] = m_envelope_statut[i];
    }
    computeQ();
    prepareBpf();
}

void Sinensis::computeFrequencyMidiOff() {

    for (int i = 0; i < 6; i++) {
        //multiply frequence by ratio
        float thisBandFreq = m_parameters.root_frequency;
        for (int j = 1; j <= i; j++) { thisBandFreq *= m_parameters.ratio; }
        thisBandFreq = truncf(thisBandFreq);
        //frequency folding
        while (thisBandFreq > 15000.f || thisBandFreq < 30.f) {
            if (thisBandFreq > 15000.F) { thisBandFreq = 15000.f - (thisBandFreq - 15000.f); }
            if (thisBandFreq < 30.f) { thisBandFreq = 30.f + (30.f - thisBandFreq); }
        }
        m_frequency[i] = thisBandFreq;
    }

}

void Sinensis::computeFrequencyMidiMono() {
    if (m_notes.getLast() == 0) return;
    m_parameters.root_frequency = juce::MidiMessage::getMidiNoteInHertz(m_notes.getLast());
    computeFrequencyMidiOff();

}

void Sinensis::computeFrequencyMidiPoly() {
    for (int i = 0; i < 6; i++) {
        if (m_notes[i] != 0) {
            m_frequency[i] = juce::MidiMessage::getMidiNoteInHertz(m_notes[i]);
        }
    }
}

void Sinensis::computeGain() {
 
  switch (m_parameters.band_selector_mode){
  case 0:computeLowHigh(); break;
  case 1:computeOddEven(); break;
  case 2:computePeak(); break;
  }

}

void Sinensis::computeLowHigh() {
    float band_selector = m_parameters.band_selector;
    float alpha = band_selector * 4.0f - 2.0f;
    float beta = -2.0f*(band_selector * band_selector) + 1.0f;

    for (int i = 0; i < 6; i++) {
        float this_band_index = static_cast<float> (i);
        this_band_index /= 5.0f;
        float this_band_gain = this_band_index * alpha + beta;
        if (this_band_gain < 0.0f) {
            this_band_gain = 0.0f;
        }
        m_gain[i] = this_band_gain;
    }
}

void Sinensis::computeOddEven() {
    for (int i = 0; i < 6; i++) {
        float this_band_gain = 0;
        //odd case
        if (i == 0 || i == 2 || i == 4) {
            this_band_gain = m_parameters.band_selector;
        }
        //even case
        if (i == 1 || i == 3 || i == 5) {
            this_band_gain = 1.0f - m_parameters.band_selector;
        }
        m_gain[i] = this_band_gain;
    }
}

void Sinensis::computePeak() {
        float cursor = m_parameters.band_selector;
        cursor *= 5.;
        for (int i = 0; i < 6; i++) {
            float band_index = static_cast<float> (i);
            float band_gain = 0.0f;
            if (cursor < band_index) {
                band_gain = band_index - cursor;
            }
            else { band_gain = cursor - band_index; }
            
            band_gain = 1 - band_gain;
            if (band_gain > 1.) { band_gain = 1.; }
            if (band_gain < 0.) { band_gain = 0.; }
            m_gain[i] = band_gain;
        }
}

void Sinensis::processEnvelopePoly(int i) {
    if (m_notes[i] == 0) m_envelope_statut[i] -= m_decay_step;
    else m_envelope_statut[i] += m_attack_step;
    if (m_envelope_statut[i] > 1) m_envelope_statut[i] = 1;
    if (m_envelope_statut[i] < 0) m_envelope_statut[i] = 0;
}

void Sinensis::processEnvelopeMono() {
    if (m_notes.getLast() == 0) m_envelope_statut[0] -= m_decay_step;
    else m_envelope_statut[0] += m_attack_step;
    if (m_envelope_statut[0] > 1) m_envelope_statut[0] = 1;
    if (m_envelope_statut[0] < 0) m_envelope_statut[0] = 0;
}

void Sinensis::computeEnvelopesStep() {
    m_attack_step = 1.f / (m_sampling_frequency * m_parameters.attack);
    m_decay_step = 1.f / (m_sampling_frequency * m_parameters.decay);
}


void Sinensis::computeQ() {
    for (int i = 0; i < 6; i++) {
        float Q = (m_parameters.resonance * m_gain[i]) + 0.707f;
        if (Q > 32.) Q = 32.0f;
        m_Q[i] = Q;
    }
}

void Sinensis::prepareBpf() {
    for (int i = 0; i < 6; i++) {
        m_bpf[i].setParam(m_frequency[i], m_Q[i]);
    }
}

void Sinensis::extractMidiMono(juce::MidiBuffer& midi_buffer) {

    for (const auto metadata : midi_buffer)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn()) m_notes.add(msg.getNoteNumber());
        else if (msg.isNoteOff()) m_notes.removeValue(msg.getNoteNumber());
    }
}

void Sinensis::extractMidiPoly(juce::MidiBuffer& midi_buffer) {

    for (const auto metadata : midi_buffer)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn()) m_notes.add(msg.getNoteNumber());
        else if (msg.isNoteOff()) m_notes.removeValue(msg.getNoteNumber());
    }

}

void Sinensis::setSamplingFrequency(float sampling_frequency) {
    for (auto bpf : m_bpf) bpf.setSamplingFrequency(sampling_frequency);
    m_sampling_frequency = sampling_frequency;
}

