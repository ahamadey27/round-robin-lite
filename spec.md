# Project Specification: Round Robin Lite

## Tech Stack: JUCE Framework & C++ (Visual Studio 2022)
This specification breaks the project into eight distinct phases, each with actionable steps. The goal is to build a robust Minimum Viable Product (MVP) efficiently, test it thoroughly, and prepare for a successful launch as a commercial audio plugin.

---

## Phase 1: Foundation & Setup (Estimated Time: 1-2 Days)
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
- [ ] Create organized folder structure in Source/ directory:
  - [ ] /Source/Audio/ for audio engine classes
  - [ ] /Source/DSP/ for signal processing components (EQ, transient shaping)
  - [ ] /Source/UI/ for GUI components
  - [ ] /Source/Data/ for sample management and data structures
  - [ ] /Source/Parameters/ for parameter definitions and management
- [ ] Document folder structure and purpose in README.md
- [ ] Add plugin feature description to README.md
- [ ] Create CHANGELOG.md for tracking version updates
- [ ] Set up basic project documentation structure

---

## Phase 2: Core Audio Engine (Estimated Time: 1 Week)
**Goal:** Implement the fundamental audio playback system with custom synthesiser voice and sound classes for sample playback.

### Step 1: Study JUCE Synthesiser Architecture
- [ ] Read JUCE documentation on Synthesiser class
- [ ] Study SynthesiserVoice class and its virtual methods
- [ ] Study SynthesiserSound class and its purpose
- [ ] Review JUCE synthesiser example projects in JUCE/examples
- [ ] Understand the renderNextBlock() audio callback workflow
- [ ] Learn how JUCE handles MIDI note on/off events
- [ ] Understand voice allocation and stealing mechanisms

### Step 2: Create Custom Sound Class (RRSound)
- [ ] Create RRSound.h header file in /Source/Audio/
- [ ] Create RRSound.cpp implementation file in /Source/Audio/
- [ ] Set up class to inherit from juce::SynthesiserSound
- [ ] Add member variable for storing audio sample buffer (juce::AudioBuffer<float>)
- [ ] Add member variable for storing original sample rate (double)
- [ ] Add member variable for root MIDI note number (int, default 48 for C2)
- [ ] Add member variable for sample display name (juce::String)
- [ ] Add member variable for key pair index (int, 0-9 representing which pair)
- [ ] Implement appliesToNote() method to define which MIDI notes trigger this sound
- [ ] Implement appliesToChannel() method (return true for all channels)
- [ ] Add method to load audio from file and store in buffer
- [ ] Add method to get sample buffer for playback
- [ ] Add method to get root note for pitch calculation

### Step 3: Create Custom Voice Class (RRVoice)
- [ ] Create RRVoice.h header file in /Source/Audio/
- [ ] Create RRVoice.cpp implementation file in /Source/Audio/
- [ ] Set up class to inherit from juce::SynthesiserVoice
- [ ] Add member variable for current sample playback position (double)
- [ ] Add member variable for calculated pitch ratio (double)
- [ ] Add member variable for ADSR envelope (juce::ADSR)
- [ ] Add member variable for voice playing state flag (bool)
- [ ] Implement canPlaySound() to check if sound is compatible (RRSound type)
- [ ] Implement startNote() method:
  - [ ] Calculate pitch shift ratio based on MIDI note vs root note
  - [ ] Reset playback position to beginning of sample
  - [ ] Trigger ADSR envelope attack phase
  - [ ] Set voice as currently playing
- [ ] Implement stopNote() method:
  - [ ] Trigger ADSR envelope release phase
  - [ ] Allow voice to fade out naturally
  - [ ] Mark voice as finished when envelope completes
- [ ] Implement renderNextBlock() method:
  - [ ] Get audio data from associated RRSound
  - [ ] Apply pitch shifting using sample rate conversion
  - [ ] Apply ADSR envelope to each sample
  - [ ] Add processed audio to output buffer
  - [ ] Handle monophonic voice stealing (stop current note if new note starts)

### Step 4: Integrate Synthesiser into AudioProcessor
- [ ] In PluginProcessor.h, add juce::Synthesiser member variable
- [ ] In PluginProcessor constructor, add one RRVoice to synthesiser (monophonic)
- [ ] Initialize synthesiser with no sounds initially (will load later)
- [ ] In prepareToPlay() method, set synthesiser sample rate
- [ ] In prepareToPlay() method, initialize synthesiser for processing
- [ ] In processBlock() method:
  - [ ] Clear output buffer before processing
  - [ ] Pass MIDI messages to synthesiser
  - [ ] Call synthesiser.renderNextBlock() to generate audio
  - [ ] Ensure synthesiser output fills the buffer correctly

### Step 5: Design and Implement MIDI Note Mapping System
- [ ] Create MidiMapper.h utility file in /Source/Audio/
- [ ] Define the complete key pair mapping structure:
  - [ ] Pair 0: C0/D0 (MIDI 36/38) maps to -7 semitones from root
  - [ ] Pair 1: E0/F0 (MIDI 40/41) maps to -6 semitones from root
  - [ ] Pair 2: G0/A0 (MIDI 43/45) maps to -5 semitones from root
  - [ ] Pair 3: B0/C1 (MIDI 47/48) maps to -4 semitones from root
  - [ ] Pair 4: E1/F1 (MIDI 52/53) maps to -3 semitones from root
  - [ ] Pair 5: G1/A1 (MIDI 55/57) maps to -2 semitones from root
  - [ ] Pair 6: B1/C2 (MIDI 59/60) maps to -1 semitone from root
  - [ ] Pair 7: C2/D2 (MIDI 48/50) maps to 0 semitones (ROOT pitch)
  - [ ] Pair 8: E2/F2 (MIDI 64/65) maps to +1 semitone from root
  - [ ] Pair 9: G2/A2 (MIDI 67/69) maps to +2 semitones from root
- [ ] Create function to get the two MIDI note numbers for any given pair index
- [ ] Create function to get semitone offset for any given pair index
- [ ] Create function to validate pair assignments (only 10 pairs maximum)
- [ ] Add documentation explaining the pairing system for footstep sounds

### Step 6: Test Basic Audio Playback
- [ ] Load a single test WAV file manually in PluginProcessor constructor
- [ ] Create RRSound object from loaded audio file
- [ ] Assign sound to root key pair (C2/D2, MIDI notes 48 and 50)
- [ ] Add sound to synthesiser
- [ ] Build plugin and load in AudioPluginHost
- [ ] Connect a virtual MIDI keyboard to plugin
- [ ] Play MIDI note 48 (C2) and verify sample plays at correct pitch
- [ ] Play MIDI note 50 (D2) and verify sample plays at correct pitch
- [ ] Test pitch shifting by playing other MIDI notes
- [ ] Verify monophonic behavior (only one note plays at a time)
- [ ] Test that releasing one note and playing another works smoothly

---

## Phase 3: Parameter System with APVTS (Estimated Time: 4-5 Days)
**Goal:** Implement all plugin parameters using JUCE's AudioProcessorValueTreeState for proper DAW automation, preset management, and parameter smoothing.

### Step 1: Create Parameter ID Constants
- [ ] Create ParameterIDs.h file in /Source/Parameters/
- [ ] Define namespace for all parameter ID strings
- [ ] Create ID constant for Semitone pitch parameter
- [ ] Create ID constant for Fine Tune pitch parameter (cents)
- [ ] Create ID constant for Volume parameter
- [ ] Create ID constant for Low Band EQ Gain parameter
- [ ] Create ID constant for Low Band EQ Frequency parameter
- [ ] Create ID constant for Mid Band EQ Gain parameter
- [ ] Create ID constant for Mid Band EQ Frequency parameter
- [ ] Create ID constant for High Band EQ Gain parameter
- [ ] Create ID constant for High Band EQ Frequency parameter
- [ ] Create ID constant for Transient Attack parameter
- [ ] Create ID constant for Transient Decay parameter
- [ ] Document each parameter's purpose and range in comments

### Step 2: Set Up AudioProcessorValueTreeState (APVTS)
- [ ] In PluginProcessor.h, add APVTS member variable
- [ ] Create helper function createParameterLayout() in PluginProcessor.cpp
- [ ] Set up parameter layout object to hold all parameter definitions
- [ ] Initialize APVTS in PluginProcessor constructor with layout
- [ ] Ensure APVTS is properly constructed before other members

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

### Step 5: Connect Parameters to Audio Processing
- [ ] In processBlock(), retrieve current Semitone value from APVTS
- [ ] In processBlock(), retrieve current Fine Tune value from APVTS
- [ ] In processBlock(), retrieve current Volume value from APVTS
- [ ] Calculate combined pitch shift from semitone and fine tune parameters
- [ ] Apply global pitch shift to all active voices
- [ ] Apply volume gain to final output buffer
- [ ] Prepare parameter values for EQ processor (to be connected later)
- [ ] Prepare parameter values for transient processor (to be connected later)
- [ ] Ensure parameter changes are smooth and don't cause clicks or pops

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
**Goal:** Implement comprehensive sample loading, storage, and management with proper file handling, error checking, and key pair assignment.

### Step 1: Create Sample Slot Data Structure
- [ ] Create SampleSlot.h file in /Source/Data/
- [ ] Define SampleSlot struct or class with necessary member variables
- [ ] Add audio buffer member to store loaded sample data
- [ ] Add original sample rate member (double)
- [ ] Add boolean flag for loaded state
- [ ] Add source file member (juce::File) to remember file location
- [ ] Add display name member (juce::String) for UI display
- [ ] Add key pair index member (int, 0-9) for MIDI note assignment
- [ ] Implement clear() method to reset slot to empty state
- [ ] Implement loadFromFile() method declaration
- [ ] Create array of 20 SampleSlot objects in PluginProcessor

### Step 2: Implement Audio File Loading System
- [ ] Create SampleLoader.h and SampleLoader.cpp in /Source/Data/
- [ ] Set up juce::AudioFormatManager member variable
- [ ] Register basic audio formats (WAV, AIFF, FLAC, OGG)
- [ ] Implement loadSample() function that takes File and SampleSlot reference
- [ ] Create AudioFormatReader from file
- [ ] Validate that file is a supported audio format
- [ ] Read entire audio file into temporary buffer
- [ ] Store original sample rate from file
- [ ] Handle stereo-to-mono conversion (mix both channels or take left only)
- [ ] Copy processed audio into SampleSlot buffer
- [ ] Set isLoaded flag to true on success
- [ ] Return success/failure status for error handling

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

### Step 5: Connect Loaded Samples to Synthesiser
- [ ] Implement updateSynthesiserSounds() method in PluginProcessor
- [ ] Clear all existing sounds from synthesiser before updating
- [ ] Loop through all 20 sample slots
- [ ] For each loaded sample:
  - [ ] Create new RRSound object
  - [ ] Copy audio buffer from slot to sound
  - [ ] Set MIDI note mapping based on keyPairIndex
  - [ ] Calculate root MIDI note (C2 = 48, adjusted by pair offset)
  - [ ] Set sound to respond to correct MIDI note pair
  - [ ] Add sound to synthesiser
- [ ] Call updateSynthesiserSounds() whenever samples change
- [ ] Call updateSynthesiserSounds() when key pair assignments change

### Step 6: Implement Key Pair Assignment Logic
- [ ] Create assignSampleToKeyPair() method in PluginProcessor
- [ ] Validate that pair index is between 0-9
- [ ] Check if pair is already assigned to another slot
- [ ] Handle conflicts (ask user or auto-unassign previous slot)
- [ ] Update slot's keyPairIndex member
- [ ] Recalculate MIDI note mapping for the slot
- [ ] Update synthesiser with new sound assignments
- [ ] Provide feedback about successful/failed assignment
- [ ] Implement unassignSample() method to remove from pair
- [ ] Allow user to see which pairs are currently assigned

### Step 7: Add Sample Persistence (Save/Load with Project State)
- [ ] In getStateInformation():
  - [ ] Create "SampleData" ValueTree node
  - [ ] For each loaded sample slot, create child node
  - [ ] Store file path (consider relative vs absolute paths)
  - [ ] Store key pair assignment index
  - [ ] Store display name
  - [ ] Append SampleData node to main state tree
- [ ] In setStateInformation():
  - [ ] Find and parse "SampleData" node
  - [ ] For each stored sample entry:
    - [ ] Check if file still exists at stored path
    - [ ] Attempt to reload sample from file
    - [ ] Restore key pair assignment
    - [ ] Restore display name
    - [ ] Show warning message if file is missing
- [ ] Consider option to embed samples as Base64 in project (increases file size)
- [ ] Implement "Missing Samples" dialog on project load if files not found

### Step 8: Test Complete Sample Management System
- [ ] Prepare 10 diverse test WAV files (different lengths, sample rates)
- [ ] Manually load samples into different slots
- [ ] Assign samples to various key pairs (0-9)
- [ ] Verify each sample triggers on correct MIDI notes
- [ ] Test pitch shifting across different semitone offsets
- [ ] Test with mono and stereo source files
- [ ] Test with 44.1kHz, 48kHz, and 96kHz sample rates
- [ ] Save project with all samples loaded
- [ ] Close and reopen project
- [ ] Verify all samples reload correctly
- [ ] Test behavior when source file is moved/deleted
- [ ] Test memory usage with 20 large samples loaded

---

## Phase 5: Basic User Interface (Estimated Time: 1-2 Weeks)
**Goal:** Create a functional, intuitive UI with all essential controls needed for the MVP, focusing on usability over visual polish.

### Step 1: Plan UI Layout and Design
- [ ] Sketch UI layout on paper or in design tool (Figma, Adobe XD, Sketch)
- [ ] Define main sections:
  - [ ] Top Section: Sample Slots (20 slots, organized in 2 rows of 10)
  - [ ] Middle-Left: Global Pitch Controls (Semitone, Fine Tune)
  - [ ] Middle-Center: Volume Control
  - [ ] Middle-Right: 3-Band EQ Controls (Low, Mid, High)
  - [ ] Bottom Section: Transient Master (Attack, Decay)
- [ ] Decide on basic color scheme (dark/light theme)
- [ ] Plan minimum window size (suggest 800x600 pixels)
- [ ] Consider resizable window for future enhancement
- [ ] Keep layout simple and uncluttered for MVP

### Step 2: Create Custom Rotary Knob Component
- [ ] Create RRKnob.h and RRKnob.cpp files in /Source/UI/
- [ ] Inherit from juce::Slider or create fully custom Component
- [ ] Configure as rotary knob style (not linear slider)
- [ ] Implement custom paint() method:
  - [ ] Draw circular knob background
  - [ ] Draw value indicator (line from center or filled arc)
  - [ ] Draw current value as text overlay
- [ ] Add juce::Label underneath knob for parameter name
- [ ] Make easily attachable to APVTS via SliderAttachment
- [ ] Set appropriate mouse drag sensitivity
- [ ] Add double-click to reset to default value
- [ ] Test with different parameter ranges and units
- [ ] Ensure knob is visually clear and easy to use

### Step 3: Create Sample Slot UI Component
- [ ] Create SampleSlotComponent.h and SampleSlotComponent.cpp in /Source/UI/
- [ ] Design two visual states:
  - [ ] Empty state: Dashed border, "+" icon, "Load Sample" text
  - [ ] Loaded state: Solid border, sample filename, optional waveform preview
- [ ] Add hover effect (highlight on mouse over)
- [ ] Implement drag-and-drop support:
  - [ ] Inherit from FileDragAndDropTarget interface
  - [ ] Implement isInterestedInFileDrag() to check for audio files
  - [ ] Implement filesDropped() to trigger sample loading
  - [ ] Provide visual feedback during drag operation
- [ ] Add "Load" button as alternative to drag-and-drop
- [ ] Add "Clear" button (small X icon) to remove loaded sample
- [ ] Display assigned key pair (e.g., "C0/D0") when sample is loaded
- [ ] Add callback to notify parent editor when sample changes
- [ ] Show loading indicator during sample load operation

### Step 4: Build Main Plugin Editor Layout
- [ ] In PluginEditor.h, add member variables for all UI components:
  - [ ] Array of 20 SampleSlotComponent objects
  - [ ] Semitone knob (RRKnob)
  - [ ] Fine tune knob (RRKnob)
  - [ ] Volume knob (RRKnob)
  - [ ] 6 EQ knobs (Low Gain/Freq, Mid Gain/Freq, High Gain/Freq)
  - [ ] 2 Transient knobs (Attack, Decay)
  - [ ] Section labels for organization
- [ ] In PluginEditor constructor:
  - [ ] Set editor size (800x600 or as designed)
  - [ ] Initialize all component objects
  - [ ] Set ranges and styles for all knobs
  - [ ] Add all components to editor with addAndMakeVisible()
  - [ ] Set up callbacks for sample slot changes
- [ ] In resized() method:
  - [ ] Layout sample slots in 2 rows x 10 columns grid
  - [ ] Position pitch controls in left section
  - [ ] Position volume control in center
  - [ ] Position EQ knobs in right section (3 columns)
  - [ ] Position transient controls at bottom
  - [ ] Ensure proper spacing and alignment
  - [ ] Consider using FlexBox for responsive layout

### Step 5: Connect UI Controls to Parameters (APVTS Attachments)
- [ ] In PluginEditor.h, add SliderAttachment unique_ptr for each knob
- [ ] Create attachment for Semitone knob to semitone parameter
- [ ] Create attachment for Fine Tune knob to fineTune parameter
- [ ] Create attachment for Volume knob to volume parameter
- [ ] Create attachment for Low Gain knob to lowGain parameter
- [ ] Create attachment for Low Freq knob to lowFreq parameter
- [ ] Create attachment for Mid Gain knob to midGain parameter
- [ ] Create attachment for Mid Freq knob to midFreq parameter
- [ ] Create attachment for High Gain knob to highGain parameter
- [ ] Create attachment for High Freq knob to highFreq parameter
- [ ] Create attachment for Attack knob to transientAttack parameter
- [ ] Create attachment for Decay knob to transientDecay parameter
- [ ] Verify all attachments are created in editor constructor
- [ ] Test that moving knobs updates parameters in real-time

### Step 6: Implement Sample Loading UI Integration
- [ ] For each SampleSlotComponent, set up onSampleLoaded callback
- [ ] In callback, call PluginProcessor's loadSampleIntoSlot() method
- [ ] Pass slot index and selected file to processor
- [ ] Show loading indicator while file is being processed
- [ ] Update slot UI to reflect loaded state when complete
- [ ] Display sample filename or abbreviated name
- [ ] Auto-assign loaded sample to next available key pair
- [ ] Show error message dialog if loading fails
- [ ] Update slot to show assigned MIDI notes (e.g., "C0/D0")
- [ ] Implement file browser dialog as fallback to drag-and-drop
- [ ] Consider adding recent files menu for quick reloading

### Step 7: Add Visual Feedback and Polish
- [ ] Add tooltips to all knobs showing current value
- [ ] Add tooltips to sample slots explaining drag-and-drop
- [ ] Show key pair assignment clearly on each loaded slot
- [ ] Consider adding mini waveform preview in sample slots (optional)
- [ ] Color-code sample slots by key pair for visual organization
- [ ] Add section headers/labels (Pitch, Volume, EQ, Transient)
- [ ] Ensure all text is readable at standard screen resolutions
- [ ] Test UI with different OS themes (dark/light mode)
- [ ] Verify UI doesn't freeze during sample loading
- [ ] Add keyboard shortcuts for common actions (optional)

### Step 8: Test Complete UI Workflow
- [ ] Build plugin and load in AudioPluginHost or DAW
- [ ] Test drag-and-drop for loading samples
- [ ] Test "Load" button file browser
- [ ] Test clearing samples with "Clear" button
- [ ] Test all 11 parameter knobs respond correctly
- [ ] Verify parameter automation from DAW updates UI knobs
- [ ] Test sample loading with various file types
- [ ] Verify UI provides clear feedback for all actions
- [ ] Test with different screen sizes and DPI settings
- [ ] Ensure UI is responsive and doesn't lag
- [ ] Check that all components are properly aligned
- [ ] Verify no visual glitches or rendering issues

---

## Phase 6: DSP Implementation (EQ & Transient Master) (Estimated Time: 1 Week)
**Goal:** Implement the 3-band EQ and transient shaping processors to complete the audio processing chain.

### Step 1: Understand JUCE DSP Module
- [ ] Study JUCE dsp module documentation
- [ ] Learn about dsp::ProcessorChain for serial processing
- [ ] Study dsp::IIR::Filter for implementing EQ bands
- [ ] Review filter types (Peak, LowShelf, HighShelf)
- [ ] Understand filter coefficient calculation
- [ ] Learn about ProcessSpec for configuring DSP processors
- [ ] Study examples of multi-band EQ implementation

### Step 2: Implement 3-Band EQ Processor
- [ ] Create EQProcessor.h and EQProcessor.cpp in /Source/DSP/
- [ ] Set up three separate IIR filters (Low, Mid, High)
- [ ] Configure Low band as Low Shelf filter
- [ ] Configure Mid band as Peak/Bell filter
- [ ] Configure High band as High Shelf filter
- [ ] Add prepare() method to initialize filters with sample rate
- [ ] Add process() method to apply all three bands sequentially
- [ ] Implement parameter update methods for gains and frequencies
- [ ] Add reset() method to clear filter states
- [ ] Consider using ProcessorChain to organize filters
- [ ] Ensure filters are properly initialized before use

### Step 3: Connect EQ to Parameter System
- [ ] In PluginProcessor, add EQProcessor member variable
- [ ] Initialize EQ processor in prepareToPlay()
- [ ] In processBlock(), retrieve EQ parameter values from APVTS
- [ ] Update Low band gain and frequency from parameters
- [ ] Update Mid band gain and frequency from parameters
- [ ] Update High band frequency and frequency from parameters
- [ ] Recalculate filter coefficients when parameters change
- [ ] Apply EQ processing to audio buffer after synthesiser
- [ ] Test that EQ changes are audible and smooth
- [ ] Verify no audio glitches when changing EQ parameters

### Step 4: Design Transient Shaping System
- [ ] Research transient shaping techniques (envelope following, gain adjustment)
- [ ] Decide on implementation approach:
  - [ ] Option A: Envelope follower with attack/decay adjustment
  - [ ] Option B: ADSR envelope modification in voice
  - [ ] Option C: Dynamic gain adjustment based on signal transients
- [ ] Create TransientProcessor.h and TransientProcessor.cpp in /Source/DSP/
- [ ] Implement envelope detection for finding transient peaks
- [ ] Add attack parameter to control transient enhancement/reduction
- [ ] Add decay parameter to control sustain and release shaping
- [ ] Ensure processing is efficient enough for real-time use

### Step 5: Implement Transient Master Processor
- [ ] Set up envelope follower to detect signal level changes
- [ ] Implement attack enhancement:
  - [ ] Positive values: Emphasize transients (faster attack)
  - [ ] Negative values: Soften transients (slower attack)
- [ ] Implement decay adjustment:
  - [ ] Positive values: Extend sustain/decay
  - [ ] Negative values: Shorten sustain/decay
- [ ] Add prepare() method to initialize with sample rate
- [ ] Add process() method to apply transient shaping
- [ ] Add parameter update methods for attack and decay values
- [ ] Implement smoothing to prevent clicks when parameters change
- [ ] Test with percussive samples (drums, percussion)

### Step 6: Integrate Transient Processor into Signal Chain
- [ ] In PluginProcessor, add TransientProcessor member variable
- [ ] Initialize transient processor in prepareToPlay()
- [ ] In processBlock(), retrieve transient parameters from APVTS
- [ ] Update transient attack and decay from parameters
- [ ] Apply transient processing after EQ in signal chain
- [ ] Ensure processing order: Synthesiser → Volume → EQ → Transient → Output
- [ ] Test that transient changes are audible
- [ ] Verify no phase issues or artifacts
- [ ] Check CPU usage is reasonable

### Step 7: Test Complete DSP Chain
- [ ] Load various sample types (drums, vocals, sustained tones)
- [ ] Test each EQ band individually (solo low, mid, high)
- [ ] Sweep EQ frequency knobs and listen for smooth transitions
- [ ] Test extreme EQ gain values (+24dB, -24dB)
- [ ] Test transient attack enhancement on percussive samples
- [ ] Test transient decay adjustment on sustained samples
- [ ] Test all DSP effects together with various settings
- [ ] Verify no audio glitches, clicks, or pops
- [ ] Check CPU usage and optimize if necessary
- [ ] Compare output quality with professional plugins

---

## Phase 7: Randomization System (Estimated Time: 4-5 Days)
**Goal:** Implement the parameter randomization feature to allow users to add variation to their sounds easily.

### Step 1: Design Randomization Architecture
- [ ] Decide on randomization scope:
  - [ ] Option A: Global randomization (affects all samples)
  - [ ] Option B: Per-sample randomization (each sample gets different values)
- [ ] For MVP, choose Option A (simpler implementation)
- [ ] Plan UI for setting randomization bounds (min/max sliders)
- [ ] Plan randomization trigger (button for each parameter group)
- [ ] Decide which parameters can be randomized:
  - [ ] Pitch (Semitone, Fine Tune)
  - [ ] Volume
  - [ ] EQ (all 6 parameters)
  - [ ] Transient (Attack, Decay)

### Step 2: Create Randomization UI Components
- [ ] Create RandomizableControl.h and RandomizableControl.cpp in /Source/UI/
- [ ] Design component to wrap existing knobs
- [ ] Add min bound slider (small slider above or beside main knob)
- [ ] Add max bound slider (small slider above or beside main knob)
- [ ] Add "Randomize" button with dice icon
- [ ] Add visual indicator showing current randomization range
- [ ] Make component reusable for all randomizable parameters
- [ ] Ensure component fits well in existing layout
- [ ] Add tooltips explaining randomization feature

### Step 3: Implement Randomization Logic
- [ ] Create Randomizer.h utility class in /Source/Data/
- [ ] Implement getRandomValue() method that takes min and max bounds
- [ ] Use juce::Random class for random number generation
- [ ] Ensure random values respect parameter ranges and step sizes
- [ ] Add option for different random distributions (uniform, gaussian)
- [ ] For MVP, use simple uniform distribution
- [ ] Add seed option for reproducible randomization (optional)
- [ ] Test that random values are evenly distributed

### Step 4: Connect Randomization to Parameters
- [ ] For each randomizable parameter, add min/max bound storage
- [ ] Store bounds as additional parameters in APVTS (optional) or as member variables
- [ ] When "Randomize" button clicked:
  - [ ] Get current min and max bounds for parameter
  - [ ] Generate random value within bounds
  - [ ] Set parameter value to random value
  - [ ] Update UI to reflect new value
- [ ] Implement randomization for Pitch section (Semitone + Fine)
- [ ] Implement randomization for Volume
- [ ] Implement randomization for all 6 EQ parameters
- [ ] Implement randomization for Transient section (Attack + Decay)

### Step 5: Add Group Randomization
- [ ] Add "Randomize All Pitch" button that randomizes both semitone and fine tune
- [ ] Add "Randomize All EQ" button that randomizes all 6 EQ parameters at once
- [ ] Add "Randomize All Transient" button for attack and decay together
- [ ] Add "Randomize All" button to randomize everything at once
- [ ] Ensure grouped randomization respects individual parameter bounds
- [ ] Provide visual feedback during randomization
- [ ] Add undo support for randomization (APVTS handles this automatically)

### Step 6: Test Randomization System
- [ ] Set specific min/max bounds for each parameter
- [ ] Click randomize buttons multiple times
- [ ] Verify values stay within set bounds
- [ ] Test extreme bounds (-127 to +127 for transient, for example)
- [ ] Test that randomization works with parameter automation
- [ ] Verify randomization doesn't cause audio glitches
- [ ] Test undo/redo of randomization
- [ ] Save and load preset with randomization bounds
- [ ] Verify bounds are recalled correctly
- [ ] Test randomization with various sample types

---

## Phase 8: Advanced Features & Polish (Estimated Time: 1 Week)
**Goal:** Add final features, optimize performance, and polish the plugin for release.

### Step 1: Implement Undo/Redo System (If not using APVTS)
- [ ] Verify APVTS provides undo/redo automatically in host
- [ ] Test undo/redo for parameter changes
- [ ] Test undo/redo for randomization
- [ ] If needed, implement custom undo manager for sample operations
- [ ] Test undo/redo of sample loading/clearing
- [ ] Ensure undo/redo works correctly across sessions

### Step 2: Add Preset Management
- [ ] Implement factory presets with useful starting points
- [ ] Create preset: "Natural" (minimal processing)
- [ ] Create preset: "Punchy" (transient enhancement, tight EQ)
- [ ] Create preset: "Warm" (low-end boost, soft attack)
- [ ] Create preset: "Bright" (high-end boost, fast attack)
- [ ] Create preset: "Randomized" (varied settings for experimentation)
- [ ] Implement preset browser if not provided by host
- [ ] Test preset recall accuracy
- [ ] Ensure presets save/load sample assignments

### Step 3: Performance Optimization
- [ ] Profile CPU usage with all features active
- [ ] Optimize sample loading to use background threads
- [ ] Optimize processBlock() for efficiency:
  - [ ] Minimize memory allocations
  - [ ] Use SIMD where applicable
  - [ ] Avoid unnecessary copies
- [ ] Test with maximum polyphony (20 samples loaded)
- [ ] Test with various buffer sizes (64, 128, 256, 512 samples)
- [ ] Ensure plugin doesn't drop audio or cause glitches
- [ ] Optimize UI repainting to avoid visual lag
- [ ] Test memory usage and check for leaks

### Step 4: Add Visualization Features (Optional for MVP)
- [ ] Add waveform display in sample slots
- [ ] Add level meter for output monitoring
- [ ] Add visual feedback for active MIDI notes
- [ ] Add EQ frequency response graph (optional)
- [ ] Keep visualizations lightweight to maintain performance
- [ ] Ensure visualizations update smoothly

### Step 5: Implement Additional Quality-of-Life Features
- [ ] Add sample auto-naming (remove file extension, format nicely)
- [ ] Add context menu on sample slots (right-click options)
- [ ] Add keyboard shortcuts for common actions
- [ ] Add MIDI learn for parameters (if supported by host)
- [ ] Add parameter lock feature to prevent accidental changes
- [ ] Add "clear all samples" button with confirmation
- [ ] Add sample library browser (optional for MVP)
- [ ] Add drag-to-reorder sample slots (optional)

### Step 6: Error Handling and Edge Cases
- [ ] Handle case where no samples are loaded (silent output)
- [ ] Handle case where file paths change between sessions
- [ ] Handle corrupted or invalid audio files gracefully
- [ ] Handle out-of-memory situations
- [ ] Handle incompatible sample rates properly
- [ ] Add user-friendly error messages for all failure scenarios
- [ ] Implement logging system for debugging user issues
- [ ] Test plugin behavior with malformed project files

### Step 7: Cross-Platform Testing
- [ ] Test on Windows 10/11
- [ ] Test on macOS (if applicable)
- [ ] Test with different DAWs:
  - [ ] Ableton Live
  - [ ] FL Studio
  - [ ] Reaper
  - [ ] Logic Pro (macOS)
  - [ ] Cubase/Nuendo
- [ ] Test VST3 compatibility across hosts
- [ ] Test standalone version functionality
- [ ] Verify UI scales correctly on different screen DPIs
- [ ] Test with different audio interfaces and sample rates

---

## Phase 9: Testing & Quality Assurance (Estimated Time: 1 Week)
**Goal:** Thoroughly test the plugin to ensure reliability, stability, and professional quality before release.

### Step 1: Functional Testing
- [ ] Test sample loading from all supported formats (WAV, AIFF, FLAC, OGG)
- [ ] Test MIDI input across all 88 keys (entire keyboard range)
- [ ] Test all parameter changes individually
- [ ] Test all parameter changes together (interaction testing)
- [ ] Test randomization with various bound settings
- [ ] Test preset save and recall
- [ ] Test project save and reload in DAW
- [ ] Test with samples missing from disk
- [ ] Test sample clearing and reloading
- [ ] Test monophonic voice behavior (note stealing)

### Step 2: Audio Quality Testing
- [ ] Test for audio clicks or pops during parameter changes
- [ ] Test for audio glitches during sample loading
- [ ] Test for phase issues in stereo output
- [ ] Test for aliasing or digital artifacts in pitch shifting
- [ ] Test frequency response of EQ bands
- [ ] Test transient shaping accuracy
- [ ] Compare audio quality with reference plugins
- [ ] Test at various sample rates (44.1, 48, 96 kHz)
- [ ] Test with various buffer sizes (64-1024 samples)
- [ ] Verify output levels don't clip or distort

### Step 3: Performance and Stability Testing
- [ ] Measure CPU usage with all samples loaded
- [ ] Measure CPU usage with all effects active
- [ ] Test for memory leaks (load/unload samples repeatedly)
- [ ] Test plugin stability over extended sessions (hours)
- [ ] Test rapid parameter changes (stress test)
- [ ] Test rapid sample loading/unloading
- [ ] Monitor RAM usage with maximum sample count
- [ ] Test performance with large sample files (>100MB)
- [ ] Verify plugin doesn't crash under any normal use
- [ ] Test recovery from abnormal conditions

### Step 4: Compatibility Testing
- [ ] Test in multiple DAW environments
- [ ] Test VST3 format compatibility
- [ ] Test standalone version functionality
- [ ] Test on different Windows versions (10, 11)
- [ ] Test on different macOS versions (if applicable)
- [ ] Test with various audio driver types (ASIO, DirectSound, CoreAudio)
- [ ] Test UI on different screen resolutions
- [ ] Test UI on different DPI settings (100%, 125%, 150%)
- [ ] Verify plugin loads correctly in all tested hosts
- [ ] Test plugin alongside other plugins in large projects

### Step 5: User Experience Testing
- [ ] Have non-developer users test the plugin
- [ ] Observe user workflow and identify confusion points
- [ ] Get feedback on UI clarity and organization
- [ ] Test if users can complete basic tasks without instructions
- [ ] Verify error messages are clear and helpful
- [ ] Check if tooltips provide sufficient guidance
- [ ] Assess whether parameter ranges are intuitive
- [ ] Evaluate if randomization feature is discoverable
- [ ] Gather feedback on sound quality and usefulness
- [ ] Identify any missing features users expect

### Step 6: Documentation Review
- [ ] Create user manual with basic instructions
- [ ] Document all parameters and their ranges
- [ ] Explain MIDI note mapping system clearly
- [ ] Provide examples of common use cases
- [ ] Create quick start guide for new users
- [ ] Document keyboard shortcuts if implemented
- [ ] Include troubleshooting section
- [ ] Add FAQ section based on tester feedback
- [ ] Include audio examples or video tutorial (optional)
- [ ] Proofread all documentation for clarity

### Step 7: Final Bug Fixes and Polish
- [ ] Create list of all bugs found during testing
- [ ] Prioritize bugs by severity (critical, major, minor)
- [ ] Fix all critical and major bugs before release
- [ ] Address minor bugs if time permits
- [ ] Verify fixes don't introduce new issues (regression testing)
- [ ] Update version number for release
- [ ] Finalize all UI text and labels
- [ ] Ensure all features work as intended
- [ ] Conduct final round of testing after bug fixes

---

## Phase 10: Release Preparation (Estimated Time: 3-4 Days)
**Goal:** Prepare the plugin for commercial release with proper packaging, distribution, and marketing materials.

### Step 1: Build Release Versions
- [ ] Build plugin in Release configuration (optimized, no debug symbols)
- [ ] Build for all target platforms (Windows VST3, macOS VST3/AU if applicable)
- [ ] Build standalone version
- [ ] Test release builds thoroughly
- [ ] Verify release builds have no debug code or logging
- [ ] Check file sizes are optimized
- [ ] Ensure all dependencies are included
- [ ] Test installation process

### Step 2: Create Installer
- [ ] Choose installer solution (Inno Setup for Windows, Packages for macOS)
- [ ] Create installer that copies plugin to correct system directories
- [ ] Include uninstaller option
- [ ] Add license agreement to installer
- [ ] Add option to install standalone version
- [ ] Include user manual or link to online documentation
- [ ] Test installer on clean system
- [ ] Verify plugin appears in DAW after installation
- [ ] Ensure uninstaller removes all files

### Step 3: Licensing and Copy Protection
- [ ] Decide on licensing model for MVP (simple license key vs online activation)
- [ ] Implement basic license key validation (if using license keys)
- [ ] Decide on copy protection level (light protection for MVP recommended)
- [ ] Consider using JUCE's built-in online unlocking feature
- [ ] Test license activation and validation
- [ ] Ensure plugin works offline after activation
- [ ] Implement grace period for license verification
- [ ] Create license generation system for customers

### Step 4: Prepare Marketing Materials
- [ ] Create product webpage with feature list
- [ ] Write compelling product description
- [ ] Create demo videos showing plugin in action
- [ ] Create audio examples demonstrating capabilities
- [ ] Design product logo and branding
- [ ] Create user manual PDF
- [ ] Prepare press release (optional)
- [ ] Create social media posts announcing launch
- [ ] Prepare email newsletter for launch
- [ ] Set up customer support email/system

### Step 5: Set Up Distribution and Sales
- [ ] Create account on plugin marketplace (e.g., Plugin Boutique, Gumroad)
- [ ] Upload plugin files and installers
- [ ] Set pricing for Round Robin Lite (MVP version)
- [ ] Configure payment processing
- [ ] Set up automated license delivery system
- [ ] Create free trial/demo version (optional but recommended)
- [ ] Test purchase and download process
- [ ] Prepare customer support resources
- [ ] Set up analytics to track sales and downloads

### Step 6: Beta Testing Program
- [ ] Recruit 5-10 beta testers
- [ ] Provide beta testers with free licenses
- [ ] Create feedback form or survey
- [ ] Gather feedback on bugs, features, usability
- [ ] Address critical feedback before public release
- [ ] Create case studies or testimonials from beta testers
- [ ] Thank beta testers with credits or lifetime licenses
- [ ] Use beta feedback to prioritize post-launch updates

### Step 7: Launch Plan
- [ ] Set official launch date
- [ ] Coordinate marketing efforts around launch
- [ ] Prepare email to existing contacts/mailing list
- [ ] Post on social media platforms
- [ ] Submit to plugin news sites and blogs
- [ ] Offer launch discount or early bird pricing
- [ ] Monitor initial sales and downloads
- [ ] Respond to customer inquiries promptly
- [ ] Address any critical issues immediately
- [ ] Celebrate successful launch!

---

## Phase 11: Post-Launch & Iteration (Ongoing)
**Goal:** Support users, gather feedback, and plan future updates including advanced "Pro" version.

### Step 1: Customer Support
- [ ] Set up support email or ticket system
- [ ] Monitor support requests daily
- [ ] Create FAQ based on common questions
- [ ] Respond to all support emails within 24 hours
- [ ] Track common issues for future updates
- [ ] Maintain positive relationship with customers
- [ ] Provide refunds if necessary (within policy)
- [ ] Thank users for feedback and suggestions

### Step 2: Bug Fixes and Updates
- [ ] Monitor crash reports and bug reports
- [ ] Prioritize bugs by severity and user impact
- [ ] Release bug fix updates promptly (v1.0.1, v1.0.2, etc.)
- [ ] Test updates thoroughly before release
- [ ] Notify users of important updates
- [ ] Maintain changelog documenting all changes
- [ ] Ensure update process is smooth for users
- [ ] Consider automatic update notification

### Step 3: Gather User Feedback
- [ ] Send survey to purchasers after 30 days
- [ ] Ask about ease of use, features wanted, bugs encountered
- [ ] Monitor social media mentions and reviews
- [ ] Read reviews on marketplaces
- [ ] Engage with users in audio production forums
- [ ] Create feature request board (Trello, GitHub Issues, etc.)
- [ ] Analyze feedback for trends and priorities
- [ ] Communicate roadmap to users

### Step 4: Plan Advanced "Pro" Version
- [ ] Based on feedback, identify premium features:
  - [ ] More sample slots (40, 60, or unlimited)
  - [ ] More EQ bands (5-band or parametric EQ)
  - [ ] Advanced modulation system (LFOs, envelopes)
  - [ ] Per-sample parameter randomization
  - [ ] Velocity sensitivity for samples
  - [ ] Round-robin with multiple samples per note (actual round-robin)
  - [ ] Advanced transient shaping with more controls
  - [ ] Built-in effects (reverb, delay, compression)
  - [ ] Sample editor with trim, fade, normalize
  - [ ] MIDI mapping and control customization
- [ ] Prioritize features that add most value
- [ ] Create development roadmap for Pro version
- [ ] Set pricing strategy for Pro version
- [ ] Plan upgrade path for Lite users

### Step 5: Community Building
- [ ] Create user forum or Discord server
- [ ] Share tips, tricks, and workflows
- [ ] Encourage users to share presets and samples
- [ ] Feature user creations on social media
- [ ] Run contests or challenges
- [ ] Build brand recognition in target market
- [ ] Engage with audio production community
- [ ] Attend trade shows or virtual events (if budget allows)

### Step 6: Long-term Sustainability
- [ ] Monitor sales trends and adjust pricing/marketing as needed
- [ ] Plan regular content updates (new presets, tutorials)
- [ ] Consider expanding to other platforms (iOS/AU3, AAX for Pro Tools)
- [ ] Evaluate performance metrics and user retention
- [ ] Plan for continued development and support
- [ ] Build reserves for development of Pro version
- [ ] Stay updated with JUCE framework updates
- [ ] Maintain compatibility with new OS versions

---

## Success Criteria for MVP

### Core Functionality
- [ ] Plugin loads in all tested DAWs without errors
- [ ] All 20 sample slots load and play samples correctly
- [ ] MIDI note mapping works as designed (C0-C4, paired keys)
- [ ] Monophonic playback functions properly
- [ ] All 11 parameters work and sound good
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

---

## Estimated Total Development Time

- Phase 1: Foundation & Setup - 1-2 days
- Phase 2: Core Audio Engine - 1 week
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

## Key Resources and Learning Materials

### JUCE Documentation
- [ ] JUCE Official Documentation (docs.juce.com)
- [ ] JUCE Tutorials (especially Synthesiser and Plugin tutorials)
- [ ] JUCE API Reference
- [ ] JUCE Forum for troubleshooting

### Video Tutorials
- [ ] The Audio Programmer YouTube channel
- [ ] JUCE Official YouTube channel
- [ ] Audio plugin development courses on Udemy/Skillshare

### Books
- [ ] "Designing Audio Effect Plugins in C++" by Will Pirkle
- [ ] "The Audio Programming Book" (MIT Press)
- [ ] JUCE framework-specific books and resources

### Community
- [ ] JUCE Forum (forum.juce.com)
- [ ] Audio developer Discord servers
- [ ] KVR Audio developer forum
- [ ] Reddit r/AudioProgramming

---

## Notes for Future Reference

- Remember to keep MVP scope limited - resist feature creep
- Test early and often to catch issues before they compound
- Prioritize audio quality over visual polish for MVP
- Get user feedback as soon as possible
- Document code as you go for future maintenance
- Consider hiring a designer for UI in Pro version
- Build relationships with other plugin developers
- Stay focused on core value proposition: easy round-robin sampling

---

**Good luck with development! This is an achievable and exciting project. Take it one phase at a time, and you'll have a working plugin before you know it.**