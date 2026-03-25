# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Round Robin Lite is a monophonic MIDI sampler VST3/AU plugin for footstep sounds, built with JUCE 8+ and C++17. It uses a paired-key MIDI mapping system (10 pairs of adjacent white keys) for alternating left/right footstep patterns. Samples play unpitched at their original sample rate regardless of which MIDI key triggers them.

## Build

Requires JUCE installed at `/Users/alex/Documents/JUCE` (hardcoded in `NewProject/CMakeLists.txt`).

```bash
cd NewProject
cmake -B build
cmake --build build
```

Output copies automatically to `~/Library/Audio/Plug-Ins/VST3/round-robin-lite.vst3`.

Formats: VST3, AU, Standalone. There is also a Projucer file (`NewProject/NewProject.jucer`) and Xcode project under `NewProject/Builds/MacOSX/`.

No automated tests exist. Test manually via JUCE AudioPluginHost or any DAW.

## Architecture

Standard JUCE Synthesiser pattern:

- **PluginProcessor** (`Source/PluginProcessor.h/cpp`) — Main audio engine. Owns the `juce::Synthesiser`, `AudioProcessorValueTreeState` (APVTS), DSP processors, `SampleSlot[20]` array, and `SampleLoader`. `processBlock()` renders audio and applies the DSP chain.
- **PluginEditor** (`Source/PluginEditor.h/cpp`) — GUI. Custom Look-and-Feel classes for knobs/sliders. 700×566px canvas.
- **RRVoice** (`Source/Audio/RRvoice.h/cpp`) — `SynthesiserVoice` subclass. Handles sample playback, pitch shifting, ADSR envelope, and per-note randomization.
- **RRSound** (`Source/Audio/RRSound.h/cpp`) — `SynthesiserSound` subclass. Holds one audio buffer per sample.
- **MidiMapper** (`Source/Audio/MidiMapper.h`) — Static utility mapping 10 key pairs to MIDI notes. Root note = C1 (MIDI 36).
- **SampleSlot/SampleLoader** (`Source/Data/`) — Sample loading with stereo-to-mono conversion and resampling.
- **RandomizationEngine** (`Source/DSP/RandomizationEngine.h/cpp`) — Asymmetric per-note randomization (separate negative/positive ranges).
- **ParametersIDs** (`Source/Parameters/ParametersIDs.h`) — All 41 parameter ID string constants.

### DSP Chain (`Source/DSP/`)

- **ToneControl** — Simple 2-knob low/high shelf EQ (250Hz/4kHz, ±12dB). Fully integrated (DSP, voice randomization, UI).
- **ThreeBandEQ** — Full 3-band EQ. Code exists but is commented out in the Lite version (reserved for Pro).
- **TransientShaper** — Attack/decay transient processor. Also commented out in Lite.

### UI (`Source/UI/`)

- **RRLookAndFeel** — Custom `LookAndFeel_V4` subclasses: `RRKnobLAF` (rotary knobs), `RRNegSliderLAF`/`RRPosSliderLAF` (blue/red randomization range sliders), `RRToggleLAF` (Series/Random toggle).

## Key Design Decisions

- **Monophonic:** Only one voice/sample plays at a time.
- **Unpitched playback:** MIDI key determines which sample, not pitch. Pitch control is global only (semitone + fine tune).
- **Playback modes:** Series (round-robin) or Random (Fisher-Yates shuffle, no repeats until all played).
- **Randomization:** Every parameter has 4 associated randomization params (neg range, pos range for asymmetric variation). Values generated per note-on in `RRVoice::startNote()`.
- **Presets:** Saved as `.rrpreset` files. DAW state save/restore via APVTS `getStateInformation()`/`setStateInformation()`.

## Adding a New Parameter (established pattern)

Follow this sequence across files — each step builds on the previous:

1. **ParametersIDs.h** — Add `inline constexpr auto` for the main param + `const juce::String` for rnd neg/pos. Update `totalParameters`.
2. **PluginProcessor.cpp `createParameterLayout()`** — Register with `layout.add(...)`. Use `AudioParameterFloat` with `NormalisableRange` for dB params, display with `juce::String(value, 1) + " dB"`.
3. **RRVoice.h** — Add `float randomizedX = 0.0f;` member, getter, and pointer fields in `RandomizationPointers` struct.
4. **RRVoice.cpp `setRandomizationReferences()`** — Cache pointers via `params->getRawParameterValue(...)`.
5. **RRVoice.cpp `startNote()`** — Call `randEngine->generateRandomValue(base, neg, pos)` and clamp. Rnd sliders are 0–1, scale them to the actual range (e.g., `* 12.0f` for ±12dB). Add defaults in the `else` branch.
6. **PluginProcessor.cpp `processBlock()`** — Read from APVTS, override with voice getter when active.
7. **PluginEditor.h** — Declare slider, attachment, rnd sliders, rnd attachments.
8. **PluginEditor.cpp** — Constructor init list (attachments), `setupKnob`/`setupSlider` loops, LAF + color assignment, destructor cleanup, `paint()` section box + label, `paintOverChildren()` arc overlay, `resized()` positioning, `hitTest()`/`mouseDown()` trios.

Premium-only features use `// COMMENTED FOR LITE — ACTIVE IN PREMIUM` convention.

## Current State

Phase 1 (UI simplification) complete. Phase 2 steps 2.1–2.5 complete. Step 2.6 (verification/testing) is next. EQ, TransientShaper, and Envelope controls are hidden/commented out in the Lite version but code remains for the Pro version. See `RRLite_spec_v2.md` for the full 5-phase roadmap.

## Layout

The right panel has three stacked sections (Amplitude → Tone → Pitch) with 8px gaps. Left panel is a Sample Manager placeholder. The `paint()` and `resized()` layout constants must stay in sync — they are duplicated in both methods.
