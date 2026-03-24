# ============================================================
# round-robin-lite — JUCE Audio Plugin
# ============================================================

juce_add_plugin(round-robin-lite
    # Identity
    COMPANY_NAME                    "roundrobinlite"
    PLUGIN_CODE                     "rrll"
    PLUGIN_MANUFACTURER_CODE        "Rrlt"
    PRODUCT_NAME                    "round-robin-lite"

    # Plugin behaviour
    IS_SYNTH                        TRUE
    NEEDS_MIDI_INPUT                TRUE
    NEEDS_MIDI_OUTPUT               FALSE
    IS_MIDI_EFFECT                  FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS     FALSE

    # Build formats (matches .jucer: buildStandalone, buildVST3)
    FORMATS                         VST3 Standalone

    # Don't auto-copy on build by default; set to TRUE to enable
    COPY_PLUGIN_AFTER_BUILD         FALSE
)

# ============================================================
# Source Files
# ============================================================
target_sources(round-robin-lite
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h

        Source/Audio/RRVoice.cpp
        Source/Audio/RRVoice.h
        Source/Audio/RRSound.cpp
        Source/Audio/RRSound.h
        Source/Audio/MidiMapper.h

        Source/Data/SampleLoader.cpp
        Source/Data/SampleLoader.h
        Source/Data/SampleSlot.h

        Source/DSP/RandomizationEngine.cpp
        Source/DSP/RandomizationEngine.h
        Source/DSP/ThreeBandEQ.cpp
        Source/DSP/ThreeBandEQ.h
        Source/DSP/TransientShaper.cpp
        Source/DSP/TransientShaper.h

        Source/Parameters/ParametersIDs.h
        Source/Parameters/RandomizationParams.h

        Source/UI/RRLookAndFeel.cpp
        Source/UI/RRLookAndFeel.h
)

# ============================================================
# Include Directories
# ============================================================
target_include_directories(round-robin-lite
    PRIVATE
        Source
        Source/Audio
        Source/Data
        Source/DSP
        Source/Parameters
        Source/UI
)

# ============================================================
# Compile Definitions
# Matches JUCEOPTIONS in NewProject.jucer
# ============================================================
target_compile_definitions(round-robin-lite
    PUBLIC
        # Disable unused JUCE subsystems to reduce build time
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        # Match .jucer JUCEOPTIONS
        JUCE_VST3_CAN_REPLACE_VST2=0
    PRIVATE
        JUCE_STRICT_REFCOUNTEDPOINTER=1
)

# ============================================================
# Compiler Flags
# /W4 on MSVC matches the VS export in .jucer
# ============================================================
if(MSVC)
    target_compile_options(round-robin-lite PRIVATE /W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    target_compile_options(round-robin-lite PRIVATE -Wall -Wextra)
endif()

# ============================================================
# Linked Libraries
# All 14 modules listed in .jucer MODULES section
# (juce_audio_processors_headless is bundled inside
#  juce_audio_processors in the CMake API)
# ============================================================
target_link_libraries(round-robin-lite
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
