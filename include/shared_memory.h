#pragma once

#include <windows.h>
#include <iostream>

//struct SharedData {
//    float carFloats[38]; // 12 Physics, 3 Hitbox, 3 HitboxOffset, 1 Boost
//    int carInts[16]; // 7 ScoreInfo, 2 Team
//    bool carBools[10]; // demoed, grounded, supersonic, jumped, doublejumped
//    // name
//    bool boostBools[34];
//    float boostFloats[34];
//    float ballFloats[16]; // 9 Physics, HitTime, 3 HitPos, 3 HitNormal
//    int ballInts[2]; // HitTeam, HitIdx
//    float gameFloats[2]; // time, timeRemaining
//    bool gameBools[2]; // overtime, kickoff
//    // teaminfo
//    int tick;
//    int numCars;
//    int bTick;
//    float bControllerF[5];
//    float bControllerB[3];
//    int oTick;
//    float oControllerF[5];
//    float oControllerB[3];
//};

class SharedMemory {
public:
    SharedMemory(const std::string& name, size_t size);
    ~SharedMemory();

    char* GetPBuf();
    void Close();

private:
    HANDLE hMapFile;
    char* pBuf;
    size_t size;
    std::string name;
};

