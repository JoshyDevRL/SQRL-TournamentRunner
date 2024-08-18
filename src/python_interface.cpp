#include "../include/python_interface.h"

PythonInterface::PythonInterface(const std::string& pipeName) : pipeName(pipeName) {
    hPipe = CreateNamedPipeA(
        pipeName.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, 1024, 1024, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateNamedPipe failed. GetLastError = " << GetLastError() << std::endl;
    }
}

PythonInterface::~PythonInterface() {
    if (hPipe != INVALID_HANDLE_VALUE) {
        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
    }
}

bool PythonInterface::WaitForClient() {
    BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
    if (!connected) {
        std::cerr << "ConnectNamedPipe failed. GetLastError = " << GetLastError() << std::endl;
        return false;
    }
    return true;
}

bool PythonInterface::Write(const std::string& state) {
    DWORD bytesWritten;
    BOOL success = WriteFile(hPipe, state.c_str(), state.size() + 1, &bytesWritten, NULL);
    if (!success || bytesWritten != state.size() + 1) {
        std::cerr << "WriteFile failed. GetLastError = " << GetLastError() << std::endl;
        return false;
    }
    return true;
}

bool PythonInterface::Read(std::string& action, CarControls* controller) {
    const int totalBytes = 23; // 5 floats (20 bytes) + 3 booleans (3 bytes)
    char buffer[totalBytes];
    DWORD bytesRead;

    // Read data from the pipe
    BOOL success = ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL);
    if (!success || bytesRead != sizeof(buffer)) {
        std::cerr << "ReadFile failed or read unexpected amount of data. GetLastError = " << GetLastError() << std::endl;
        return false;
    }
    size_t offset = 0;
    float floats[5];
    std::memcpy(floats, buffer + offset, 5 * sizeof(float));
    offset += 5 * sizeof(float);

    bool bools[3];
    std::memcpy(bools, buffer + offset, 3 * sizeof(bool));
    offset += 3 * sizeof(bool);

    controller->throttle = floats[0];
    controller->steer = floats[1];
    controller->pitch = floats[2];
    controller->yaw = floats[3];
    controller->roll = floats[4];

    controller->jump = bools[0];
    controller->boost = bools[1];
    controller->handbrake = bools[2];
    return true;
}

std::string PythonInterface::SerializeGamestate(Arena* arena, Car* blueCar, Car* orangeCar, int totalTicks, bool isKickoff) {
    std::string binaryData;
    auto appendData = [&](const void* data, size_t size) {
        binaryData.append(reinterpret_cast<const char*>(data), size);
    };
    // Blue Car
    CarState blueState = blueCar->GetState();
    Vec blueCarPos = blueState.pos;
    appendData(&blueCarPos.x, sizeof(float));
    appendData(&blueCarPos.y, sizeof(float));
    appendData(&blueCarPos.z, sizeof(float));
    Angle bRot = Angle::FromRotMat(blueState.rotMat);
    appendData(&bRot.pitch, sizeof(float));
    appendData(&bRot.yaw, sizeof(float));
    appendData(&bRot.roll, sizeof(float));
    Vec blueCarVel = blueState.vel;
    appendData(&blueCarVel.x, sizeof(float));
    appendData(&blueCarVel.y, sizeof(float));
    appendData(&blueCarVel.z, sizeof(float));
    Vec blueCarAngVel = blueState.angVel;
    appendData(&blueCarAngVel.x, sizeof(float));
    appendData(&blueCarAngVel.y, sizeof(float));
    appendData(&blueCarAngVel.z, sizeof(float));
    appendData(&blueState.boost, sizeof(float));
    int bteam = blueCar->team == Team::BLUE ? 0 : 1;
    appendData(&bteam, sizeof(int));
    appendData(&blueState.isDemoed, sizeof(bool));
    appendData(&blueState.isOnGround, sizeof(bool));
    appendData(&blueState.isSupersonic, sizeof(bool));
    appendData(&blueState.hasJumped, sizeof(bool));
    appendData(&blueState.hasDoubleJumped, sizeof(bool));
    // Orange Car
    CarState orangeState = orangeCar->GetState();
    Vec orangeCarPos = orangeState.pos;
    appendData(&orangeCarPos.x, sizeof(float));
    appendData(&orangeCarPos.y, sizeof(float));
    appendData(&orangeCarPos.z, sizeof(float));
    Angle oRot = Angle::FromRotMat(orangeState.rotMat);
    appendData(&oRot.pitch, sizeof(float));
    appendData(&oRot.yaw, sizeof(float));
    appendData(&oRot.roll, sizeof(float));
    Vec orangeCarVel = orangeState.vel;
    appendData(&orangeCarVel.x, sizeof(float));
    appendData(&orangeCarVel.y, sizeof(float));
    appendData(&orangeCarVel.z, sizeof(float));
    Vec orangeCarAngVel = orangeState.angVel;
    appendData(&orangeCarAngVel.x, sizeof(float));
    appendData(&orangeCarAngVel.y, sizeof(float));
    appendData(&orangeCarAngVel.z, sizeof(float));
    appendData(&orangeState.boost, sizeof(float));
    int oteam = orangeCar->team == Team::BLUE ? 0 : 1;
    appendData(&oteam, sizeof(int));
    appendData(&orangeState.isDemoed, sizeof(bool));
    appendData(&orangeState.isOnGround, sizeof(bool));
    appendData(&orangeState.isSupersonic, sizeof(bool));
    appendData(&orangeState.hasJumped, sizeof(bool));
    appendData(&orangeState.hasDoubleJumped, sizeof(bool));
    // Boost Pads
    for (const BoostPad* pad : arena->GetBoostPads()) {
        bool active = pad->GetState().isActive;
        appendData(&active, sizeof(bool));
        float timer = pad->GetState().cooldown;
        appendData(&timer, sizeof(float));
    }
    // Ball
    BallState ballState = arena->ball->GetState();
    Vec ballPos = ballState.pos;
    appendData(&ballPos.x, sizeof(float));
    appendData(&ballPos.y, sizeof(float));
    appendData(&ballPos.z, sizeof(float));
    Vec ballVel = ballState.vel;
    appendData(&ballVel.x, sizeof(float));
    appendData(&ballVel.y, sizeof(float));
    appendData(&ballVel.z, sizeof(float));
    Vec ballAngVel = ballState.angVel;
    appendData(&ballAngVel.x, sizeof(float));
    appendData(&ballAngVel.y, sizeof(float));
    appendData(&ballAngVel.z, sizeof(float));
    // Game
    int tick = arena->tickCount;
    float time = tick / 120.0f;
    appendData(&time, sizeof(float));
    float timeRemaining = totalTicks / 120.0f - time;
    appendData(&timeRemaining, sizeof(float));
    bool overtime = totalTicks < tick;
    appendData(&overtime, sizeof(bool));
    appendData(&isKickoff, sizeof(bool));
    appendData(&tick, sizeof(int));
    return binaryData;
}
