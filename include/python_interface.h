#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>  
#include <iostream>
#include <string>
#include <chrono>
#include "utils.h"
#include "RocketSim.h"

using namespace RocketSim;

class PythonInterface {
public:
    HANDLE hPipe;
    std::string pipeName;

    PythonInterface(const std::string& pipeName);
    ~PythonInterface();

    bool WaitForClient();
    bool Write(const std::string& state);
    bool Read(std::string& action, CarControls* controller);
    static std::string SerializeGamestate(Arena* arena, Car* blueCar, Car* orangeCar, int totalTicks, bool isKickoff);
};
