# MonkeyTyper

**MonkeyTyper** is a C++ game using the **SFML** library designed to help players practice fast typing of words against the clock. Players type appearing words while racing against the timer, all within a friendly graphical interface featuring sound effects and game settings.

## Features

- Time-based fast-paced gameplay
- Intuitive main menu
- Game settings (e.g., difficulty level, sound)
- Sound effects
- Clean and modular C++ code
- Graphical window with event handling (SFML)

## Requirements

- C++20
- SFML 2.5 or newer
- CMake

## How to run

1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/MonkeyTyper.git

    Enter the project directory:

cd MonkeyTyper

Build the project using CMake:

mkdir build
cd build
cmake ..
make

Run the executable:

    ./MonkeyTyper

Folder structure

MonkeyTyper/
├── assets/         # Fonts, images, sounds
└── src/            # Game source code (C++)
