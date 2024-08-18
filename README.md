# SQRL-TournamentRunner
A tournament runner that utilizes RocketSim to simulate matches specifically built for SkillQuest
- Currently still a W.I.P.
- There may be bugs that I don't know about, so please report them!
- Code has never been optimized, will come soon

# Requirements
- CMake > [REQUIRED]
- Visual Studio 2019 or Visual Studio 2022 > [REQUIRED]

# Use
1. Clone this repo using `git clone https://github.com/JoshyDevRL/SQRL-TournamentRunner` or [Download](https://github.com/JoshyDevRL/SQRL-TournamentRunner/archive/refs/heads/main.zip) the repo and extract the zip into a folder
2. Open the folder in cmd and type in these commands in order:
  - `mkdir build`
  - `cd build`
  - `cmake ..`
4. Open the generated solution in the build folder and build the exe with Release mode

## Python Bot Support
- Python 3.10 or higher
- the `sqrl_python_interface` python package

# Features
- Can start a 1v1 match for x amount of ticks (1 second = 120 ticks). Default is 36.000 ticks (5 minutes)
- **--file path_to_your_bot_py** flag guides the program to your bot file > [REQUIRED]
- **--ticks x** flag to set the desired amount of ticks to run a match for, where **x** is the amount of ticks > [OPTIONAL]
- **--render** flag to visualize/render the match happening in [RocketSimVis](https://github.com/ZealanL/RocketSimVis) (Slows down the time to real time) > [OPTIONAL]
- **--copy** flag to copy your bot folder to the exe path so it doesn't modify your main code files > [OPTIONAL]

# Performance
> **1 thread | 1v1 | Simple C++ ATBA | 36.000 ticks (5 minutes) |> ~150 milliseconds**\
> **1 thread | 1v1 | SkillQuest PY BOT | 36.000 ticks (5 minutes) |> ~4000 milliseconds**

# Near-Future
- Add Full RLBot Python bot support (GoslingUtils and SkillQuest are already supported!)
- Add more game modes; 2v2 and 3v3
- Simple TUI (text-based user interface)
- Tournament runner framework
- Multiple threads and games at the same time

# Future
- Optimize framework / code
- Full customizability
- Fully functional Replay System
- Add RLBot C# bot support
- Add RLBot C++ bot support
- API for GUI interface
- Add more variety game modes, eg. uneven teams and some extra-modes

# Development
- Pull requests are allowed
- Feel free to ask me questions about this project on discord, my username: **joshydev**

# Commits

## Commit [ ecb5fef ]
- Add full support for GoslingUtils and SkillQuest bots
- Additional features such as the --copy and --file flags
- Python autolaunch feature

## Commit [ 9895800 ]
- Start setup of the base framework
- Additional rendering option for rendering in [RocketSimVis](https://github.com/ZealanL/RocketSimVis)
- Starts a 1v1 match for x amount of ticks (1 second = 120 ticks). Default is 36.000 ticks (5 minutes)
- Tested compiling on both Visual Studio 2019 and Visual Studio 2022 with success
