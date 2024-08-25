#include "../include/python_interface.h"

void VTD(char* pBuf, const Vec& vec, int& offset) {
    memcpy(pBuf + offset, &vec.x, sizeof(float));
    offset += sizeof(float);
    memcpy(pBuf + offset, &vec.y, sizeof(float));
    offset += sizeof(float);
    memcpy(pBuf + offset, &vec.z, sizeof(float));
    offset += sizeof(float);
}

void RTD(char* pBuf, const Angle& vec, int& offset) {
    memcpy(pBuf + offset, &vec.pitch, sizeof(float));
    offset += sizeof(float);
    memcpy(pBuf + offset, &vec.yaw, sizeof(float));
    offset += sizeof(float);
    memcpy(pBuf + offset, &vec.roll, sizeof(float));
    offset += sizeof(float);
}

void F(char* pBuf, float f, int& offset) {
    memcpy(pBuf + offset, &f, sizeof(float));
    offset += sizeof(float);
}

void I(char* pBuf, int i, int& offset) {
    memcpy(pBuf + offset, &i, sizeof(int));
    offset += sizeof(int);
}

void B(char* pBuf, bool b, int& offset) {
    memcpy(pBuf + offset, &b, sizeof(bool));
    offset += sizeof(bool);
}

PythonInterface::PythonInterface(int matchId, int totalTicks)
{
    this->totalTicks = totalTicks;
    shmName = std::format("Local\\SQRL_Python_{}", matchId); 
    std::cout << std::format("Initializing SharedMemory module {} ...", shmName) << std::endl;
    shm = new SharedMemory(shmName, size);
    std::cout << "Successfully initialized the SharedMemory module" << std::endl;
    blueBot = new PythonBot();
    orangeBot = new PythonBot();
}

PythonInterface::~PythonInterface() {
}

void PythonInterface::SendGameState(Arena* arena, BallPredTracker* ballPred, bool isKickoff, bool isOvertime, int blueScore, int orangeScore) {
    char* pBuf = shm->GetPBuf();
    int offset = 0;
    // Cars
    Car* blueCar = blueBot->car;
    Car* orangeCar = orangeBot->car;
    CarState blueState = blueCar->GetState();
    CarState orangeState = orangeCar->GetState();
    VTD(pBuf, blueState.pos, offset);
    RTD(pBuf, Angle::FromRotMat(blueState.rotMat), offset);
    VTD(pBuf, blueState.vel, offset);
    VTD(pBuf, blueState.angVel, offset);
    VTD(pBuf, blueCar->config.hitboxSize, offset);
    VTD(pBuf, blueCar->config.hitboxPosOffset, offset);
    F(pBuf, blueState.boost, offset);
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, blueCar->team == Team::BLUE ? 0 : 1, offset);
    B(pBuf, blueState.isDemoed, offset);
    B(pBuf, blueState.isOnGround, offset);
    B(pBuf, blueState.isSupersonic, offset);
    B(pBuf, blueState.hasJumped, offset);
    B(pBuf, blueState.hasDoubleJumped, offset);
    VTD(pBuf, orangeState.pos, offset);
    RTD(pBuf, Angle::FromRotMat(orangeState.rotMat), offset);
    VTD(pBuf, orangeState.vel, offset);
    VTD(pBuf, orangeState.angVel, offset);
    VTD(pBuf, orangeCar->config.hitboxSize, offset);
    VTD(pBuf, orangeCar->config.hitboxPosOffset, offset);
    F(pBuf, orangeState.boost, offset);
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, -1, offset); //score info
    I(pBuf, orangeCar->team == Team::BLUE ? 0 : 1, offset);
    B(pBuf, orangeState.isDemoed, offset);
    B(pBuf, orangeState.isOnGround, offset);
    B(pBuf, orangeState.isSupersonic, offset);
    B(pBuf, orangeState.hasJumped, offset);
    B(pBuf, orangeState.hasDoubleJumped, offset);
    // Boosts
    for (int i = 0; i < arena->GetBoostPads().size(); i++) {
        B(pBuf, arena->GetBoostPads().at(i)->GetState().isActive, offset);
        F(pBuf, arena->GetBoostPads().at(i)->GetState().cooldown, offset);
    }
    // Ball
    BallState ballState = arena->ball->GetState();
    VTD(pBuf, ballState.pos, offset);
    VTD(pBuf, ballState.vel, offset);
    VTD(pBuf, ballState.angVel, offset);
    VTD(pBuf, Vec(0, 0, 0), offset); //hit pos
    VTD(pBuf, Vec(0, 0, 0), offset); //hit normal
    F(pBuf, -1, offset); //hit time
    I(pBuf, -1, offset); //hit team
    I(pBuf, -1, offset); //hit index
    // Game
    int tickCount = arena->tickCount;
    float time = tickCount / 120.0f;
    F(pBuf, time, offset);
    F(pBuf, totalTicks / 120.0f - time, offset);
    B(pBuf, isOvertime, offset);
    B(pBuf, isKickoff, offset);
    B(pBuf, isOvertime ? blueScore != orangeScore : totalTicks < tickCount && ballState.pos.z < 95, offset);
    I(pBuf, tickCount, offset);
    //teaminfo
    // Ball Prediction
    for (size_t i = 2; i < 722; i += 2) {
        VTD(pBuf, ballPred->predData[i].pos, offset);
        VTD(pBuf, ballPred->predData[i].vel, offset);
        VTD(pBuf, ballPred->predData[i].angVel, offset);
        F(pBuf, time + i / 120.0f, offset);
    }
    int sz = 14883;
    I(pBuf, 360, sz);
    sz = 14945;
    I(pBuf, 2, sz); // make dynamic
}

void PythonInterface::SetControllers() {
    blueBot->car->controls = blueControls;
    orangeBot->car->controls = orangeControls;

}

void PythonInterface::GetControllers() {
    char* pBuf = shm->GetPBuf();

    memcpy(&blueControls.throttle, pBuf + 14895, sizeof(float));
    memcpy(&blueControls.steer, pBuf + 14899, sizeof(float));
    memcpy(&blueControls.pitch, pBuf + 14903, sizeof(float));
    memcpy(&blueControls.yaw, pBuf + 14907, sizeof(float));
    memcpy(&blueControls.roll, pBuf + 14911, sizeof(float));
    memcpy(&blueControls.jump, pBuf + 14915, sizeof(bool));
    memcpy(&blueControls.boost, pBuf + 14916, sizeof(bool));
    memcpy(&blueControls.handbrake, pBuf + 14917, sizeof(bool));

    memcpy(&orangeControls.throttle, pBuf + 14922, sizeof(float));
    memcpy(&orangeControls.steer, pBuf + 14926, sizeof(float));
    memcpy(&orangeControls.pitch, pBuf + 14930, sizeof(float));
    memcpy(&orangeControls.yaw, pBuf + 14934, sizeof(float));
    memcpy(&orangeControls.roll, pBuf + 14938, sizeof(float));
    memcpy(&orangeControls.jump, pBuf + 14942, sizeof(bool));
    memcpy(&orangeControls.boost, pBuf + 14943, sizeof(bool));
    memcpy(&orangeControls.handbrake, pBuf + 14944, sizeof(bool));
}
