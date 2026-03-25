# Audio-Player1

A desktop audio player built with C++ and JUCE. The project provides a dual-player interface so two audio tracks can be loaded, controlled, and played from the same window.

## Overview

This application is organized around two independent player panels. Each panel manages its own audio source, playback controls, waveform display, metadata view, and playlist entries. The main window mixes the output of both players, allowing both tracks to play at the same time.

## Features

- Dual audio player layout
- Load and play multiple audio files
- Independent playback controls for each player
- Volume control
- Playback speed control
- Seek bar with current time / total time display
- Restart, stop, pause, mute, jump to start, and jump to end controls
- Standard looping
- A-B loop point selection
- Waveform visualization
- Playlist-style buttons for loaded tracks
- Metadata display using TagLib
- Mixed output when both players are active

## Tech Stack

- C++
- [JUCE](https://juce.com/) for GUI and audio playback
- [TagLib](https://taglib.org/) for reading audio metadata

## Project Structure

```text
Audio-Player1/
|-- Source/
|   |-- Main.cpp
|   |-- MainComponent.h
|   |-- MainComponent.cpp
|   |-- PlayerAudio.h
|   |-- PlayerAudio.cpp
|   |-- PlayerGUI.h
|   `-- PlayerGUI.cpp
`-- .gitignore
```

## How It Works

### `Main.cpp`
Creates the JUCE application and main window.

### `MainComponent`
Owns two `PlayerGUI` instances and mixes their audio output into the main audio callback.

### `PlayerAudio`
Handles low-level audio tasks:

- loading files
- playback start/stop
- transport position
- gain control
- speed control through resampling
- loop configuration

### `PlayerGUI`
Implements the user interface for one player:

- transport buttons
- sliders
- waveform drawing
- playlist buttons
- metadata display
- A-B loop logic

## Supported Audio Formats

The source currently loads:

- `*.wav`
- `*.mp3`

Actual format support depends on how JUCE is configured in your build.

## Requirements

To build this project, you will need:

- A C++ compiler with JUCE support
- JUCE installed and linked in your project
- TagLib installed and linked in your project
- A JUCE project setup generated manually, with Projucer, or with CMake

## Build Notes

This repository currently contains the source files only. It does not include:

- a `CMakeLists.txt`
- a `.jucer` project file
- IDE project files

That means you will need to create the build configuration yourself before compiling.

## Running the App

Once built, the app opens a window containing two audio players. For each player, you can:

1. Load one or more audio files.
2. Click a generated playlist button to start playback.
3. Adjust volume, speed, and position.
4. Use loop or A-B loop controls for repeat playback.
5. View waveform and available metadata for the selected file.

## Current Notes

- Files are loaded through a file chooser and added to an in-memory playlist.
- Metadata is shown when available from the selected track.
- When both players are active, the main component mixes both signals and applies a small gain reduction.

## Future Improvements

- Add a `CMakeLists.txt` or `.jucer` file
- Improve duplicate file handling in playlists
- Add drag-and-drop file loading
- Add better error handling for unsupported or unreadable files
- Save and restore playlists between sessions
- Improve styling and responsive layout behavior

## License

No license file is currently included in this repository. If you plan to publish or share the project, adding a license is recommended.
