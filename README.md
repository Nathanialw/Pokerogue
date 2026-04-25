# PicoRogue

**PicoRogue** is a custom-built Pokémon-inspired roguelike running on the Raspberry Pi Pico (RP2040), featuring a fully original game engine, rendering pipeline, audio system, and asset toolchain.

This project was built from scratch in C as a first embedded and systems programming project, with supporting Python tooling for asset generation and data pipelines.

---

## 🎮 Features

* Tile-based roguelike gameplay
* Turn-based combat system inspired by Pokémon
* Fully custom **effect system** for skills, spells, and items
* Procedural dungeon generation
* Sprite-based rendering with dirty-tile updates
* Real-time audio synthesis (multi-voice, software mixing)
* Battery monitoring and input handling on hardware
* Multicore usage (game loop + background tasks)

---

## 🧠 Technical Highlights

### Embedded Systems (C)

* Runs on **RP2040 (Raspberry Pi Pico)**
* Fixed-memory design (no dynamic allocation)
* Explicit separation of:

    * **Flash data (`g_gameFlash`)**
    * **Runtime state (`g_run`)**
* Packed data structures and memory-conscious layout
* Function-pointer driven systems (combat, tiles, generation)

### Rendering Engine

* Tile-based renderer with **dirty-region updates**
* Cached tile/sprite conversion to minimize redraw cost
* Transitioning toward **partial framebuffer rendering**

### Audio Engine

* Software-generated audio (no external assets required)
* Multi-voice playback with:

    * Oscillators
    * Envelopes
    * Mixing + soft clipping
* I2S output using Pico audio buffers

### Combat System

* **Composable effect system**:

    * Skills, spells, and items are built from reusable effect primitives
    * Allows bespoke, order-dependent behaviors
* Supports complex interactions:

    * Damage, status, healing, repositioning, capture, etc.

### Tooling & Pipeline (Python)

* SQLite-backed content system
* Automatic export of:

    * Names, descriptions, structs → C `.inc` files
* Image processing pipeline:

    * Palette conversion (RGB565)
    * Tile slicing + RLE compression
* Local AI pipeline for:

    * Generating creature descriptions
    * Generating image prompts
* Batch asset generation with GPU-aware job system

---

## 🧱 Architecture Overview

Game runtime:

* `GameCore/` → gameplay systems (combat, map, objects, UI)
* `PicoGame/` → hardware layer (rendering, input, audio)

Data pipeline:

* Python scripts generate assets → exported as `.inc` files → compiled into firmware

Key design principle:

> **Reusable primitives, bespoke composition**

---

## ⚙️ Hardware

* Raspberry Pi Pico (RP2040)
* ILI9341 display (SPI)
* Audio output via I2S
* ADC-based battery monitoring

---

## 🚀 Build & Deploy

```bash
mkdir build-pico
cd build-pico
cmake ..
make
```

Flash to Pico:

```bash
cp PicoRogue.uf2 /media/<user>/RPI-RP2/
```

---

## 🛠️ Project Goals

This project was built to explore:

* Low-level systems programming in C
* Embedded development on constrained hardware
* Custom rendering and audio pipelines
* Data-driven game architecture
* Tooling and automation for content-heavy systems

---

## 📈 Future Work

* Partial framebuffer renderer
* Expanded effect system and combat depth
* Improved asset compression and streaming
* UI/UX polish
* Performance optimization and memory tuning

---

## 📷 Demo

*(Add screenshots or a short video here — highly recommended)*

---

## 👤 Author

Built as a first C / embedded project over ~5–6 weeks.

---

## 💡 Notes

This project prioritizes:

* performance
* flexibility
* system design

over strict abstraction purity, and continues to evolve as systems are refined.

---
