/*
  ==============================================================================

    Sinensis.h
    Created: 4 Mar 2023 5:01:48pm
    Author:  thoma

  ==============================================================================
*/

#pragma once
#include "Filter.h"
#include "JuceHeader.h"
#include <vector>

class MonoMidiNote {
private:
    std::vector<int> m_note = {};

public:
    MonoMidiNote() { m_note.push_back(0); }

    int getNote() {
        return m_note.back();
    }

    void addNote(int note) {
        m_note.push_back(note);
    }
    bool noteIsNew(int note) {
        for (auto note_iterator : m_note) {
            if (note_iterator == note) return false;
        }
        return true;
    }
    void eraseNote(int note) {
        for (std::vector<int>::iterator it = m_note.begin(); it != m_note.end(); it++)
        {
            if (*it == note) {
                it = m_note.erase(it);
                return;
            }
        }
    }
    void computeMidi(juce::MidiBuffer& midi_buffer) {
        for (const auto metadata : midi_buffer)
        {
            const auto msg = metadata.getMessage();
            if (msg.isNoteOn()) {
                if (noteIsNew(msg.getNoteNumber())) addNote(msg.getNoteNumber());
            }
            else if (msg.isNoteOff()) eraseNote(msg.getNoteNumber());
        }
    }

};

class Sinensis {
public:
    ///<summary> Parameters of Sinensis </summary>
        /// <param name = "frequency"> </param>
        /// @param float Q
        /// @param float band_selector
        /// @param float ratio
        /// @param bool gain_Q_link
        /// @param float band_selector_mode
    struct Parameters{
        int midi_mode;
        int band_selector_mode;
        float root_frequency;
        float resonance;
        float band_selector;
        float ratio;
        float attack;
        float decay;
        float output_volume;
    };
    Sinensis();
    ~Sinensis();
    void setParameters(Sinensis::Parameters parameters);
    float processSample(float input);
    float processSinensis(float input, juce::MidiBuffer& midi_buffer);
    void attenuate(float& input);
    void saturate(float& input);
    void prepareMidiOff();
    void prepareMidiMono(juce::MidiBuffer& midi_buffer);
    void prepareMidiPoly(juce::MidiBuffer& midi_buffer);
    

    void computeGain();
        void computeLowHigh();
        void computeOddEven();
        void computePeak();
        void processEnvelopePoly(int envelope_index);
        void processEnvelopeMono();
    void computeEnvelopesStep();

    void computeQ();
    void prepareBpf();
        void computeFrequencyMidiOff();
        void computeFrequencyMidiMono();
        void computeFrequencyMidiPoly();

    void extractMidiMono(juce::MidiBuffer& midi_buffer);
    void extractMidiPoly(juce::MidiBuffer& midi_buffer);

    void setSamplingFrequency(float sampling_frequency);

private:
    Sinensis::Parameters m_parameters;
    noi::Filter::BPF m_bpf[6];
    float m_sampling_frequency;
    float m_Q[6];
    float m_frequency[6];
    float m_gain[6];
    float m_attack_step;
    float m_decay_step;
    float m_envelope_statut[6];
    float m_saturation_memory;
    //int m_notes[6];
    juce::SortedSet<int> m_notes;
    MonoMidiNote monophonic_midi_buffer;

};