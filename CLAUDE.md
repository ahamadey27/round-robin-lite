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
- **PluginEditor** (`Source/PluginEditor.h/cpp`) — GUI. Custom Look-and-Feel classes for knobs/sliders. 700×450px canvas.
- **RRVoice** (`Source/Audio/RRvoice.h/cpp`) — `SynthesiserVoice` subclass. Handles sample playback, pitch shifting, ADSR envelope, and per-note randomization.
- **RRSound** (`Source/Audio/RRSound.h/cpp`) — `SynthesiserSound` subclass. Holds one audio buffer per sample.
- **MidiMapper** (`Source/Audio/MidiMapper.h`) — Static utility mapping 10 key pairs to MIDI notes. Root note = C1 (MIDI 36).
- **SampleSlot/SampleLoader** (`Source/Data/`) — Sample loading with stereo-to-mono conversion and resampling.
- **RandomizationEngine** (`Source/DSP/RandomizationEngine.h/cpp`) — Asymmetric per-note randomization (separate negative/positive ranges).
- **ParametersIDs** (`Source/Parameters/ParametersIDs.h`) — All 35+ parameter ID string constants.

### DSP Chain (`Source/DSP/`)

- **ToneControl** — Simple 2-knob low/high shelf EQ (250Hz/4kHz, ±12dB). Phase 2 addition, partially integrated.
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

## Current State

Phase 1 (UI simplification) complete. Phase 2 (ToneControl integration) in progress. EQ, TransientShaper, and Envelope controls are hidden/commented out in the Lite version but code remains for the Pro version. See `RRLite_spec_v2.md` for the full 5-phase roadmap.
