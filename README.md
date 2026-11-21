# SampleStack

A personal-use sampler instrument inspired by Ableton Sampler/Simpler, with a dedicated pre-sample (Snapback) layer.

## Architecture

The code is separated by responsibility so audio processing, UI rendering, parameters and the JUCE host adapter can be changed independently.

```
Source/
├── Audio/
│   ├── SampleLayer.h/.cpp       # Owns one decoded audio layer
│   └── SamplerEngine.h/.cpp     # DSP coordinator and future voice scheduler
├── Parameters/
│   └── SamplerParameters.h/.cpp # Parameter IDs and automatable parameter layout
├── Plugin/
│   ├── PluginProcessor.h/.cpp   # JUCE host adapter and state persistence
│   └── PluginEditor.h/.cpp      # Host editor shell
└── UI/
    ├── SampleStackTheme.h       # Shared colours and visual tokens
    ├── SampleStackMainView.h/.cpp
    └── WaveformDisplay.h/.cpp   # Main and pre-sample waveform drawing
```

## Current milestone

**Modular foundation**

- JUCE 7.0.12 is pinned through CMake FetchContent.
- C++17 VST3 instrument, 64-bit Windows build.
- MIDI input enabled.
- Audio engine and sample-layer ownership are separate from the plug-in wrapper.
- Parameters have their own module and are saved/restored by the processor.
- UI theme, main view and dual-colour waveform display are separate components.
- Audio output is intentionally silent until the voice renderer is implemented.

## Build with GitHub Actions

The workflow builds on a Windows 2022 runner using the Visual Studio 2019 (v142) toolset and uploads an artifact named **SampleStack-VST3-x64**.

## Local build

Install Visual Studio 2022 with the C++ desktop workload and v142 toolset, plus CMake 3.22 or newer.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -T v142
cmake --build build --config Release --target SampleStack_VST3 --parallel 2
```

Output bundle: `build/SampleStack_artefacts/Release/SampleStack.vst3`.

## Windows 7 compatibility

The project requests the Windows 7 API baseline and uses the v142 toolset in CI. This remains a compatibility target, not a guarantee: test the produced VST3 on Windows 7 SP1 x64 and in the intended FL Studio version. Required Microsoft runtime components and Windows updates may be necessary. A successful GitHub Actions build alone does not prove Windows 7 runtime compatibility.

## Development rules

- Keep audio processing out of UI classes.
- Keep JUCE host callbacks in the Plugin layer.
- Keep parameter IDs in the Parameters module; do not duplicate string literals in controls.
- UI components must not own or mutate audio-thread sample buffers.
- Before adding sample loading, playback voices or waveform snapshots, define their thread-safe ownership and update path.
- Keep new DSP features in Audio/ and UI-only drawing/interaction in UI/.

## Planned milestones

1. Verify the modular plugin builds and loads in FL Studio.
2. Implement sample loading and playback voices.
3. Draw main and pre-sample waveforms with a shared timeline and separate colours.
4. Add trim, loop, reverse, pitch and gain controls.
5. Add editable multi-curve envelopes.
6. Add modulation, filtering and independent DAW outputs.
7. Complete the Ableton-inspired layout and maximized editor mode.

## JUCE licensing

JUCE 7 licensing terms: https://juce.com/juce-7-licence
