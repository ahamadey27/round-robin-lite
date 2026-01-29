/*
  ==============================================================================

    ParameterIDs.h
    Created: 28 Jan 2026 3:38:18pm
    Author:  hamad

    Parameter ID Constants for Round Robin Lite

    This file defines all parameter identifiers used with AudioProcessorValueTreeState.
    These IDs are used for:
    - DAW automation
    - Preset saving/loading
    - Parameter binding to UI components

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
 * ParameterIDs Namespace
 *
 * Contains all parameter ID string constants for Round Robin Lite.
 * Using a namespace prevents naming conflicts and keeps IDs organized.
 */
namespace ParameterIDs
{
    //==============================================================================
    // PITCH CONTROL PARAMETERS (Global - affects all loaded samples)

    /**
     * Semitone Pitch Parameter
     * Range: -12 to +12 semitones
     * Step: 1 semitone
     * Default: 0 (no transposition)
     * Purpose: Globally transpose all loaded samples in semitone increments
     */
    inline const juce::String SEMITONE{ "semitone" };

    /**
     * Fine Tune Pitch Parameter (in cents)
     * Range: -100 to +100 cents
     * Step: 1 cent
     * Default: 0 (no fine tuning)
     * Purpose: Fine-tune global pitch in cent increments (100 cents = 1 semitone)
     */
    inline const juce::String FINE_TUNE{ "fineTune" };

    //==============================================================================
    // VOLUME CONTROL

    /**
     * Master Volume Parameter
     * Range: 0.0 to 1.0 (linear) OR -60dB to 0dB (decibel scale - TBD in Step 3)
     * Default: 0.75 (or -6dB)
     * Purpose: Control overall output level of the plugin
     */
    inline const juce::String VOLUME{ "volume" };

    //==============================================================================
    // 3-BAND EQ PARAMETERS

    /**
     * Low Band EQ Gain
     * Range: -24dB to +24dB
     * Default: 0dB (no boost/cut)
     * Purpose: Boost or cut low frequency content
     */
    inline const juce::String LOW_EQ_GAIN{ "lowEqGain" };

    /**
     * Low Band EQ Frequency
     * Range: 20Hz to 500Hz
     * Default: 100Hz
     * Scaling: Logarithmic (use skew factor for natural frequency response)
     * Purpose: Set the center frequency for low band EQ
     */
    inline const juce::String LOW_EQ_FREQ{ "lowEqFreq" };

    /**
     * Mid Band EQ Gain
     * Range: -24dB to +24dB
     * Default: 0dB (no boost/cut)
     * Purpose: Boost or cut mid frequency content
     */
    inline const juce::String MID_EQ_GAIN{ "midEqGain" };

    /**
     * Mid Band EQ Frequency
     * Range: 200Hz to 5000Hz
     * Default: 1000Hz
     * Scaling: Logarithmic (use skew factor for natural frequency response)
     * Purpose: Set the center frequency for mid band EQ
     */
    inline const juce::String MID_EQ_FREQ{ "midEqFreq" };

    /**
     * High Band EQ Gain
     * Range: -24dB to +24dB
     * Default: 0dB (no boost/cut)
     * Purpose: Boost or cut high frequency content
     */
    inline const juce::String HIGH_EQ_GAIN{ "highEqGain" };

    /**
     * High Band EQ Frequency
     * Range: 2000Hz to 20000Hz
     * Default: 5000Hz
     * Scaling: Logarithmic (use skew factor for natural frequency response)
     * Purpose: Set the center frequency for high band EQ
     */
    inline const juce::String HIGH_EQ_FREQ{ "highEqFreq" };

    //==============================================================================
    // TRANSIENT SHAPING PARAMETERS

    /**
     * Transient Attack Parameter
     * Range: -127 to +127
     * Default: 0 (no change to attack)
     * Scaling: Linear
     * Purpose: Enhance or reduce the attack/transient portion of samples
     * Negative values: Soften attack
     * Positive values: Enhance attack
     */
    inline const juce::String TRANSIENT_ATTACK{ "transientAttack" };

    /**
     * Transient Decay Parameter
     * Range: -127 to +127
     * Default: 0 (no change to decay)
     * Scaling: Linear
     * Purpose: Enhance or reduce the decay/sustain portion of samples
     * Negative values: Reduce sustain/body
     * Positive values: Enhance sustain/body
     */
    inline const juce::String TRANSIENT_DECAY{ "transientDecay" };

    //==============================================================================
    // PARAMETER COUNT

    /**
     * Total number of parameters in the plugin
     * Used for validation and UI layout
     */
    static constexpr int TOTAL_PARAM_COUNT = 11;

} // namespace ParameterIDs
