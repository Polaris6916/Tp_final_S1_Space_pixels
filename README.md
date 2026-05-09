# 🎮 Space Pixels

Space Pixels is a Shoot'Em Up game developed in C. It is a pixel art space shooter featuring fast-paced action, enemy waves, and dynamic gameplay.

## 🕹️ About the Project

This project was developed as part of the mid-year project for the first year at ESIEA in 2024-2025.

The repository contains the basecode provided for this project, which serves as the foundation for the game's implementation.

## ✨ Game Concept

Space Pixels is a 2D pixel art Shoot'Em Up where players fight through waves of enemies in space. The gameplay features:

* Fast-paced shooting mechanics
* Enemy wave progression
* Pixel art visual style
* Space-themed arenas

## 🚀 Features

### 👤 Developed by Riwal CLECH

#### 🛸 Enemies

| Enemy | Behavior |
|---|---|
| **Battlecruiser** | Fires bullets in a circular pattern |
| **Torpedo** | Fires guided missiles |
| **Scout** | Fires high-velocity bullets |
| **Frigate** | Fires highly-guided red missiles |

#### 💥 Projectiles

| Projectile | Behavior |
|---|---|
| **Scout bullet** | High-velocity straight shot |
| **Blue missile** | Slightly guided toward the player |
| **Red missile** | Highly guided toward the player (fired by Frigate) |
| **Battlecruiser bullet** | Fires in a circular spread pattern |

#### ⚙️ Systems
* Enemy spawning system
* Circular movement patterns for enemy ships
* Collision detection between player bullets and red missiles (Frigate)
* Collision detection between the Dash ship and enemies (damage handled by Contributor 2)

---

### 👤 Developed by Anatole BARBOUX

#### 🎛️ Main Menu
* Player count selection
* Music toggle (enable/disable)
* Sound effects toggle (enable/disable)
* Ship type selection (Basic or Dash)

#### 🚀 Player Ship Types

| Ship | Description |
|---|---|
| **Basic** | Standard ship with shooting mechanics |
| **Dash** | No bullets — attacks by ramming enemies; deploys an animated shield while dashing and takes no damage during the charge |

#### ❤️ Health Bars
* Enemy health bars
* Player health bars

---

## 👥 Contributors

Game development:

* *Riwal CLECH*
* *Anatole BARBOUX*

Base code:

* Arnaud BANNIER (professor from ESIEA)
* Nicolas BODIN (professor from ESIEA)

## 🎓 Academic Context

This project was created as part of the first-year mid-year project at ESIEA (2024-2025).

## License

All code produced for this project is licensed under the __[MIT license](/LICENSE.md)__. It uses the __[SDL2](https://github.com/libsdl-org/SDL)__, __[SDL2\_image](https://github.com/libsdl-org/SDL_image)__, __[SDL2\_mixer](https://github.com/libsdl-org/SDL_mixer)__ and __[SDL2\_ttf](https://github.com/libsdl-org/SDL_ttf)__ libraries.

Game asset licenses are available in the assets folder.

## Building

### Building for Visual Studio (Windows)

This project was developed for Windows under Visual Studio. To compile this project, make sure you have correctly installed the following tools:

* __[Visual Studio](https://visualstudio.microsoft.com/)__ with "Desktop Development in C++" option,
* __[CMake](https://cmake.org/download/)__.

Rename the `build_vs.txt` file to `build_vs.bat` and double-click on this file to run the script. The Visual Studio solution is created in the `_build_vs` folder.

If you wish to compile the project manually, you can run the following commands:

```
mkdir _build_vs
cd _build_vs
cmake .. -G "Visual Studio 18 2026" -A x64 -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build . --config Release
```

### Building for Linux

Use the following commands to install project dependencies:

```
sudo apt install cmake
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install libsdl2-ttf-dev
sudo apt install libsdl2-mixer-dev
```

Then run the following commands:

```
mkdir _build_linux
cd _build_linux
cmake ..
cmake --build .
../_bin/application
```