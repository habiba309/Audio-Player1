# 🎧 Audio Player 1

![C++](https://img.shields.io/badge/Language-C++17-blue)
![Framework](https://img.shields.io/badge/Framework-JUCE-orange)
![Status](https://img.shields.io/badge/Status-Active-success)

A desktop **dual audio player** built with C++ and JUCE.
It allows you to load, control, and play **two audio tracks simultaneously** within a single interface.

---

## 📌 Overview

This application features **two independent player panels**, each with its own:

* 🎵 Audio source
* 🎛️ Playback controls
* 📊 Waveform visualization
* 🏷️ Metadata display
* 📂 Playlist

💡 The main window mixes both players, enabling **simultaneous playback**.

---

## ✨ Features

### 🎶 Core Features

* 🎧 Dual audio player interface
* 📂 Load and play multiple audio files
* 🎛️ Independent controls for each player
* 🔊 Volume control
* ⏩ Playback speed control
* ⏱️ Seek bar with time display

### 🔁 Playback Controls

* ▶️ Play / ⏸️ Pause / ⏹️ Stop
* 🔄 Restart
* 🔇 Mute
* ⏮️ Jump to start
* ⏭️ Jump to end
* 🔁 Standard looping
* 🔂 A-B loop selection

### 📊 Visualization & Info

* 📈 Waveform display
* 🧾 Metadata display (via TagLib)
* 📋 Playlist buttons

### ⚡ Advanced

* 🎚️ Mixed output when both players are active

---

## 🧰 Tech Stack

* **C++**
* **JUCE** → GUI & audio engine
* **TagLib** → audio metadata extraction

---

## 📂 Project Structure

```text
Audio-Player1/
├── Source/
│   ├── Main.cpp
│   ├── MainComponent.h
│   ├── MainComponent.cpp
│   ├── PlayerAudio.h
│   ├── PlayerAudio.cpp
│   ├── PlayerGUI.h
│   └── PlayerGUI.cpp
└── .gitignore
```

---

## ⚙️ How It Works

### 🧩 `Main.cpp`

* Initializes the JUCE application
* Creates the main window

### 🧠 `MainComponent`

* Holds two `PlayerGUI` instances
* Mixes their audio output

### 🔊 `PlayerAudio`

Handles:

* File loading
* Playback control
* Transport position
* Volume (gain)
* Speed (resampling)
* Loop configuration

### 🎛️ `PlayerGUI`

Handles:

* UI controls (buttons & sliders)
* Waveform rendering
* Playlist buttons
* Metadata display
* A-B loop logic

---

## 🎵 Supported Audio Formats

* `.wav`
* `.mp3`

⚠️ Actual support depends on your JUCE configuration.

---

## ⚙️ Requirements

To build the project:

* ✅ C++ compiler
* ✅ JUCE installed & linked
* ✅ TagLib installed & linked
* ⚙️ JUCE project setup (Projucer / CMake / manual)

---

## 🛠️ Build Notes

⚠️ This repository includes **source files only**.

Missing:

* ❌ `CMakeLists.txt`
* ❌ `.jucer` file
* ❌ IDE project files

👉 You must configure the build manually.

---

## ▶️ Running the App

After building:

1. 🎵 Load audio files
2. ▶️ Select from playlist to play
3. 🎛️ Adjust volume, speed, position
4. 🔁 Use loop or A-B loop
5. 📊 View waveform & metadata

---

## 📝 Current Notes

* Files are loaded via file chooser
* Stored in an in-memory playlist
* Metadata shown when available
* 🎚️ When both players run → audio is mixed with slight gain reduction

---

## 🚀 Future Improvements

* 📦 Add `CMakeLists.txt` or `.jucer`
* 🔄 Improve duplicate handling
* 🖱️ Drag & drop file support
* ⚠️ Better error handling
* 💾 Save/load playlists
* 🎨 Improve UI styling

---

## 📄 License

⚠️ No license included yet.
👉 It’s recommended to add one if you plan to share or publish the project.

---

## 💡 Author

**C++ / JUCE Audio Player Project**

---