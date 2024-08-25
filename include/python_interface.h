#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <string>
#include "utils.h"
#include "RocketSim.h"
#include "shared_memory.h"
#include "python_bot.h"
#include "DataStream/DataStreamOut.h"
#include "Sim/BallPredTracker/BallPredTracker.h"

using namespace RocketSim;

class PythonInterface {
public:
    const size_t size = 14952;

    int totalTicks;
    std::string shmName;
    SharedMemory* shm;
    PythonBot* blueBot;
    PythonBot* orangeBot;
    CarControls blueControls;
    CarControls orangeControls;

    PythonInterface(int matchId, int totalTicks);
    ~PythonInterface();

    void SendGameState(Arena* arena, BallPredTracker* ballPred, bool isKickoff, bool isOvertime, int blueScore, int orangeScore);
    void SetControllers();
    void GetControllers();

};
