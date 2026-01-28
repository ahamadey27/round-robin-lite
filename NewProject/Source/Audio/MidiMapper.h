/*
  ==============================================================================

    MIDI Note Mapping System for Round Robin Lite

    Simplified: All samples triggered by C2/D2 at original pitch.
    Pitch shifting will be controlled globally via plugin parameters.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
 * MidiMapper - Static utility class for Round Robin Lite's MIDI system.
 *
 * SIMPLIFIED DESIGN:
 * - All samples are triggered by C2 and D2 (MIDI 36/38)
 * - Both keys play samples at their original recorded pitch
 * - The two keys allow for L/R alternation (e.g., left foot, right foot)
 * - Global pitch shifting is handled by plugin parameters (Phase 3)
 *
 * MIDI MAPPING:
 * C2 (MIDI 36) → Triggers all loaded samples at original pitch
 * D2 (MIDI 38) → Triggers all loaded samples at original pitch
 */

class MidiMapper
{
public:
    //==============================================================================
    // Constants

    static constexpr int TRIGGER_NOTE_1 = 36;  // C2
    static constexpr int TRIGGER_NOTE_2 = 38;  // D2

    //==============================================================================
    /**
     * Check if a MIDI note is one of our trigger notes.
     * @param midiNoteNumber The MIDI note to check (0-127)
     * @return true if this is C2 (36) or D2 (38)
     */
    static bool isTriggerNote(int midiNoteNumber)
    {
        return (midiNoteNumber == TRIGGER_NOTE_1 || midiNoteNumber == TRIGGER_NOTE_2);
    }

    //==============================================================================
    /**
     * Get human-readable name for trigger notes.
     * @return "C2/D2"
     */
    static juce::String getTriggerNoteName()
    {
        return "C2/D2";
    }

    //==============================================================================
    /**
     * Print mapping info to debug output.
     */
    static void printMappingInfo()
    {
        DBG("========================================");
        DBG("Round Robin Lite - MIDI Mapping");
        DBG("========================================");
        DBG("Trigger Notes: C2 (MIDI 36) and D2 (MIDI 38)");
        DBG("All samples play at original pitch");
        DBG("Use global pitch controls for transposition");
        DBG("========================================");
    }

private:
    // This is a static utility class - no instances should be created
    MidiMapper() = delete;
    ~MidiMapper() = delete;
};