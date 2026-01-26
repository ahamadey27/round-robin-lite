/*
  ==============================================================================

    MIDI Note Mapping System for Round Robin Lite

    Maps 10 key pairs to pitch offsets optimized for footstep sounds.
    Each pair consists of two adjacent white keys for L/R alternation.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
 * MidiMapper - Static utility class for Round Robin Lite's key pair system.
 *
 * KEY PAIR CONCEPT:
 * Instead of mapping samples across a full chromatic keyboard, Round Robin Lite
 * uses PAIRS of white keys. This allows users to easily alternate between two
 * notes to create realistic footstep patterns (left foot, right foot).
 *
 * MAPPING TABLE:
 * Pair Index | Keys     | MIDI Notes | Semitone Offset | Use Case
 * -----------|----------|------------|-----------------|------------------
 * 0          | C2/D2    | 36/38      | -12             | Very low (octave down)
 * 1          | E2/F2    | 40/41      | -10             | Low pitch
 * 2          | G2/A2    | 43/45      | -9              | Low-mid pitch
 * 3          | B2/C3    | 47/49      | -7              | Mid-low pitch
 * 4          | E3/F3    | 52/53      | -4              | Mid pitch
 * 5          | G3/A3    | 55/57      | -3              | Mid-high pitch
 * 6          | B3/C4    | 59/60      | -1              | High-mid pitch
 * 7          | C4/D4    | 60/62      | 0 (ROOT)        | Original pitch (Middle C) ★
 * 8          | E4/F4    | 64/65      | +2              | High pitch
 * 9          | G4/A4    | 67/69      | +5              | Very high pitch
 *
 * ROOT NOTE: C4/Middle C (MIDI 60) is the reference pitch where samples play unmodified.
 * This is the industry standard reference point used by most samplers and DAWs.
 */

class MidiMapper
{
public:
    //==============================================================================
    // Constants

    static constexpr int NUM_KEY_PAIRS = 10;        // Total number of key pairs
    static constexpr int ROOT_PAIR_INDEX = 7;       // C4/D4 pair (Middle C - no pitch shift)
    static constexpr int ROOT_MIDI_NOTE = 60;       // C4 (Middle C) is the root reference pitch

    //==============================================================================
    /**
     * Get the two MIDI note numbers for a given key pair index.
     *
     * @param pairIndex The key pair index (0-9)
     * @param note1 [out] First MIDI note of the pair
     * @param note2 [out] Second MIDI note of the pair
     * @return true if pairIndex is valid (0-9), false otherwise
     *
     * Example:
     *   int n1, n2;
     *   MidiMapper::getMidiNotesForPair(7, n1, n2);
     *   // Result: n1 = 60 (C4/Middle C), n2 = 62 (D4)
     */
    static bool getMidiNotesForPair(int pairIndex, int& note1, int& note2)
    {
        if (pairIndex < 0 || pairIndex >= NUM_KEY_PAIRS)
            return false;

        // Define the complete mapping table
        // Each pair: {first note, second note}
        static const int pairMapping[NUM_KEY_PAIRS][2] =
        {
            { 36, 38 },  // Pair 0: C2/D2
            { 40, 41 },  // Pair 1: E2/F2
            { 43, 45 },  // Pair 2: G2/A2
            { 47, 49 },  // Pair 3: B2/C3
            { 52, 53 },  // Pair 4: E3/F3
            { 55, 57 },  // Pair 5: G3/A3
            { 59, 60 },  // Pair 6: B3/C4
            { 60, 62 },  // Pair 7: C4/D4 (ROOT - Middle C)
            { 64, 65 },  // Pair 8: E4/F4
            { 67, 69 }   // Pair 9: G4/A4
        };

        note1 = pairMapping[pairIndex][0];
        note2 = pairMapping[pairIndex][1];
        return true;
    }

    //==============================================================================
    /**
     * Get the semitone offset for a given key pair index.
     *
     * The offset determines how much to pitch-shift the sample when played.
     * Positive = higher pitch, Negative = lower pitch, 0 = original pitch
     *
     * @param pairIndex The key pair index (0-9)
     * @return Semitone offset (-12 to +5), or 0 if invalid index
     *
     * Example:
     *   int offset = MidiMapper::getSemitoneOffsetForPair(0);
     *   // Result: -12 (pair 0 plays one octave lower than root)
     */
    static int getSemitoneOffsetForPair(int pairIndex)
    {
        if (pairIndex < 0 || pairIndex >= NUM_KEY_PAIRS)
            return 0;

        // Semitone offsets for each pair relative to root (Middle C = MIDI 60)
        static const int semitoneOffsets[NUM_KEY_PAIRS] =
        {
            -12,  // Pair 0: C2/D2 (octave down)
            -10,  // Pair 1: E2/F2
            -9,   // Pair 2: G2/A2
            -7,   // Pair 3: B2/C3
            -4,   // Pair 4: E3/F3
            -3,   // Pair 5: G3/A3
            -1,   // Pair 6: B3/C4
             0,   // Pair 7: C4/D4 (ROOT - Middle C, no pitch shift)
            +2,   // Pair 8: E4/F4
            +5    // Pair 9: G4/A4
        };

        return semitoneOffsets[pairIndex];
    }

    //==============================================================================
    /**
     * Find which key pair (if any) a MIDI note belongs to.
     *
     * @param midiNoteNumber The MIDI note to check (0-127)
     * @return The pair index (0-9) if found, or -1 if not part of any pair
     *
     * Example:
     *   int pair = MidiMapper::getPairIndexForMidiNote(60);
     *   // Result: 7 (C4/Middle C is in pair 7)
     *   // Note: Also returns 6 because 60 appears in both pair 6 and 7
     *   // In this case, returns the FIRST match (pair 6)
     */
    static int getPairIndexForMidiNote(int midiNoteNumber)
    {
        // Check each pair to see if this note belongs to it
        for (int pairIndex = 0; pairIndex < NUM_KEY_PAIRS; ++pairIndex)
        {
            int note1, note2;
            getMidiNotesForPair(pairIndex, note1, note2);

            if (midiNoteNumber == note1 || midiNoteNumber == note2)
                return pairIndex;
        }

        return -1;  // Not found in any pair
    }

    //==============================================================================
    /**
     * Check if a MIDI note is part of the Round Robin Lite key pairs.
     *
     * @param midiNoteNumber The MIDI note to check (0-127)
     * @return true if this note is in one of the 10 key pairs
     */
    static bool isMidiNoteInMapping(int midiNoteNumber)
    {
        return getPairIndexForMidiNote(midiNoteNumber) != -1;
    }

    //==============================================================================
    /**
     * Validate that a pair index is within valid range.
     *
     * @param pairIndex The index to validate
     * @return true if pairIndex is between 0 and 9 (inclusive)
     */
    static bool isValidPairIndex(int pairIndex)
    {
        return pairIndex >= 0 && pairIndex < NUM_KEY_PAIRS;
    }

    //==============================================================================
    /**
     * Get the root MIDI note for a given pair index.
     * This is the note at which the sample should play at its original pitch.
     *
     * @param pairIndex The key pair index (0-9)
     * @return The root MIDI note (e.g., 60 for Middle C adjusted by semitone offset)
     */
    static int getRootNoteForPair(int pairIndex)
    {
        if (!isValidPairIndex(pairIndex))
            return ROOT_MIDI_NOTE;

        // Root note is Middle C (60) adjusted by the pair's semitone offset
        return ROOT_MIDI_NOTE + getSemitoneOffsetForPair(pairIndex);
    }

    //==============================================================================
    /**
     * Get a human-readable name for a key pair.
     * Useful for UI display and debugging.
     *
     * @param pairIndex The key pair index (0-9)
     * @return String like "C2/D2" or "C4/D4 (Root)"
     */
    static juce::String getKeyPairName(int pairIndex)
    {
        if (!isValidPairIndex(pairIndex))
            return "Invalid";

        static const juce::String pairNames[NUM_KEY_PAIRS] =
        {
            "C2/D2",
            "E2/F2",
            "G2/A2",
            "B2/C3",
            "E3/F3",
            "G3/A3",
            "B3/C4",
            "C4/D4 (Root)",  // Middle C - the reference pitch
            "E4/F4",
            "G4/A4"
        };

        return pairNames[pairIndex];
    }

    //==============================================================================
    /**
     * Print the complete mapping table to debug output.
     * Useful for verification and debugging.
     */
    static void printMappingTable()
    {
        DBG("========================================");
        DBG("Round Robin Lite - MIDI Note Mapping");
        DBG("========================================");

        for (int i = 0; i < NUM_KEY_PAIRS; ++i)
        {
            int note1, note2;
            getMidiNotesForPair(i, note1, note2);

            int offset = getSemitoneOffsetForPair(i);
            juce::String offsetStr = (offset > 0 ? "+" : "") + juce::String(offset);

            DBG("Pair " + juce::String(i) + ": " +
                getKeyPairName(i) +
                " | MIDI: " + juce::String(note1) + "/" + juce::String(note2) +
                " | Offset: " + offsetStr + " semitones");
        }

        DBG("========================================");
    }

private:
    // This is a static utility class - no instances should be created
    MidiMapper() = delete;
    ~MidiMapper() = delete;
};