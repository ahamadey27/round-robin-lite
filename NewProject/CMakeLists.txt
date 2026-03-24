cmake_minimum_required(VERSION 3.22)
project(RoundRobinLite VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# JUCE — absolute path to local installation
# ============================================================
add_subdirectory(/Users/alex/Documents/JUCE "${CMAKE_BINARY_DIR}/JUCE")

# ============================================================
# Plugin target
# ============================================================
juce_add_plugin(RoundRobinLite
    COMPANY_NAME                    "roundrobinlite"
    PLUGIN_CODE                     "rrll"
    PLUGIN_MANUFACTURER_CODE        "Rrlt"
    PRODUCT_NAME                    "round-robin-lite"

    IS_SYNTH                        TRUE
    NEEDS_MIDI_INPUT                TRUE
    NEEDS_MIDI_OUTPUT               FALSE
    IS_MIDI_EFFECT                  FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS     FALSE
    COPY_PLUGIN_AFTER_BUILD         FALSE

    FORMATS                         VST3 AU Standalone
)

# ============================================================
# Source files
# NOTE: RRvoice uses lowercase 'v' to match filenames on disk
# ============================================================
target_sources(RoundRobinLite
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h

        Source/Audio/RRvoice.cpp
        Source/Audio/RRvoice.h
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
# Include directories
# ============================================================
target_include_directories(RoundRobinLite
    PRIVATE
        Source
        Source/Audio
        Source/Data
        Source/DSP
        Source/Parameters
        Source/UI
)

# ============================================================
# Compile definitions
# ============================================================
target_compile_definitions(RoundRobinLite
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
    PRIVATE
        JUCE_STRICT_REFCOUNTEDPOINTER=1
)

# ============================================================
# Linked libraries — all modules from NewProject.jucer
# ============================================================
target_link_libraries(RoundRobinLite
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
