# SampleStack

A personal-use sampler instrument inspired by Ableton Sampler/Simpler, with a dedicated pre-sample (Snapback) layer.

## Milestone 1: plugin foundation

- JUCE 7.0.12 pinned through CMake FetchContent.
- C++17 VST3 instrument, 64-bit.
- MIDI input enabled.
- Resizable placeholder editor.
- Processor intentionally outputs silence until sample playback is implemented.

## GitHub Actions

The workflow builds on a Windows 2022 runner using the Visual Studio 2019 (v142) toolset and uploads an artifact named `SampleStack-VST3-x64`.

## Local build

Install Visual Studio 2022 with the C++ desktop workload and v142 toolset, plus CMake 3.22 or newer.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -T v142
cmake --build build --config Release --target SampleStack_VST3 --parallel 2
```

Output bundle: `build/SampleStack_artefacts/Release/SampleStack.vst3`.

## Windows 7 compatibility

The project will be built with the v142 toolset and JUCE 7.0.12. This is a compatibility target, not a guarantee: the produced VST3 must be tested on Windows 7 SP1 x64 and in the intended FL Studio version. The required Microsoft runtime and Windows updates may be necessary. A successful GitHub Actions build alone does not prove Windows 7 runtime compatibility.

## Planned milestones

1. Build and load the plugin in FL Studio.
2. Implement sample loading and playback.
3. Draw main and pre-sample waveforms in separate colours.
4. Add trim, loop, reverse, pitch and gain controls.
5. Add editable multi-curve envelopes.
6. Add modulation, filtering and independent DAW outputs.
7. Complete the Ableton-inspired layout and maximized editor mode.

## JUCE

JUCE 7 licensing terms: https://juce.com/juce-7-licence
