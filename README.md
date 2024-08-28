# SQRL-TournamentRunner
A tournament runner that utilizes RocketSim to simulate matches specifically built for SkillQuest
- Currently still a W.I.P.
- There may be bugs that I don't know about, so please report them!
- Code has never been optimized, will come soon

# Requirements
- CMake
- Visual Studio 2019 or Visual Studio 2022

# Use
1. Clone this repo using `git clone https://github.com/JoshyDevRL/SQRL-TournamentRunner` or [Download](https://github.com/JoshyDevRL/SQRL-TournamentRunner/archive/refs/heads/main.zip) the repo and extract the zip into a folder
2. Open the folder in cmd and type in these commands in order:
  - `mkdir build`
  - `cd build`
  - `cmake ..`
3. Open the generated solution in the build folder and build the exe with Release mode

## Python Bot Support
- Python 3.10 or higher
- the [sqrl_python_interface](https://pypi.org/project/sqrl-python-interface) python package

# Features
- Can start a 1v1 match for x amount of ticks (1 second = 120 ticks). Default is 36.000 ticks (5 minutes)
- **--blue / --orange path_to_your_bot_py** flag guides the program to your bot file > [REQUIRED]
- **--ticks x** flag to set the desired amount of ticks to run a match for, where **x** is the amount of ticks > [OPTIONAL]
- **--render** flag to render the match happening in [RocketSimVis](https://github.com/ZealanL/RocketSimVis) (Slows down the game to real time) > [OPTIONAL]
- **--copy** flag to copy your bot folder to the exe path so it doesn't modify your main code files > [OPTIONAL]

# Limitations
- 1v1 matches only
- Boostpad states not being sent correctly to the visualizer [FIXING SOON]
- Gametickpacket does not posses all the data (Currently has enough to support 99% of bots) [FIXING NEXT UPDATE]
- Bug where the main program does not exit, most likely because of the python interface exiting already?
- ~~Weird desync issues with python~~
- ~~Currently does not have ball-prediction~~
- ~~If a bot uses quickchats it will break~~
- ~~Can't use different bots for each team~~
- ~~Overtime does not work~~
- ~~Kickoff bug where the timer is incorrect~~

# Performance
> **1 thread | 1v1 | Simple C++ ATBA | 36.000 ticks (5 minutes) | Named Pipe |> ~150 ms**\
> **1 thread | 1v1 | SkillQuest PY BOT | 36.000 ticks (5 minutes) | Named Pipe |> ~4.000 ms**\
> **1 thread | 1v1 | SkillQuest PY BOT + Ball Prediction | 36.000 ticks (5 minutes) | Named Pipe |> ~170.000 ms**\
> **1 thread | 1v1 | SkillQuest PY BOT | 36.000 ticks (5 minutes) | Shared Memory |> ~250 ms**\
> **1 thread | 1v1 | SkillQuest PY BOT + Ball Prediction | 36.000 ticks (5 minutes) | Shared Memory |> ~115.000 ms**

# Near-Future
- Add player info tracker, eg. shots, saves, assists [ADDING NEXT UPDATE]
- Simple TUI (text-based user interface)
- Add Full RLBot Python bot support
- Add more game modes; 2v2 and 3v3
- Tournament runner framework
- Multiple threads and games at the same time
- Seperate and/or disable printing from the python agents
- ~~Add support for Skillquest and GoslingUtils bots~~
- ~~Switch from Named Pipes to Shared Memory for the Python Interface~~

# Future
- Optimize framework / code
- Full customizability
- Fully functional Replay System
- Add RLBot C# bot support
- Add RLBot C++ bot support
- Add RLBot ML bot support
- API for GUI interface
- Add more variety game modes, eg. uneven teams and some extra-modes

# Development
- Pull requests are allowed
- Feel free to ask me questions about this project on discord, my username: **joshydev**

# Commits

## Commit [ 554b41c ]
- Replaced --file flag with --blue & --orange (It's possible to use different bots for each team now)
- Switched from Named Pipes to Shared Memory
- Started working on the event tracker, eg. shots, saves, goals
- Changed structure from PythonBot containing a seperate PythonInterface to a single PythonInterface containing all the PythonBots
- Added the 1-tick delay from Rocket League
- Added realistic overtime and game ending functionality (Only end game when ball is on ground and timer ran out or continue if the score is equal and reset to kickoff position)
- Fixed kickoff bug where the timer was incorrect
- Added feature that automatically gets the name of the bot from the .cfg file
- Send match id instead of pipe name to python sqrl_interface
- Update [sqrl_python_interface](https://github.com/JoshyDevRL/sqrl_python_interface) and [pypi package](https://pypi.org/project/sqrl-python-interface) to 0.2.0

## Commit [ ecb5fef ]
- Add full support for GoslingUtils and SkillQuest bots
- Additional features such as the --copy and --file flags
- Python autolaunch feature
- Update [sqrl_python_interface](https://github.com/JoshyDevRL/sqrl_python_interface) and [pypi package](https://pypi.org/project/sqrl-python-interface) to 0.1.0

## Commit [ 9895800 ]
- Start setup of the base framework
- Additional rendering option for rendering in [RocketSimVis](https://github.com/ZealanL/RocketSimVis)
- Starts a 1v1 match for x amount of ticks (1 second = 120 ticks). Default is 36.000 ticks (5 minutes)
- Tested compiling on both Visual Studio 2019 and Visual Studio 2022 with success
