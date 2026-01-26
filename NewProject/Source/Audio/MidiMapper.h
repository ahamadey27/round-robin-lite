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
 * 0          | C0/D0    | 36/38      | -7              | Very low pitch
 * 1          | E0/F0    | 40/41      | -6              | Low pitch
 * 2          | G0/A0    | 43/45      | -5              | Low-mid pitch
 * 3          | B0/C1    | 47/48      | -4              | Mid-low pitch
 * 4          | E1/F1    | 52/53      | -3              | Mid pitch
 * 5          | G1/A1    | 55/57      | -2              | Mid-high pitch
 * 6          | B1/C2    | 59/60      | -1              | High-mid pitch
 * 7          | C2/D2    | 48/50      | 0 (ROOT)        | Original pitch ★
 * 8          | E2/F2    | 64/65      | +1              | High pitch
 * 9          | G2/A2    | 67/69      | +2              | Very high pitch
 *
 * ROOT NOTE: C2 (MIDI 48) is the reference pitch where samples play unmodified.
 */

class MidiMapper
{
public:
    //==============================================================================
    // Constants

    static constexpr int NUM_KEY_PAIRS = 10;        // Total number of key pairs
    static constexpr int ROOT_PAIR_INDEX = 7;       // C2/D2 pair (no pitch shift)
    static constexpr int ROOT_MIDI_NOTE = 48;       // C2 is the root reference pitch

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
     *   // Result: n1 = 48 (C2), n2 = 50 (D2)
     */
    static bool getMidiNotesForPair(int pairIndex, int& note1, int& note2)
    {
        if (pairIndex < 0 || pairIndex >= NUM_KEY_PAIRS)
            return false;

        // Define the complete mapping table
        // Each pair: {first note, second note}
        static const int pairMapping[NUM_KEY_PAIRS][2] =
        {
            { 36, 38 },  // Pair 0: C0/D0
            { 40, 41 },  // Pair 1: E0/F0
            { 43, 45 },  // Pair 2: G0/A0
            { 47, 48 },  // Pair 3: B0/C1
            { 52, 53 },  // Pair 4: E1/F1
            { 55, 57 },  // Pair 5: G1/A1
            { 59, 60 },  // Pair 6: B1/C2
            { 48, 50 },  // Pair 7: C2/D2 (ROOT - note the duplicate C2!)
            { 64, 65 },  // Pair 8: E2/F2
            { 67, 69 }   // Pair 9: G2/A2
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
     * @return Semitone offset (-7 to +2), or 0 if invalid index
     *
     * Example:
     *   int offset = MidiMapper::getSemitoneOffsetForPair(0);
     *   // Result: -7 (pair 0 plays 7 semitones lower than root)
     */
    static int getSemitoneOffsetForPair(int pairIndex)
    {
        if (pairIndex < 0 || pairIndex >= NUM_KEY_PAIRS)
            return 0;

        // Semitone offsets for each pair relative to root (C2)
        static const int semitoneOffsets[NUM_KEY_PAIRS] =
        {
            -7,  // Pair 0: C0/D0
            -6,  // Pair 1: E0/F0
            -5,  // Pair 2: G0/A0
            -4,  // Pair 3: B0/C1
            -3,  // Pair 4: E1/F1
            -2,  // Pair 5: G1/A1
            -1,  // Pair 6: B1/C2
             0,  // Pair 7: C2/D2 (ROOT - no pitch shift)
            +1,  // Pair 8: E2/F2
            +2   // Pair 9: G2/A2
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
     *   int pair = MidiMapper::getPairIndexForMidiNote(48);
     *   // Result: 7 (C2 is in pair 7)
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
     * @return The root MIDI note (e.g., 48 for C2 adjusted by semitone offset)
     */
    static int getRootNoteForPair(int pairIndex)
    {
        if (!isValidPairIndex(pairIndex))
            return ROOT_MIDI_NOTE;

        // Root note is C2 (48) adjusted by the pair's semitone offset
        return ROOT_MIDI_NOTE + getSemitoneOffsetForPair(pairIndex);
    }

    //==============================================================================
    /**
     * Get a human-readable name for a key pair.
     * Useful for UI display and debugging.
     *
     * @param pairIndex The key pair index (0-9)
     * @return String like "C0/D0" or "C2/D2 (Root)"
     */
    static juce::String getKeyPairName(int pairIndex)
    {
        if (!isValidPairIndex(pairIndex))
            return "Invalid";

        static const juce::String pairNames[NUM_KEY_PAIRS] =
        {
            "C0/D0",
            "E0/F0",
            "G0/A0",
            "B0/C1",
            "E1/F1",
            "G1/A1",
            "B1/C2",
            "C2/D2 (Root)",  // Special label for root pair
            "E2/F2",
            "G2/A2"
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