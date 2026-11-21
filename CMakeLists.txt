cmake_minimum_required(VERSION 3.22)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING
    "Use the dynamically linked Microsoft C runtime")

project(SampleStack VERSION 0.5.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SampleStack
    COMPANY_NAME "SSYMA Records"
    BUNDLE_ID com.ssymarecords.samplestack
    PLUGIN_MANUFACTURER_CODE Ssym
    PLUGIN_CODE Smpl
    PRODUCT_NAME "SampleStack"
    FORMATS VST3
    VST3_CATEGORIES Instrument
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(SampleStack
    PRIVATE
        Source/Plugin/PluginProcessor.cpp
        Source/Plugin/PluginEditor.cpp
        Source/Audio/SampleLayer.cpp
        Source/Audio/SamplerEngine.cpp
        Source/Parameters/SamplerParameters.cpp
        Source/UI/CustomLookAndFeel.cpp
        Source/UI/EnvelopeEditor.cpp
        Source/UI/WaveformDisplay.cpp
        Source/UI/SampleStackMainView.cpp
)

target_compile_definitions(SampleStack
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

if(WIN32)
    target_compile_definitions(SampleStack
        PRIVATE
            WINVER=0x0601
            _WIN32_WINNT=0x0601
            NTDDI_VERSION=0x06010000
    )
endif()

target_link_libraries(SampleStack
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_formats
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)
