# SQRL-TournamentRunner
A tournament runner that utilizes RocketSim to simulate matches specifically built for SkillQuest

# Requirements
- CMake
- Visual Studio 2019 or Visual Studio 2022

# Use
1. Clone this repo using `git clone https://github.com/JoshyDevRL/SQRL-TournamentRunner` or [Download](https://github.com/JoshyDevRL/SQRL-TournamentRunner/archive/refs/heads/main.zip) the repo and extract the zip into a folder
2. Open the folder in cmd and type in these commands in order:
  - `mkdir build`
  - `cd build`
  - `cmake ..`
4. Open the generated solution in the build folder and build the exe with Release mode

# Features
- Can start a 1v1 match for x amount of ticks (1 second = 120 ticks). Default is 36.000 ticks (5 minutes)
- **--ticks x** flag to set the desired amount of ticks to run a match for, where **x** is the amount of ticks 
- **--render** flag to visualize/render the match happening in [RocketSimVis](https://github.com/ZealanL/RocketSimVis) (Slows down the time to real time)

# Performance
> **1 thread | 1v1 | Simple C++ ATBA | 36.000 ticks (5 minutes) |> ~150 milliseconds**

# Near-Future
- Add RLBot Python bot support
- Add more variety game modes, eg. more players, uneven teams and some extra-modes
- Simple TUI (text-based user interface)
- Tournament runner framework
- Multiple threads and games at the same time

# Future
- Add RLBot C# bot support
- Add RLBot C++ bot support
- API for GUI interface

# Development
- Pull requests are allowed
- Feel free to ask me questions about this project on discord, my username: **joshydev**

# Commits

## Commit [ 9895800 ]
- Start setup of the base framework
- Additional rendering option for rendering in [RocketSimVis](https://github.com/ZealanL/RocketSimVis)
- Starts a 1v1 match for x amount of ticks (1 second = 120 ticks). Default is 36.000 ticks (5 minutes)
- Tested compiling on both Visual Studio 2019 and Visual Studio 2022 with success
