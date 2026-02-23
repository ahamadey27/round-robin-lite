# Project Specification: Round Robin Lite

## Tech Stack: JUCE Framework & C++ (Visual Studio 2022)
This specification breaks the project into eight distinct phases, each with actionable steps. The goal is to build a robust Minimum Viable Product (MVP) efficiently, test it thoroughly, and prepare for a successful launch as a commercial audio plugin.

## MIDI Mapping Design (Simplified)
**Trigger Notes:** C2 and D2 (MIDI 36 and 38)
- Both keys trigger all loaded samples at their **original recorded pitch**
- No automatic pitch shifting based on which key is pressed
- The two keys allow for L/R alternation (e.g., left foot, right foot for footsteps)
- **Global pitch shifting** is controlled by plugin parameters (Semitone + Fine Tune in Phase 3)
- Users can transpose all samples together using the plugin interface

**Note:** Depending on your MIDI keyboard manufacturer, C2 (MIDI 36) may be labeled as C1 on your keyboard. This is just a labeling convention difference - the plugin responds to the actual MIDI note numbers (36 and 38).

---

## Phase 1: Foundation & Setup (Estimated Time: 1-2 Days) ✅ COMPLETE
**Goal:** Establish a solid foundation by setting up the development environment, configuring JUCE, and creating the initial project structure.

### Step 1: Development Environment Setup
- [x] Ensure Visual Studio 2022 is installed with C++ Desktop Development workload
- [x] Download and install JUCE Framework (latest stable version from juce.com)
- [x] Install Projucer (comes with JUCE download)
- [x] Set up JUCE global paths in Projucer (Modules path pointing to JUCE/modules)
- [x] Install VST3 SDK (if not included with JUCE)
- [x] Verify AudioPluginHost utility is available in JUCE/extras

### Step 2: Create New JUCE Project in Projucer
- [x] Launch Projucer application
- [x] Create new "Audio Plug-In" project named "round-robin-lite"
- [x] Set Project Name: "Round Robin Lite"
- [x] Set Plugin Manufacturer: "roundrobinlite"
- [x] Set Plugin Code: "rrll" (4-character unique identifier)
- [x] Enable Plugin Format: VST3 (check the box)
- [x] Enable "Standalone" format for testing (optional but helpful)
- [x] Set Plugin Characteristics: Check "Plugin is a Synth"
- [x] Set Plugin Characteristics: Check "Plugin MIDI Input"
- [x] Uncheck "Plugin MIDI Output" (we generate audio, not MIDI)
- [x] Set Visual Studio 2022 as exporter target
- [x] Configure build settings (Debug and Release configurations)

### Step 3: Verify Build System Works
- [x] Save Projucer project and generate Visual Studio solution
- [x] Open generated .sln file in Visual Studio 2022
- [x] Build project in Debug configuration successfully
- [x] Build project in Release configuration successfully
- [x] Locate compiled plugin DLL/VST3 in Builds folder
- [x] Launch JUCE AudioPluginHost application
- [x] Scan for plugins and locate Round Robin Lite
- [x] Load plugin in AudioPluginHost
- [x] Verify default "Hello World" GUI appears
- [x] Test that plugin receives MIDI input

### Step 4: Version Control Setup
- [x] Initialize Git repository in project root directory
- [x] Create .gitignore file (ignore Builds/, JuceLibraryCode/, .vs/, *.user, *.suo files)
- [x] Add all source files to initial commit
- [x] Create README.md with project name and description
- [x] Create initial commit with message "Initial JUCE project setup"
- [x] Create remote repository on GitHub or GitLab
- [x] Push local repository to remote
- [x] Verify repository structure is correct online

### Step 5: Project Organization and Documentation
- [x] Create organized folder structure in Source/ directory:
  - [x] /Source/Audio/ for audio engine classes
  - [x] /Source/DSP/ for signal processing components (EQ, transient shaping)
  - [x] /Source/UI/ for GUI components
  - [x] /Source/Data/ for sample management and data structures
  - [x] /Source/Parameters/ for parameter definitions and management
- [x] Document folder structure and purpose in README.md
- [x] Add plugin feature description to README.md
- [x] Create CHANGELOG.md for tracking version updates
- [x] Set up basic project documentation structure

---

## Phase 2: Core Audio Engine (Estimated Time: 1 Week) ✅ COMPLETE
**Goal:** Implement the fundamental audio playback system with custom synthesiser voice and sound classes for sample playback.

### Step 1: Study JUCE Synthesiser Architecture
- [x] Read JUCE documentation on Synthesiser class
- [x] Study SynthesiserVoice class and its virtual methods
- [x] Study SynthesiserSound class and its purpose
- [x] Review JUCE synthesiser example projects in JUCE/examples
- [x] Understand the renderNextBlock() audio callback workflow
- [x] Learn how JUCE handles MIDI note on/off events
- [x] Understand voice allocation and stealing mechanisms

### Step 2: Create Custom Sound Class (RRSound)
- [x] Create RRSound.h header file in /Source/Audio/
- [x] Create RRSound.cpp implementation file in /Source/Audio/
- [x] Set up class to inherit from juce::SynthesiserSound
- [x] Add member variable for storing audio sample buffer (juce::AudioBuffer<float>)
- [x] Add member variable for storing original sample rate (double)
- [x] Add member variable for sample display name (juce::String)
- [x] Implement appliesToNote() method - responds to C2 and D2 (MIDI 36 and 38)
- [x] Implement appliesToChannel() method (return true for all channels)
- [x] Add method to load audio from file and store in buffer
- [x] Add method to get sample buffer for playback
- [x] Add method to get original sample rate

### Step 3: Create Custom Voice Class (RRVoice)
- [x] Create RRVoice.h header file in /Source/Audio/
- [x] Create RRVoice.cpp implementation file in /Source/Audio/
- [x] Set up class to inherit from juce::SynthesiserVoice
- [x] Add member variable for current sample playback position (double)
- [x] Add member variable for pitch ratio (double) - always 1.0 for original pitch
- [x] Add member variable for ADSR envelope (juce::ADSR)
- [x] Add member variable for voice playing state flag (bool)
- [x] Implement canPlaySound() to check if sound is compatible (RRSound type)
- [x] Implement startNote() method:
  - [x] Set pitch ratio to 1.0 (play at original pitch)
  - [x] Reset playback position to beginning of sample
  - [x] Trigger ADSR envelope attack phase
  - [x] Set voice as currently playing
- [x] Implement stopNote() method:
  - [x] Trigger ADSR envelope release phase
  - [x] Allow voice to fade out naturally
  - [x] Mark voice as finished when envelope completes
- [x] Implement renderNextBlock() method:
  - [x] Get audio data from associated RRSound
  - [x] Use linear interpolation for smooth playback
  - [x] Apply ADSR envelope to each sample
  - [x] Add processed audio to output buffer
  - [x] Handle monophonic voice stealing (stop current note if new note starts)

**Quick Test After Step 3:**
- [x] Add RRVoice files to Projucer project and save
- [x] Regenerate Visual Studio solution
- [x] Build project - verify no compile errors
- [x] Check that RRVoice.h and RRVoice.cpp appear in Solution Explorer under Audio folder

### Step 4: Integrate Synthesiser into AudioProcessor
- [x] In PluginProcessor.h, add juce::Synthesiser member variable
- [x] In PluginProcessor constructor, add one RRVoice to synthesiser (monophonic)
- [x] Initialize synthesiser with no sounds initially (will load later)
- [x] In prepareToPlay() method, set synthesiser sample rate
- [x] In prepareToPlay() method, initialize synthesiser for processing
- [x] In processBlock() method:
  - [x] Clear output buffer before processing
  - [x] Pass MIDI messages to synthesiser
  - [x] Call synthesiser.renderNextBlock() to generate audio
  - [x] Ensure synthesiser output fills the buffer correctly

**Quick Test After Step 4:**
- [x] Build project successfully
- [x] Load plugin in AudioPluginHost
- [x] Connect MIDI input (keyboard or test signal)
- [x] Verify plugin loads without crashes (even though no sound yet - no samples loaded)
- [x] Check Output window in Visual Studio for any debug messages

### Step 5: Design and Implement MIDI Note Mapping System
- [x] Create MidiMapper.h utility file in /Source/Audio/
- [x] Define simplified MIDI mapping:
  - [x] TRIGGER_NOTE_1: C2 (MIDI 36)
  - [x] TRIGGER_NOTE_2: D2 (MIDI 38)
- [x] Create isTriggerNote() function to check if MIDI note is 36 or 38
- [x] Create getTriggerNoteName() function to return "C2/D2" for display
- [x] Create printMappingInfo() function for debug output
- [x] Add documentation explaining that both keys play samples at original pitch
- [x] Note that global pitch controls will be added in Phase 3

**Quick Test After Step 5:**
- [x] Build project successfully
- [x] In PluginProcessor, call MidiMapper::printMappingInfo()
- [x] Verify output shows correct trigger notes (C2/D2, MIDI 36/38)
- [x] Remove test code after verification

### Step 6: Test Basic Audio Playback
- [x] Load a single test WAV file manually in PluginProcessor constructor
- [x] Create RRSound object from loaded audio file
- [x] Add sound to synthesiser (will respond to C2/D2 automatically)
- [x] Build plugin and load in AudioPluginHost
- [x] Connect a MIDI keyboard to plugin
- [x] Play MIDI note 36 (C2/C1 on keyboard) and verify sample plays at original pitch
- [x] Play MIDI note 38 (D2/D1 on keyboard) and verify sample plays at original pitch
- [x] Verify both keys sound identical (same pitch)
- [x] Verify monophonic behavior (only one note plays at a time)
- [x] Test that releasing one note and playing another works smoothly
- [x] Confirm no pitch shifting occurs - samples play at recorded pitch

**COMPLETED:** Phase 2 is complete! All samples now trigger on C2/D2 at their original pitch. Global pitch controls will be added in Phase 3.

---

## Phase 3: Parameter System with APVTS (Estimated Time: 4-5 Days)
**Goal:** Implement all plugin parameters using JUCE's AudioProcessorValueTreeState for proper DAW automation, preset management, and parameter smoothing.

**Total Parameters: 13**
- 2 Global Pitch Controls (Semitone, Fine Tune)
- 1 Volume Control
- 2 Amplitude Envelope (Attack, Decay)
- 6 EQ Controls (3 bands × 2 parameters each)
- 2 Transient Master Controls (Attack, Decay)

### Step 1: Create Parameter ID Constants
- [x] Create ParameterIDs.h file in /Source/Parameters/
- [x] Define namespace for all parameter ID strings
- [x] Create ID constant for Semitone pitch parameter (global control)
- [x] Create ID constant for Fine Tune pitch parameter in cents (global control)
- [x] Create ID constant for Volume parameter
- [x] Create ID constant for Envelope Attack parameter
- [x] Create ID constant for Envelope Decay parameter
- [x] Create ID constant for Low Band EQ Gain parameter
- [x] Create ID constant for Low Band EQ Frequency parameter
- [x] Create ID constant for Mid Band EQ Gain parameter
- [x] Create ID constant for Mid Band EQ Frequency parameter
- [x] Create ID constant for High Band EQ Gain parameter
- [x] Create ID constant for High Band EQ Frequency parameter
- [x] Create ID constant for Transient Attack parameter
- [x] Create ID constant for Transient Decay parameter
- [x] Document each parameter's purpose and range in comments

**Quick Test After Step 1:**
- [x] Build project successfully
- [x] Verify no naming conflicts or compile errors
- [x] Check that ParameterIDs.h is properly included where needed

### Step 2: Set Up AudioProcessorValueTreeState (APVTS)
- [x] In PluginProcessor.h, add APVTS member variable
- [x] Create helper function createParameterLayout() in PluginProcessor.cpp
- [x] Set up parameter layout object to hold all parameter definitions
- [x] Initialize APVTS in PluginProcessor constructor with layout
- [x] Ensure APVTS is properly constructed before other members

**Quick Test After Step 2:**
- [x] Build project successfully
- [x] Load plugin in AudioPluginHost
- [x] Verify plugin still loads without crashes
- [x] Check no parameters appear yet (they'll be added in next step)

### Step 3: Define All Plugin Parameters with Proper Ranges
- [x] Add Semitone parameter: Range -12 to +12 semitones, step size 1, default 0
- [x] Add Fine Tune parameter: Range -100 to +100 cents, step size 1, default 0
- [x] Add Volume parameter: Range 0.0 to 1.0 (linear) or -60dB to 0dB, default 0.75
- [x] Add Envelope Attack: Range 0 to 1000ms, default 0ms (instant attack)
- [x] Add Envelope Decay: Range 0 to 5000ms, default 100ms (short natural release)
- [x] Add Low EQ Gain: Range -24dB to +24dB, default 0dB, use decibel scaling
- [x] Add Low EQ Frequency: Range 20Hz to 500Hz, use logarithmic skew factor, default 100Hz
- [x] Add Mid EQ Gain: Range -24dB to +24dB, default 0dB, use decibel scaling
- [x] Add Mid EQ Frequency: Range 200Hz to 5000Hz, use logarithmic skew factor, default 1000Hz
- [x] Add High EQ Gain: Range -24dB to +24dB, default 0dB, use decibel scaling
- [x] Add High EQ Frequency: Range 2000Hz to 20000Hz, use logarithmic skew factor, default 5000Hz
- [x] Add Transient Attack: Range -127 to +127, default 0, linear scale
- [x] Add Transient Decay: Range -127 to +127, default 0, linear scale
- [x] Set appropriate units and labels for each parameter (dB, Hz, semitones, cents, ms)

**Note:** 
- Envelope Attack/Decay control the ADSR amplitude envelope applied to all samples
- Attack: 0ms = instant (no fade-in), higher values = gradual fade-in
- Decay: Controls the release time after note-off (how quickly sound fades out)
- Semitone and Fine Tune parameters will globally transpose all loaded samples together

**Quick Test After Step 3:**
- [x] Build project successfully
- [x] Load plugin in AudioPluginHost or DAW
- [x] Open plugin's generic editor (provided by host)
- [x] Verify all 13 parameters appear in the parameter list
- [x] Move each parameter slider and verify ranges are correct
- [x] Check default values are as specified

### Step 4: Implement Parameter Smoothing
- [x] In PluginProcessor.h, add SmoothedValue object for Volume parameter
- [x] Add SmoothedValue object for Semitone parameter
- [x] Add SmoothedValue object for Fine Tune parameter
- [x] Add SmoothedValue objects for Envelope Attack and Decay (2 total)
- [x] Add SmoothedValue objects for all EQ parameters (6 total)
- [x] Add SmoothedValue objects for Transient Attack and Decay (2 total)
- [x] In prepareToPlay(), initialize all SmoothedValue objects with sample rate
- [x] Set appropriate ramp time for each parameter (typically 20-50ms)
- [x] Reset all smoothed values to current parameter values
- [x] In processBlock(), update target values from APVTS parameters
- [x] Use smoothed values in audio processing instead of raw parameter values

**Quick Test After Step 4:**
- [x] Build project successfully
- [x] Load plugin and play audio through it
- [x] Rapidly move volume parameter
- [x] Verify no clicks, pops, or audio glitches
- [x] Test with other parameters as well

### Step 4.1: Configure Unpitched Playback Across All Keys
**Goal:** Modify the sampler to play uploaded samples at their original pitch across ALL MIDI keys without pitch shifting. C1 (MIDI note 36) is set as the root reference note.

**Context:** Round Robin Lite will trigger samples without pitch modification, allowing natural footstep sounds to play consistently regardless of which key is pressed. This differs from traditional samplers that pitch-shift samples across the keyboard.

**Implementation Tasks:**
- [x] Update MidiMapper constants to set C1 (MIDI 36) as ROOT_MIDI_NOTE
- [x] Modify RRSound::appliesToNote() to respond to ALL MIDI notes (0-127)
- [x] Remove pitch shifting logic from key pair mapping
- [x] Update RRSound to store samples without pitch offset calculations
- [x] Verify root note reference is C1 for consistency
- [x] Update documentation/comments to reflect unpitched behavior

**Quick Test After Step 4.1:**
- [x] Build project successfully
- [x] Load a test sample into any slot
- [x] Play various MIDI notes (low C1, middle C4, high C7)
- [x] Verify sample plays at identical pitch for all keys
- [x] Confirm monophonic behavior (only one sample plays at a time)
- [x] Test with multiple samples - each should play unpitched

**Files to Modify:**
- MidiMapper.h (update ROOT_MIDI_NOTE constant)
- RRSound.h and RRSound.cpp (modify appliesToNote method)
- PluginProcessor comments (update to reflect unpitched design)

**Expected Behavior:**
After this step, pressing any key on a MIDI controller will trigger the loaded sample at its original, unmodified pitch. The key pair system still groups samples for L/R foot alternation, but no longer applies pitch shifting.

### Step 5: Connect Parameters to Audio Processing
- [x] In processBlock(), retrieve current Semitone value from APVTS
- [x] In processBlock(), retrieve current Fine Tune value from APVTS
- [x] In processBlock(), retrieve current Volume value from APVTS
- [x] In processBlock(), retrieve Envelope Attack and Decay values from APVTS
- [x] Calculate combined pitch shift from semitone and fine tune parameters
- [x] Apply global pitch shift to RRVoice's pitchRatio (modify startNote to accept global pitch)
- [x] Pass envelope Attack/Decay values to RRVoice to update ADSR parameters
- [x] Apply volume gain to final output buffer
- [x] Prepare parameter values for EQ processor (to be connected later)
- [x] Prepare parameter values for transient processor (to be connected later)
- [x] Ensure parameter changes are smooth and don't cause clicks or pops

**Implementation Note:** To apply global pitch shifting:
1. Store global pitch offset in AudioProcessor as member variable
2. In startNote(), pass or access this global pitch value
3. Calculate: `pitchRatio = pow(2.0, globalSemitones / 12.0) * pow(2.0, globalCents / 1200.0)`
4. This shifts ALL samples uniformly regardless of which trigger key was pressed

**Envelope Implementation Note:**
1. Store envelope parameters in AudioProcessor or pass to RRVoice
2. Update RRVoice's ADSR::Parameters in prepareToPlay() or startNote()
3. Set attack time from Envelope Attack parameter (convert ms to seconds)
4. Set decay/release time from Envelope Decay parameter (convert ms to seconds)
5. Keep sustain at 1.0 (full) for consistent playback

**Quick Test After Step 5:**
- [x] Build and load plugin with a sample loaded
- [x] Play C2 or D2 and adjust Volume - verify level changes
- [x] Adjust Semitone knob - verify pitch changes in semitone steps
- [x] Adjust Fine Tune - verify subtle pitch changes
- [x] Test all three parameters together
- [x] Verify smooth transitions with no audio artifacts
- [x] Envelope Test

### Step 6: Implement Preset Save and Load Functionality
- [x] In getStateInformation() method:
  - [x] Copy current APVTS state to ValueTree object
  - [x] Create placeholder for custom data (sample file paths)
  - [x] Convert ValueTree to XML format
  - [x] Serialize XML to binary memory block
  - [x] Store in destData parameter
- [x] In setStateInformation() method:
  - [x] Parse binary memory block to XML
  - [x] Convert XML back to ValueTree
  - [x] Restore APVTS state from ValueTree
  - [x] Restore custom data (sample file paths - to implement later)
  - [x] Update any dependent state or UI

**Quick Test After Step 6:**
- [x] Set parameters to specific values (e.g., Volume 50%, Semitone +5)
- [x] Save DAW project or create a preset in host
- [x] Change all parameters to different values
- [x] Reload the project or preset
- [x] Verify all parameters return to saved values

### Step 7: Test Parameter Automation in DAW
- [x] Build plugin with all parameters defined
- [x] Load plugin in AudioPluginHost or actual DAW
- [x] Verify all 13 parameters appear in host's parameter list
- [x] Test moving each parameter and hearing the effect
- [x] Test Envelope Attack/Decay parameters affect sample fade-in and fade-out
- [x] Test parameter automation by recording parameter movements
- [x] Save a preset with specific parameter values
- [x] Load the preset and verify all parameters recall correctly
- [x] Test undo/redo functionality in host (APVTS handles automatically)
- [x] Verify parameters update smoothly without audio glitches

### Step 7: Implement 3-Band EQ Processor

- [x] **Create EQ Processor Class Structure**
  - [x] Create `ThreeBandEQ.h` in `/Source/DSP/`
  - [x] Create `ThreeBandEQ.cpp` in `/Source/DSP/`
  - [x] Set up class to hold three filter instances (low/mid/high)
  - [x] Use JUCE's `dsp::IIR::Filter` or `dsp::ProcessorDuplicator` for filters

- [x] **Implement EQ Filter Configuration**
  - [x] Create method `prepareToPlay(double sampleRate, int samplesPerBlock)`
  - [x] Create method `updateFilters(float lowGain, float lowFreq, float midGain, float midFreq, float highGain, float highFreq)`
  - [x] Low band: Low-shelf filter (boost/cut below frequency)
  - [x] Mid band: Peaking/bell filter (boost/cut around frequency)
  - [x] High band: High-shelf filter (boost/cut above frequency)
  - [x] Convert dB gain values to linear gain for filters

- [x] **Implement EQ Processing Method**
  - [x] Create method `processBlock(juce::AudioBuffer<float>& buffer)`
  - [x] Apply low-shelf filter to buffer
  - [x] Apply mid peaking filter to buffer
  - [x] Apply high-shelf filter to buffer
  - [x] Process both channels (stereo)

- [x] **Add EQ to PluginProcessor**
  - [x] In `PluginProcessor.h`, add `ThreeBandEQ` member variable
  - [x] In `prepareToPlay()`, initialize EQ with sample rate and buffer size
  - [x] In `processBlock()`, get EQ parameter values from smoothed parameters
  - [x] In `processBlock()`, call `eq.updateFilters()` with current parameter values
  - [x] In `processBlock()`, call `eq.processBlock(buffer)` AFTER synthesiser renders but BEFORE volume

**Quick Test After Step 7:**
- [x] Build project successfully
- [x] Load plugin and play sample
- [x] Adjust Low Gain parameter - should hear bass boost/cut
- [x] Adjust Mid Gain parameter - should hear midrange boost/cut
- [x] Adjust High Gain parameter - should hear treble boost/cut
- [x] Adjust frequency parameters - should change where boost/cut occurs
- [x] Verify EQ responds smoothly without clicks

### Step 8: Implement Transient Master Processor

- [x] **Create Transient Processor Class Structure**
  - [x] Create `TransientShaper.h` in `/Source/DSP/`
  - [x] Create `TransientShaper.cpp` in `/Source/DSP/`
  - [x] Set up envelope follower for detecting transients
  - [x] Use attack/decay parameters to modify transient portions

- [x] **Implement Envelope Follower**
  - [x] Create method `prepareToPlay(double sampleRate, int samplesPerBlock)`
  - [x] Create method `detectEnvelope(const float* input, int numSamples)`
  - [x] Implement attack/release time constants for envelope detection
  - [x] Return envelope curve representing audio dynamics

- [x] **Implement Transient Shaping Algorithm**
  - [x] Create method `processBlock(juce::AudioBuffer<float>& buffer, float attackAmount, float decayAmount)`
  - [x] Split signal into transient (fast) and sustain (slow) components
  - [x] Apply gain to transient component based on attackAmount (-127 to +127)
  - [x] Apply gain to sustain component based on decayAmount (-127 to +127)
  - [x] Recombine components and write to buffer
  - [x] Scale parameters: -127 = 0.5x (softer), 0 = 1.0x (unchanged), +127 = 2.0x (louder)

- [x] **Add Transient Shaper to PluginProcessor**
  - [x] In `PluginProcessor.h`, add `TransientShaper` member variable
  - [x] In `prepareToPlay()`, initialize transient shaper with sample rate
  - [x] In `processBlock()`, get transient parameters from smoothed values
  - [x] In `processBlock()`, call `transientShaper.processBlock(buffer, attackVal, decayVal)` AFTER EQ but BEFORE volume
  - [x] Convert parameter range (-127 to +127) to gain multipliers (0.5x to 2.0x)

**Quick Test After Step 8:**
- [x] Build project successfully
- [x] Load plugin and play sample with strong attack (e.g., drum hit)
- [x] Set Transient Attack to +100 - should emphasize the initial hit
- [x] Set Transient Attack to -100 - should soften the initial hit
- [x] Set Transient Decay to +100 - should emphasize the tail/sustain
- [x] Set Transient Decay to -100 - should reduce the tail/sustain
- [x] Test with Transient Attack at 0 and Decay at 0 - should sound unchanged
- [x] Verify transient shaping responds smoothly without clicks

### Step 9: DSP Chain Integration and Testing

- [ ] **Verify DSP Processing Order**
  - [ ] Confirm order in `processBlock()`: Synthesiser → EQ → Transient Shaper → Volume
  - [ ] Add debug output showing DSP chain execution
  - [ ] Document why this order is optimal (frequency shaping before dynamics, gain last)

- [ ] **Test DSP Chain with Combined Effects**
  - [ ] Load sample and set pitch to +5 semitones
  - [ ] Add low-end boost (+12dB at 100Hz)
  - [ ] Add transient attack enhancement (+50)
  - [ ] Verify all three effects work simultaneously
  - [ ] Test extreme parameter combinations for stability

- [ ] **Performance and CPU Optimization Check**
  - [ ] Profile plugin CPU usage with all DSP active
  - [ ] Check for audio dropouts or glitches
  - [ ] Verify smooth parameter changes don't cause clicks
  - [ ] Ensure plugin runs efficiently in release build

- [ ] **Documentation and Comments**
  - [ ] Add comments explaining DSP chain in `processBlock()`
  - [ ] Document parameter ranges and their effects
  - [ ] Update README.md with DSP features
  - [ ] Note any known limitations or future improvements

**Success Criteria for Step 9:**
- [ ] All parameters (pitch, volume, envelope, EQ, transient) work correctly
- [ ] DSP processors can be used in combination without issues
- [ ] Plugin performs well with minimal CPU usage
- [ ] Code is well-documented and maintainable

---

## Phase 4: Parameter Randomization System (Estimated Time: 1 Week)
**Goal:** Implement per-note parameter randomization with independent positive and negative range controls for all parameters.

### Overview
Each parameter has TWO randomization controls:
- **Negative Range:** How far below current value to randomize
- **Positive Range:** How far above current value to randomize
- **Trigger:** Randomization occurs on each MIDI note-on event
- **Example:** Fine Tune at 0 cents with Neg=-4, Pos=+10 → random value from -4 to +10 cents

**Parameters to Randomize (11 total):**
1. Pitch Semitone
2. Pitch Fine
3. Volume
4. EQ Low Gain
5. EQ Low Frequency
6. EQ Mid Gain
7. EQ Mid Frequency
8. EQ High Gain
9. EQ High Frequency
10. Transient Attack
11. Transient Decay

### Step 1: Add Randomization Parameters (22 new params)
- [x] Create RandomizationParams.h in /Source/Parameters/
- [x] Add 2 parameters per randomizable param (negative + positive range)
- [x] Define parameter IDs with "_RND_NEG" and "_RND_POS" suffixes
- [x] Set all randomization ranges to match their target parameter ranges
- [x] Default all randomization parameters to 0 (no randomization)
- [x] Add all 22 parameters to AudioProcessorValueTreeState
- [x] Update total parameter count in comments (was 13, now 35)

**Parameter Naming Convention:**
```cpp
// Example for Fine Tune:
"pitchFine_rnd_neg"  // Negative randomization range
"pitchFine_rnd_pos"  // Positive randomization range
```

### Step 2: Create RandomizationEngine Class
- [x] Create RandomizationEngine.h/.cpp in /Source/DSP/
- [x] Add Random member variable (juce::Random)
- [x] Seed random generator in constructor
- [x] Implement generateRandomValue() method:
  - [x] Takes: base value, negative range, positive range
  - [x] Returns: randomized value within bounds
  - [x] Handle case where both ranges are 0 (return base value)
- [x] Add method to generate all randomized parameter values at once
- [x] Add method to reset random seed if needed

**Core Algorithm:**
```cpp
float generateRandomValue(float baseValue, float negRange, float posRange)
{
    if (negRange == 0.0f && posRange == 0.0f)
        return baseValue;
    
    float totalRange = negRange + posRange;
    float randomOffset = random.nextFloat() * totalRange - negRange;
    return baseValue + randomOffset;
}
```

### Step 3: Implement Note-Triggered Randomization
- [x] Add RandomizationEngine instance to PluginProcessor
- [x] In RRVoice::startNote(), access randomization parameters
- [x] Generate randomized values for all 11 parameters
- [x] Store randomized values in RRVoice for this note's duration
- [x] Apply randomized values during renderNextBlock()
- [x] Ensure randomization doesn't affect base parameter values (non-destructive)

**Implementation Notes:**
- Randomization is per-note, not global
- Each note gets its own random parameter set
- Base parameter values remain unchanged
- Randomization happens BEFORE any DSP processing

### Step 4: Handle Parameter Value Application
- [x] Create temporary "effective" parameter values in RRVoice
- [x] On note start, calculate: effectiveValue = baseValue + randomOffset
- [x] Clamp effective values to valid parameter ranges
- [x] Use effective values for all DSP processing (pitch, volume, EQ, etc.)
- [x] Add getEffectiveParameterValue() helper method

**Parameter Application Order:**
1. Read base parameter from APVTS
2. Read positive/negative randomization ranges
3. Generate random offset
4. Calculate effective value = base + offset
5. Clamp to valid range
6. Use for DSP processing

### Step 5: Optimize Randomization Performance
- [x] Profile randomization overhead per note
- [x] Pre-calculate random values if needed
- [x] Avoid calling Random::nextFloat() excessively
- [x] Consider caching randomization ranges if they don't change often
- [x] Ensure no audio dropouts during randomization

**Performance Targets:**
- Randomization should add <1% CPU overhead
- No perceivable latency on note trigger
- Smooth operation even with rapid note sequences

### Step 6: Add Randomization Validation & Testing
- [x] Test with all randomization ranges at 0 (should sound identical to base)
- [x] Test with only negative randomization (Fine: -10 to 0)
- [x] Test with only positive randomization (Fine: 0 to +10)
- [x] Test with asymmetric ranges (Fine: -4 to +10)
- [x] Verify each parameter randomizes independently
- [x] Test extreme values don't cause crashes or artifacts
- [x] Verify randomization is truly random (not repeating patterns)

### Step 6.1: Add Pan Parameter and Randomization 
- [x] Add Pan Parameter
- [x] Add pos/neg randomization for pan

**Test Cases:**
```
Volume: Base=0dB, Neg=-6dB, Pos=+3dB → expect -6 to +3dB variation
Semitone: Base=0, Neg=-2, Pos=+5 → expect -2 to +5 semitone variation
EQ Low Gain: Base=0dB, Neg=-12dB, Pos=0dB → expect -12 to 0dB (only cuts)
```

### Step 7: Document Randomization Behavior
- [x] Add comments explaining randomization algorithm
- [x] Document parameter interaction (randomization + base value)
- [x] Note that randomization is per-note, not per-plugin-instance
- [x] Explain clamping behavior for out-of-range values
- [x] Update README.md with randomization feature description

**Success Criteria for Phase 4:**
- [x] All 11 parameters can be randomized independently
- [x] Positive and negative ranges work correctly
- [x] Randomization triggers on every MIDI note
- [x] No audio artifacts or performance issues
- [x] Base parameter values remain unchanged
- [x] Randomization ranges are saved/loaded with presets

---

## Phase 5: Sample Management System (Estimated Time: 1 Week)
**Goal:** Implement comprehensive sample loading, storage, and management with proper file handling, error checking, and multiple sample support.

### Step 1: Create Sample Slot Data Structure
- [x] Create SampleSlot.h file in /Source/Data/
- [x] Define SampleSlot struct or class with necessary member variables
- [x] Add audio buffer member to store loaded sample data
- [x] Add original sample rate member (double)
- [x] Add boolean flag for loaded state
- [x] Add source file member (juce::File) to remember file location
- [x] Add display name member (juce::String) for UI display
- [x] Implement clear() method to reset slot to empty state
- [x] Implement loadFromFile() method declaration
- [x] Create array of 20 SampleSlot objects in PluginProcessorx

**Note:** All samples will trigger on C2/D2 at original pitch. No key pair assignments needed.

**Quick Test After Step 1:**
- [x] Build project successfully
- [x] In PluginProcessor constructor, create test SampleSlot array
- [x] Verify project compiles and runs
- [x] Check that empty slots don't cause crashes

### Step 2: Implement Audio File Loading System
- [x] Create SampleLoader.h and SampleLoader.cpp in /Source/Data/
- [x] Set up juce::AudioFormatManager member variable
- [x] Register basic audio formats (WAV, AIFF, FLAC, OGG)
- [x] Implement loadSample() function that takes File and SampleSlot reference
- [x] Create AudioFormatReader from file
- [x] Validate that file is a supported audio format
- [x] Read entire audio file into temporary buffer
- [x] Store original sample rate from file
- [x] Handle stereo-to-mono conversion (mix both channels)
- [x] Copy processed audio into SampleSlot buffer
- [x] Set isLoaded flag to true on success
- [x] Return success/failure status for error handling

**Quick Test After Step 2:**
- [ ] Build project successfully
- [ ] Manually call loadSample() with a test WAV file in PluginProcessor
- [ ] Add DBG() statements to confirm loading succeeds
- [ ] Verify file loads into SampleSlot correctly
- [ ] Test with both mono and stereo files
- [ ] Test with different sample rates (44.1kHz, 48kHz)

### Step 3: Add Comprehensive Error Handling
- [x] Add error checking for invalid file formats
- [x] Add error checking for file not found
- [x] Add error checking for corrupted audio data
- [x] Add error checking for unsupported sample rates
- [x] Add error checking for insufficient memory
- [x] Create user-friendly error messages for each case
- [x] Implement fallback behavior when loading fails
- [x] Add logging for debugging sample loading issues
- [x] Consider adding file size limits to prevent memory issues

**Quick Test After Step 3:**
- [ ] Try loading a non-audio file (e.g., .txt) - verify error message
- [ ] Try loading a file that doesn't exist - verify error message
- [ ] Try loading a very large file - verify behavior is acceptable
- [ ] Check that failed loads don't crash the plugin
- [ ] Verify error messages are clear and helpful

### Step 4: Implement Sample Rate Conversion (Resampling)
- [x] Add resampling utility function to SampleLoader
- [x] Use JUCE's LagrangeInterpolator for high-quality resampling
- [x] Implement resampleToPluginSampleRate() method
- [x] Calculate resampling ratio (plugin rate / original rate)
- [x] Create temporary buffer for resampled audio
- [x] Process audio through interpolator
- [x] Replace original buffer with resampled version
- [x] Call resampling when plugin sample rate changes in prepareToPlay()
- [x] Ensure resampling maintains audio quality
- [x] Test with various source sample rates (44.1kHz, 48kHz, 96kHz)

**Quick Test After Step 4:**
- [ ] Load samples recorded at 44.1kHz into plugin running at 48kHz
- [ ] Load samples recorded at 96kHz into plugin running at 48kHz
- [ ] Play samples and listen for pitch accuracy
- [ ] Verify no audio artifacts from resampling
- [ ] Test that samples play at correct pitch regardless of source sample rate

### Step 5: Connect Loaded Samples to Synthesiser
- [x] Implement updateSynthesiserSounds() method in PluginProcessor
- [x] Clear all existing sounds from synthesiser before updating
- [x] Loop through all 20 sample slots
- [x] For each loaded sample:
  - [x] Create new RRSound object
  - [x] Copy audio buffer from slot to sound
  - [x] Set display name
  - [x] Add sound to synthesiser (will automatically respond to C2/D2)
- [x] Call updateSynthesiserSounds() whenever samples change
- [x] All loaded samples will be available and trigger on C2/D2

**Note:** With this design, pressing C2 or D2 will cycle through or layer loaded samples. Consider implementing sample selection logic (which sample plays) in Phase 4 Step 6.

**Quick Test After Step 5:**
- [ ] Load 2-3 test samples into different slots
- [ ] Call updateSynthesiserSounds()
- [ ] Play C2/D2 and verify samples trigger
- [ ] Verify only one sample plays at a time (monophonic)

### Step 6: Implement Sample Selection Logic
- [ ] Decide on sample selection method:
  - [ ] Option A: Round-robin (cycle through loaded samples in order)
  - [ ] Option B: User-selected active sample (only one sample plays)
  - [ ] Option C: Random selection from loaded samples
- [ ] For MVP, recommend Option A (true round-robin behavior)
- [ ] Track current sample index in AudioProcessor
- [ ] On each note-on event, advance to next loaded sample
- [ ] Wrap around to first sample after last sample
- [ ] Skip empty slots automatically
- [ ] Add method to reset round-robin position

**Implementation Example for Round-Robin:**
```cpp
int currentSampleIndex = 0;
std::vector<int> loadedSampleIndices; // Indices of non-empty slots

// On note trigger:
if (!loadedSampleIndices.empty())
{
    int slotIndex = loadedSampleIndices[currentSampleIndex];
    // Play sample from slot[slotIndex]
    
    currentSampleIndex = (currentSampleIndex + 1) % loadedSampleIndices.size();
}
```

**Quick Test After Step 6:**
- [ ] Load 3-4 test samples
- [ ] Play C2/D2 repeatedly
- [ ] Verify samples cycle in order (1, 2, 3, 1, 2, 3...)
- [ ] Verify empty slots are skipped
- [ ] Test with only 1 sample loaded

### Step 7: Add Sample Persistence (Save/Load with Project State)
- [ ] In getStateInformation():
  - [ ] Create "SampleData" ValueTree node
  - [ ] For each loaded sample slot, create child node
  - [ ] Store file path (consider relative vs absolute paths)
  - [ ] Store display name
  - [ ] Append SampleData node to main state tree
- [ ] In setStateInformation():
  - [ ] Find and parse "SampleData" node
  - [ ] For each stored sample entry:
    - [ ] Check if file still exists at stored path
    - [ ] Attempt to reload sample from file
    - [ ] Restore display name
    - [ ] Show warning message if file is missing
- [ ] Consider option to embed samples as Base64 in project (increases file size)
- [ ] Implement "Missing Samples" dialog on project load if files not found

**Quick Test After Step 7:**
- [ ] Load samples
- [ ] Save DAW project
- [ ] Close and reopen project
- [ ] Verify all samples reload correctly
- [ ] Move a sample file, reload project - verify missing file warning

### Step 8: Test Complete Sample Management System
- [ ] Prepare 10 diverse test WAV files (different lengths, sample rates)
- [ ] Manually load samples into different slots
- [ ] Verify each sample can be triggered on C2/D2
- [ ] Test round-robin behavior with multiple samples
- [ ] Test with mono and stereo source files
- [ ] Test with 44.1kHz, 48kHz, and 96kHz sample rates
- [ ] Save project with all samples loaded
- [ ] Close and reopen project
- [ ] Verify all samples reload correctly
- [ ] Test behavior when source file is moved/deleted
- [ ] Test memory usage with 20 large samples loaded

---

## Phase 6-12: [Remaining phases unchanged from original spec]

---

## Success Criteria for MVP

### Core Functionality
- [ ] Plugin loads in all tested DAWs without errors
- [ ] All 20 sample slots load and play samples correctly
- [ ] MIDI note mapping works as designed (C2/D2 trigger notes)
- [ ] Monophonic playback functions properly
- [ ] Round-robin cycling through loaded samples works correctly
- [ ] All 13 parameters work and sound good
- [ ] Global pitch controls (Semitone + Fine Tune) transpose all samples uniformly
- [ ] Amplitude envelope (Attack/Decay) shapes sample playback naturally
- [ ] EQ provides clear audible control over frequency bands
- [ ] Transient master shapes attack and decay effectively
- [ ] Randomization feature works within set bounds

### Quality Standards
- [ ] No audio clicks, pops, or glitches under normal use
- [ ] CPU usage is reasonable (<10% on modern processors)
- [ ] UI is clear, intuitive, and responsive
- [ ] Presets save and recall accurately
- [ ] Plugin is stable (no crashes during testing)

### User Experience
- [ ] Users can load samples and start making music within 5 minutes
- [ ] All features are discoverable without extensive documentation
- [ ] Error messages are clear and helpful
- [ ] Plugin feels polished and professional
- [ ] C2/D2 trigger system is intuitive for alternating patterns

---

## Estimated Total Development Time

- Phase 1: Foundation & Setup - 1-2 days ✅
- Phase 2: Core Audio Engine - 1 week ✅
- Phase 3: Parameter System - 4-5 days
- Phase 4: Sample Management - 1 week
- Phase 5: Basic UI - 1-2 weeks
- Phase 6: DSP Implementation - 1 week
- Phase 7: Randomization - 4-5 days
- Phase 8: Advanced Features & Polish - 1 week
- Phase 9: Testing & QA - 1 week
- Phase 10: Release Preparation - 3-4 days
- Phase 11: Post-Launch - Ongoing

**Total Estimated MVP Development Time: 8-10 weeks**

---

## Notes for Future Reference

- Simplified MIDI mapping makes the plugin easier to use and understand
- C2/D2 trigger system is perfect for footstep L/R alternation
- Global pitch controls allow easy transposition of entire sample library
- True round-robin implementation provides natural variation
- Consider adding sample selection UI in Pro version (choose which sample plays)
- Stay focused on core value proposition: easy sample triggering with variation

---

## Phase END: Revisions to Consider
- [ ] Adust the max Amp Decay and Attack parameters based on the longest uploaded sample so the max parameters nearly perfectly fit with the audio 