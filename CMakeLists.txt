cmake_minimum_required(VERSION 3.10)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14 CACHE STRING "Build for 10.11")

project(ji-mts-source VERSION 0.9.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include (cmake/CPM.cmake)

CPMAddPackage("gh:juce-framework/JUCE#7.0.4")
add_subdirectory(libs/tuning-library)


juce_add_plugin(${PROJECT_NAME}
    PRODUCT_NAME "JI MTS Source"
    COMPANY_NAME "TBD Tuners"  
    BUNDLE_ID "org.tbd.tuners.jimts"
    PLUGIN_MANUFACTURER_CODE TbdT
    PLUGIN_CODE JImS

    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    
    FORMATS AU VST3 Standalone

    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
  src/
)

add_library(oddsound-mts-source libs/MTS-ESP/Master/libMTSMaster.cpp)
target_include_directories(oddsound-mts-source PUBLIC libs/MTS-ESP/master)

target_sources(${PROJECT_NAME} PRIVATE src/PluginEditor.cpp src/PluginProcessor.cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    JUCE_ALLOW_STATIC_NULL_VARIABLES=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1

    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0

    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0

    JUCE_ALSA=0
    JUCE_JACK=0
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        juce::juce_graphics
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_audio_devices
        tuning-library
        oddsound-mts-source
        )

