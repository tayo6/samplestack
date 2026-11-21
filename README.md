# SampleStack

A personal-use sampler instrument inspired by the workflow and visual language of Ableton Sampler/Simpler, with a dedicated pre-sample (Snapback) layer. SampleStack uses its own branding, artwork and implementation.

## Functional sample and waveform milestone

- Load one main audio sample and one pre-sample/Snapback audio file.
- MIDI note-on triggers both layers; pre-sample can be delayed independently.
- Main, pre-sample and master gain are automatable parameters.
- Each lane has draggable start and end markers.
- Each lane has draggable loop-start and loop-end markers plus an independent loop toggle.
- Playback cursors are displayed on both waveform lanes.
- Both lanes share a time ruler while retaining their own duration and colour.
- Waveform drawing reads editor-owned snapshots, never live audio-thread sample buffers.

## Architecture

```
Source/
├── Audio/       # Decoded sample data, MIDI-triggered sample voices and DSP
├── Parameters/  # Host-automatable parameters and marker values
├── Plugin/      # JUCE host adapter, file chooser, state and window sizing
└── UI/          # Custom look, envelope graph, waveform drawing and controls
```

## Playback behavior

SampleStack currently uses one main voice and one pre-sample voice. A MIDI note-on restarts both loaded layers. MIDI note-off does not stop a voice; playback is one-shot unless the corresponding loop is enabled. MIDI note 60 is the current root pitch.

## Build

JUCE 7.0.12 is fetched and pinned through CMake. Install Visual Studio 2022 with the C++ desktop workload and v142 toolset, plus CMake 3.22 or newer.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -T v142
cmake --build build --config Release --target SampleStack_VST3 --parallel 2
```

The Windows GitHub Actions workflow builds a 64-bit VST3 bundle and uploads **SampleStack-VST3-x64**.

## Windows 7 compatibility

The project requests the Windows 7 API baseline and uses the v142 toolset in CI. This remains a compatibility target, not a guarantee: test the built VST3 on Windows 7 SP1 x64 and in the intended FL Studio version. Microsoft runtime components and updates may be necessary. A successful GitHub Actions build alone does not prove Windows 7 runtime compatibility.

## Threading rules

- File decoding and waveform snapshot copies occur off the audio callback.
- The audio callback uses a non-blocking try-lock when sample data may be replaced.
- UI paint code only reads its own waveform snapshots.
- Parameter values are copied once per audio block and passed into the engine.

## JUCE licensing

JUCE 7 licensing terms: https://juce.com/juce-7-licence
