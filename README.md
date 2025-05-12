**massinissaghoul-voxel-game-engine-opengl**

A simple voxel-based game engine written in C++20 and OpenGL 3.3, featuring:

* Procedural world generation with FastNoiseLite (OpenSimplex2 FBm).
* Dynamic chunk loading and face-culling for efficient rendering.
* Block placement and destruction via ray casting.
* Camera controls with gravity, jumping, and collision detection.
* ImGui-powered admin GUI for tweaking generation parameters at runtime.
* FPS counter and GPU/VRAM usage diagnostics.

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Directory Structure](#directory-structure)
3. [Building](#building)
4. [Running](#running)
5. [Controls](#controls)
6. [Configuration](#configuration)
7. [TODOs & Roadmap](#todos--roadmap)


---

## Prerequisites

* **C++20**
* **CMake** (≥ 3.10)
* **OpenGL** 3.3 Core Profile
* **GLFW** (window/context/input)
* **GLAD** (OpenGL loader)
* **GLM** (math library)
* **stb\_image** (texture loading)
* **ImGui** (GUI)
* **FastNoiseLite** (noise generation)

On Debian/Ubuntu-based systems, you can install dependencies via:

```bash
sudo apt-get install cmake libglfw3-dev libglm-dev
# GLAD, stb_image, ImGui and FastNoiseLite are included as submodules or headers
```

---

## Directory Structure

```text
massinissaghoul-voxel-game-engine-opengl/
├── CMakeLists.txt            # Top-level CMake configuration
├── README.md                 # This file
├── done.md                   # Completed features log
├── source.md                 # Learning resources and references
├── todoDay.md                # Daily TODO list and future roadmap
└── src/                      # Engine source code
    ├── CMakeLists.txt        # CMake config for engine
    ├── main.cpp              # Entry point and render loop
    ├── adminGui.cpp/.hpp     # ImGui admin panel
    ├── block.cpp/.hpp        # Individual block mesh setup and rendering
    ├── blockRegistry.cpp/.hpp# Block definitions (AIR, GRASS, DIRT, STONE)
    ├── camera.cpp/.hpp       # FPS camera with physics and ray casting
    ├── chunk.cpp/.hpp        # Chunk mesh builder and face culling
    ├── world.cpp/.hpp        # World manager: loading/unloading chunks
    ├── worldGeneration.cpp/.hpp # Procedural terrain generator
    ├── textureAtlas.cpp/.hpp# Texture atlas loader and UV mapper
    ├── shader.cpp/.h         # Shader program compilation and utilities
    ├── loadSky.cpp/.hpp      # (Optional) fullscreen sky quad loader
    ├── glad/                 # GLAD loader sources
    ├── linking/              # Third-party headers: ImGui, stb, FastNoiseLite
    ├── shader/               # GLSL shader files (*.vs, *.fs)
    └── texture/              # Texture assets (atlas.png, sky.png, etc.)
```

---

## Building

```bash
# Create and enter a build directory
mkdir build && cd build
# Generate makefiles
cmake ..
# Compile
make -j$(nproc)
```

This will produce the executable `OpenGLWindow` in your build directory.

---

## Running

```bash
./OpenGLWindow
```

On launch, a window will open with a procedurally generated voxel world. You can interact and adjust settings in real time via the Admin GUI window.

---

## Controls

| Action                             | Key / Mouse        |
| ---------------------------------- | ------------------ |
| Move Forward / Back / Left / Right | W / S / A / D      |
| Jump                               | Spacebar           |
| Toggle Mouse Capture               | Esc                |
| Place Block                        | Right click / O    |
| Destroy Block                      | Left click / U     |
| Toggle Wireframe / Fill            | F (Fill), L (Line) |
| Exit                               | N                  |

---

## Configuration

The admin GUI (ImGui) allows you to tweak world generation parameters at runtime:

* **Seed**: regenerate with a new random seed.
* **Fractal Octaves**: number of noise octaves \[2–18].
* **Frequency**: base frequency of the noise \[0.01–0.05].
* **Lacunarity**: fractal lacunarity \[1.5–3.0].
* **Gain**: fractal gain \[0.3–0.8].
* **Amplitude**: height amplitude \[2.0–50.0].

Click **Regenerate World** to rebuild all loaded chunks with the current settings.


## TODOs & Roadmap

* MultiThreading
* Add mob / entity support
* Improve shader abstraction and error logging
* Optimize chunk streaming and GPU memory usage
* Add textures, biomes, and lighting effects

---
