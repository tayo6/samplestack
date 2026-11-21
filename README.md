# SampleStack

A personal-use sampler instrument inspired by the workflow and visual language of Ableton Sampler/Simpler, with a dedicated pre-sample (Snapback) layer. SampleStack uses its own branding, artwork and implementation.

## UI architecture

```
Source/UI/
├── CustomLookAndFeel.h/.cpp # Shared JUCE button and rotary-knob drawing
├── EnvelopeEditor.h/.cpp    # Draggable multi-point envelope graph
├── SampleStackMainView.h/.cpp # Header, navigation, page layout and expand request
├── SampleStackTheme.h       # Shared colours and visual tokens
└── WaveformDisplay.h/.cpp   # Shared ruler and separate pre/main waveform lanes
```

The plug-in editor shell owns host-window sizing and delegates all UI composition to `SampleStackMainView`. Audio processing and sample storage remain in `Source/Audio`; host integration and state persistence remain in `Source/Plugin`; parameter identifiers/layout remain in `Source/Parameters`.

## Current UI milestone

- Dark, compact sampler layout with Sample, Pitch/Osc, Filter, Mod and MIDI pages.
- Shared custom LookAndFeel for controls.
- Two aligned waveform lanes on a common time ruler: amber pre-sample and blue main sample.
- Draggable envelope control points in the UI.
- Expand/restore button requests a display-sized editor when the host permits plug-in resizing.
- Sample playback and DSP binding for the envelope are not implemented yet. Waveform lanes therefore display explicit empty states until actual sample data is loaded.

## Build

JUCE 7.0.12 is fetched and pinned through CMake.

Install Visual Studio 2022 with the C++ desktop workload and the v142 toolset, plus CMake 3.22 or newer.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -T v142
cmake --build build --config Release --target SampleStack_VST3 --parallel 2
```

The Windows GitHub Actions workflow builds a 64-bit VST3 bundle and uploads `SampleStack-VST3-x64`.

## Windows 7 compatibility

The project requests the Windows 7 API baseline and uses the v142 toolset in CI. This remains a compatibility target, not a guarantee: test the built VST3 on Windows 7 SP1 x64 and in the intended FL Studio version. Required Microsoft runtime components and Windows updates may be necessary. A successful GitHub Actions build alone does not prove Windows 7 runtime compatibility.

## Development rules

- Keep audio processing out of UI classes.
- Keep JUCE host callbacks in the Plugin layer.
- Keep parameter IDs in the Parameters module; do not duplicate parameter strings in controls.
- UI components must not own or mutate audio-thread sample buffers.
- Before sample loading or playback, define a thread-safe ownership/update path for waveform snapshots.
- Put new DSP features in Audio/ and UI-only drawing/interaction in UI/.

## Planned milestones

1. Verify the UI milestone builds and loads in FL Studio.
2. Implement sample loading and playback voices.
3. Connect the envelope editor to voice parameters.
4. Add trim/loop/start/end marker editing.
5. Add pre-sample timing and independent layer controls.
6. Add filtering, modulation and independent DAW outputs.
7. Finish the maximized interface and host resize behavior.

## JUCE licensing

JUCE 7 licensing terms: https://juce.com/juce-7-licence
