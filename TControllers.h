/* -*- c++ -*- */
#pragma once

#include "utils.h"
#include "TMemory.h"
#include "TKnobs.h"
#include "TGui.h"
#include "TMidi.h"

/**
 * Keep controller (parameters/knobs) state. Receive change events,
 * make the appropriate calls.
 *
 * Status: This is a sketch. Functionality is missing.
 */
class TControllers
{
public:
    const static uint8_t MIN = 0;
    const static uint8_t MAX = 0x7f;

    TControllers(IMidi& midiOutput) :
        ActiveKnob(0),
        MidiOutput(midiOutput)
    {
        Load();
    }

    /// Load the controller scene from flash.
    void Load();
    const TParamScene& GetScene() const { return Scene; }
    uint8_t GetValue(int knob) const { return Values[knob]; }
    int GetActiveKnob() const { return ActiveKnob; }

    void IncreaseValue(int knob, uint8_t v) {
        uint8_t value = Values[knob] + v;
        if (value > MAX) value = MAX;
        SetValue(knob, value);
    }
    void DecreaseValue(int knob, uint8_t v) {
        uint8_t value = Values[knob] - v;
        if (value > MAX) value = MIN;
        SetValue(knob, value);
    }
    void SetValue(int knob, uint8_t v) {
        Values[knob] = v;
        ActiveKnob = knob;
        if (Mode == MODE_CONTROLLER) {
            Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 255-2*Values[knob];
            Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 2*Values[knob];
            Gui.UpdateLine(3);
            Gui.UpdateLine(4);
            Gui.UpdateLine(5);
            Gui.UpdateLine(6);
        }
        SendMidi(knob);
    }

    /**
     * Output the current PWM values to knob LEDs
     */
    void UpdateKnobs() {
        if (Mode == MODE_CONTROLLER) {
            for (int knob = 0; knob < TKnobs::Knobs; knob++) {
                Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 255-2*Values[knob];
                Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 2*Values[knob];
            }
        }
    }

    void MidiEvent(const TMidiEvent& event);

    uint8_t Values[KNOBS];

private:
    void SendMidi(int knob)
    {
        TMidiEvent event = {{ (uint8_t)(TMidiEvent::MIDI_CC | Scene.Knobs[knob].Channel), Scene.Knobs[knob].Param, Values[knob] }};
        MidiOutput.SendEvent(event);
    }

    TParamScene Scene;
    uint8_t ActiveKnob;
    IMidi& MidiOutput;
};

extern TControllers Controllers;
