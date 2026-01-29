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

### Step 1: Create Parameter ID Constants
- [x] Create ParameterIDs.h file in /Source/Parameters/
- [x] Define namespace for all parameter ID strings
- [x] Create ID constant for Semitone pitch parameter (global control)
- [x] Create ID constant for Fine Tune pitch parameter in cents (global control)
- [x] Create ID constant for Volume parameter
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
- [ ] In PluginProcessor.h, add APVTS member variable
- [ ] Create helper function createParameterLayout() in PluginProcessor.cpp
- [ ] Set up parameter layout object to hold all parameter definitions
- [ ] Initialize APVTS in PluginProcessor constructor with layout
- [ ] Ensure APVTS is properly constructed before other members

**Quick Test After Step 2:**
- [ ] Build project successfully
- [ ] Load plugin in AudioPluginHost
- [ ] Verify plugin still loads without crashes
- [ ] Check no parameters appear yet (they'll be added in next step)

### Step 3: Define All Plugin Parameters with Proper Ranges
- [ ] Add Semitone parameter: Range -12 to +12 semitones, step size 1, default 0
- [ ] Add Fine Tune parameter: Range -100 to +100 cents, step size 1, default 0
- [ ] Add Volume parameter: Range 0.0 to 1.0 (linear) or -60dB to 0dB, default 0.75
- [ ] Add Low EQ Gain: Range -24dB to +24dB, default 0dB, use decibel scaling
- [ ] Add Low EQ Frequency: Range 20Hz to 500Hz, use logarithmic skew factor, default 100Hz
- [ ] Add Mid EQ Gain: Range -24dB to +24dB, default 0dB, use decibel scaling
- [ ] Add Mid EQ Frequency: Range 200Hz to 5000Hz, use logarithmic skew factor, default 1000Hz
- [ ] Add High EQ Gain: Range -24dB to +24dB, default 0dB, use decibel scaling
- [ ] Add High EQ Frequency: Range 2000Hz to 20000Hz, use logarithmic skew factor, default 5000Hz
- [ ] Add Transient Attack: Range -127 to +127, default 0, linear scale
- [ ] Add Transient Decay: Range -127 to +127, default 0, linear scale
- [ ] Set appropriate units and labels for each parameter (dB, Hz, semitones, cents)

**Note:** Semitone and Fine Tune parameters will globally transpose all loaded samples together.

**Quick Test After Step 3:**
- [ ] Build project successfully
- [ ] Load plugin in AudioPluginHost or DAW
- [ ] Open plugin's generic editor (provided by host)
- [ ] Verify all 11 parameters appear in the parameter list
- [ ] Move each parameter slider and verify ranges are correct
- [ ] Check default values are as specified

### Step 4: Implement Parameter Smoothing
- [ ] In PluginProcessor.h, add SmoothedValue object for Volume parameter
- [ ] Add SmoothedValue object for Semitone parameter
- [ ] Add SmoothedValue object for Fine Tune parameter
- [ ] Add SmoothedValue objects for all EQ parameters (6 total)
- [ ] Add SmoothedValue objects for Transient Attack and Decay
- [ ] In prepareToPlay(), initialize all SmoothedValue objects with sample rate
- [ ] Set appropriate ramp time for each parameter (typically 20-50ms)
- [ ] Reset all smoothed values to current parameter values
- [ ] In processBlock(), update target values from APVTS parameters
- [ ] Use smoothed values in audio processing instead of raw parameter values

**Quick Test After Step 4:**
- [ ] Build project successfully
- [ ] Load plugin and play audio through it
- [ ] Rapidly move volume parameter
- [ ] Verify no clicks, pops, or audio glitches
- [ ] Test with other parameters as well

### Step 5: Connect Parameters to Audio Processing
- [ ] In processBlock(), retrieve current Semitone value from APVTS
- [ ] In processBlock(), retrieve current Fine Tune value from APVTS
- [ ] In processBlock(), retrieve current Volume value from APVTS
- [ ] Calculate combined pitch shift from semitone and fine tune parameters
- [ ] Apply global pitch shift to RRVoice's pitchRatio (modify startNote to accept global pitch)
- [ ] Apply volume gain to final output buffer
- [ ] Prepare parameter values for EQ processor (to be connected later)
- [ ] Prepare parameter values for transient processor (to be connected later)
- [ ] Ensure parameter changes are smooth and don't cause clicks or pops

**Implementation Note:** To apply global pitch shifting:
1. Store global pitch offset in AudioProcessor as member variable
2. In startNote(), pass or access this global pitch value
3. Calculate: `pitchRatio = pow(2.0, globalSemitones / 12.0) * pow(2.0, globalCents / 1200.0)`
4. This shifts ALL samples uniformly regardless of which trigger key was pressed

**Quick Test After Step 5:**
- [ ] Build and load plugin with a sample loaded
- [ ] Play C2 or D2 and adjust Volume - verify level changes
- [ ] Adjust Semitone knob - verify pitch changes in semitone steps
- [ ] Adjust Fine Tune - verify subtle pitch changes
- [ ] Test all three parameters together
- [ ] Verify smooth transitions with no audio artifacts

### Step 6: Implement Preset Save and Load Functionality
- [ ] In getStateInformation() method:
  - [ ] Copy current APVTS state to ValueTree object
  - [ ] Create placeholder for custom data (sample file paths)
  - [ ] Convert ValueTree to XML format
  - [ ] Serialize XML to binary memory block
  - [ ] Store in destData parameter
- [ ] In setStateInformation() method:
  - [ ] Parse binary memory block to XML
  - [ ] Convert XML back to ValueTree
  - [ ] Restore APVTS state from ValueTree
  - [ ] Restore custom data (sample file paths - to implement later)
  - [ ] Update any dependent state or UI

**Quick Test After Step 6:**
- [ ] Set parameters to specific values (e.g., Volume 50%, Semitone +5)
- [ ] Save DAW project or create a preset in host
- [ ] Change all parameters to different values
- [ ] Reload the project or preset
- [ ] Verify all parameters return to saved values

### Step 7: Test Parameter Automation in DAW
- [ ] Build plugin with all parameters defined
- [ ] Load plugin in AudioPluginHost or actual DAW
- [ ] Verify all 11 parameters appear in host's parameter list
- [ ] Test moving each parameter and hearing the effect
- [ ] Test parameter automation by recording parameter movements
- [ ] Save a preset with specific parameter values
- [ ] Load the preset and verify all parameters recall correctly
- [ ] Test undo/redo functionality in host (APVTS handles automatically)
- [ ] Verify parameters update smoothly without audio glitches

---

## Phase 4: Sample Management System (Estimated Time: 1 Week)
**Goal:** Implement comprehensive sample loading, storage, and management with proper file handling, error checking, and multiple sample support.

### Step 1: Create Sample Slot Data Structure
- [ ] Create SampleSlot.h file in /Source/Data/
- [ ] Define SampleSlot struct or class with necessary member variables
- [ ] Add audio buffer member to store loaded sample data
- [ ] Add original sample rate member (double)
- [ ] Add boolean flag for loaded state
- [ ] Add source file member (juce::File) to remember file location
- [ ] Add display name member (juce::String) for UI display
- [ ] Implement clear() method to reset slot to empty state
- [ ] Implement loadFromFile() method declaration
- [ ] Create array of 20 SampleSlot objects in PluginProcessor

**Note:** All samples will trigger on C2/D2 at original pitch. No key pair assignments needed.

**Quick Test After Step 1:**
- [ ] Build project successfully
- [ ] In PluginProcessor constructor, create test SampleSlot array
- [ ] Verify project compiles and runs
- [ ] Check that empty slots don't cause crashes

### Step 2: Implement Audio File Loading System
- [ ] Create SampleLoader.h and SampleLoader.cpp in /Source/Data/
- [ ] Set up juce::AudioFormatManager member variable
- [ ] Register basic audio formats (WAV, AIFF, FLAC, OGG)
- [ ] Implement loadSample() function that takes File and SampleSlot reference
- [ ] Create AudioFormatReader from file
- [ ] Validate that file is a supported audio format
- [ ] Read entire audio file into temporary buffer
- [ ] Store original sample rate from file
- [ ] Handle stereo-to-mono conversion (mix both channels)
- [ ] Copy processed audio into SampleSlot buffer
- [ ] Set isLoaded flag to true on success
- [ ] Return success/failure status for error handling

**Quick Test After Step 2:**
- [ ] Build project successfully
- [ ] Manually call loadSample() with a test WAV file in PluginProcessor
- [ ] Add DBG() statements to confirm loading succeeds
- [ ] Verify file loads into SampleSlot correctly
- [ ] Test with both mono and stereo files
- [ ] Test with different sample rates (44.1kHz, 48kHz)

### Step 3: Add Comprehensive Error Handling
- [ ] Add error checking for invalid file formats
- [ ] Add error checking for file not found
- [ ] Add error checking for corrupted audio data
- [ ] Add error checking for unsupported sample rates
- [ ] Add error checking for insufficient memory
- [ ] Create user-friendly error messages for each case
- [ ] Implement fallback behavior when loading fails
- [ ] Add logging for debugging sample loading issues
- [ ] Consider adding file size limits to prevent memory issues

**Quick Test After Step 3:**
- [ ] Try loading a non-audio file (e.g., .txt) - verify error message
- [ ] Try loading a file that doesn't exist - verify error message
- [ ] Try loading a very large file - verify behavior is acceptable
- [ ] Check that failed loads don't crash the plugin
- [ ] Verify error messages are clear and helpful

### Step 4: Implement Sample Rate Conversion (Resampling)
- [ ] Add resampling utility function to SampleLoader
- [ ] Use JUCE's LagrangeInterpolator for high-quality resampling
- [ ] Implement resampleToPluginSampleRate() method
- [ ] Calculate resampling ratio (plugin rate / original rate)
- [ ] Create temporary buffer for resampled audio
- [ ] Process audio through interpolator
- [ ] Replace original buffer with resampled version
- [ ] Call resampling when plugin sample rate changes in prepareToPlay()
- [ ] Ensure resampling maintains audio quality
- [ ] Test with various source sample rates (44.1kHz, 48kHz, 96kHz)

**Quick Test After Step 4:**
- [ ] Load samples recorded at 44.1kHz into plugin running at 48kHz
- [ ] Load samples recorded at 96kHz into plugin running at 48kHz
- [ ] Play samples and listen for pitch accuracy
- [ ] Verify no audio artifacts from resampling
- [ ] Test that samples play at correct pitch regardless of source sample rate

### Step 5: Connect Loaded Samples to Synthesiser
- [ ] Implement updateSynthesiserSounds() method in PluginProcessor
- [ ] Clear all existing sounds from synthesiser before updating
- [ ] Loop through all 20 sample slots
- [ ] For each loaded sample:
  - [ ] Create new RRSound object
  - [ ] Copy audio buffer from slot to sound
  - [ ] Set display name
  - [ ] Add sound to synthesiser (will automatically respond to C2/D2)
- [ ] Call updateSynthesiserSounds() whenever samples change
- [ ] All loaded samples will be available and trigger on C2/D2

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

## Phase 5-11: [Remaining phases unchanged from original spec]

---

## Success Criteria for MVP

### Core Functionality
- [ ] Plugin loads in all tested DAWs without errors
- [ ] All 20 sample slots load and play samples correctly
- [ ] MIDI note mapping works as designed (C2/D2 trigger notes)
- [ ] Monophonic playback functions properly
- [ ] Round-robin cycling through loaded samples works correctly
- [ ] All 11 parameters work and sound good
- [ ] Global pitch controls (Semitone + Fine Tune) transpose all samples uniformly
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

**Good luck with development! Phase 2 is complete and working. Ready for Phase 3: Parameter System!**