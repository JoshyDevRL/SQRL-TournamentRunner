#include "../include/render.h"

Visualizer::Visualizer() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        throw std::runtime_error("WSAStartup failed");
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        throw std::runtime_error("Socket creation failed");
    }
}

Visualizer::~Visualizer() {
    closesocket(sock);
    WSACleanup();
}

void Visualizer::SendGameState(const json& gameState) {
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(HOST.c_str());

    std::string message = gameState.dump();

    if (sendto(sock, message.c_str(), message.size(), 0, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Send failed" << std::endl;
    }
}

json Visualizer::MakeGameState(Arena* arena, PythonBot* blueBot, PythonBot* orangeBot)
{
    const BallState& ballState = arena->ball->GetState();
    Vec ballPos = ballState.pos;
    Vec ballVel = ballState.vel;
    Vec ballAngVel = ballState.angVel;

    Car* blueCar = blueBot->car;
    const CarState& blueCarState = blueCar->GetState();
    Vec blueCarPos = blueCarState.pos;
    Vec blueCarForward = blueCarState.rotMat.forward;
    Vec blueCarRight = blueCarState.rotMat.right;
    Vec blueCarUp = blueCarState.rotMat.up;
    Vec blueCarVel = blueCarState.vel;
    Vec blueCarAngVel = blueCarState.angVel;

    Car* orangeCar = orangeBot->car;
    const CarState& orangeCarState = orangeCar->GetState();
    Vec orangeCarPos = orangeCarState.pos;
    Vec orangeCarForward = orangeCarState.rotMat.forward;
    Vec orangeCarRight = orangeCarState.rotMat.right;
    Vec orangeCarUp = orangeCarState.rotMat.up;
    Vec orangeCarVel = orangeCarState.vel;
    Vec orangeCarAngVel = orangeCarState.angVel;

    std::array<bool, 34> boosts;
    for (int i = 0; i < arena->GetBoostPads().size(); i++) {
        boosts[i] = arena->GetBoostPads()[i]->GetState().isActive;
    }

    json gameState = {
        {"ball_phys", {
            {"pos", {ballPos.x, ballPos.y, ballPos.z}},
            {"vel", {ballVel.x, ballVel.y, ballVel.z}},
            {"ang_vel", {ballAngVel.x, ballAngVel.y, ballAngVel.z}} }
        },
        {"cars", {
            {
                {"car_id", blueCar->id},
                {"team_num", blueCar->team},
                {"phys", {
                    {"pos", {blueCarPos.x, blueCarPos.y, blueCarPos.z}},
                    {"forward", {blueCarForward.x, blueCarForward.y, blueCarForward.z}},
                    {"right", {blueCarRight.x, blueCarRight.y, blueCarRight.z}},
                    {"up", {blueCarUp.x, blueCarUp.y, blueCarUp.z}},
                    {"vel", {blueCarVel.x, blueCarVel.y, blueCarVel.z}},
                    {"ang_vel", {blueCarAngVel.x, blueCarAngVel.y, blueCarAngVel.z}} }
                },
                {"controls", {
                    {"throttle", blueCar->controls.throttle},
                    {"steer", blueCar->controls.steer},
                    {"pitch", blueCar->controls.pitch},
                    {"yaw", blueCar->controls.yaw},
                    {"roll", blueCar->controls.roll},
                    {"boost", blueCar->controls.boost},
                    {"jump", blueCar->controls.jump},
                    {"handbrake", blueCar->controls.handbrake} }
                },
                {"boost_amount", blueCarState.boost},
                {"on_ground", blueCarState.isOnGround},
                {"has_flip", !blueCarState.hasDoubleJumped && !blueCarState.hasFlipped && !blueCarState.airTimeSinceJump < RLConst::DOUBLEJUMP_MAX_DELAY},
                {"is_demoed", blueCarState.isDemoed}
            },
            {
                {"car_id", orangeCar->id},
                {"team_num", orangeCar->team},
                {"phys", {
                    {"pos", {orangeCarPos.x, orangeCarPos.y, orangeCarPos.z}},
                    {"forward", {orangeCarForward.x, orangeCarForward.y, orangeCarForward.z}},
                    {"right", {orangeCarRight.x, orangeCarRight.y, orangeCarRight.z}},
                    {"up", {orangeCarUp.x, orangeCarUp.y, orangeCarUp.z}},
                    {"vel", {orangeCarVel.x, orangeCarVel.y, orangeCarVel.z}},
                    {"ang_vel", {orangeCarAngVel.x, orangeCarAngVel.y, orangeCarAngVel.z}} }
                },
                {"controls", {
                    {"throttle", orangeCar->controls.throttle},
                    {"steer", orangeCar->controls.steer},
                    {"pitch", orangeCar->controls.pitch},
                    {"yaw", orangeCar->controls.yaw},
                    {"roll", orangeCar->controls.roll},
                    {"boost", orangeCar->controls.boost},
                    {"jump", orangeCar->controls.jump},
                    {"handbrake", orangeCar->controls.handbrake} }
                },
                {"boost_amount", orangeCarState.boost},
                {"on_ground", orangeCarState.isOnGround},
                {"has_flip", !orangeCarState.hasDoubleJumped && !orangeCarState.hasFlipped && !orangeCarState.airTimeSinceJump < RLConst::DOUBLEJUMP_MAX_DELAY},
                {"is_demoed", orangeCarState.isDemoed}
            }
            },
        },
        {"boost_pad_states", { boosts[0], boosts[1], boosts[2], boosts[3], boosts[4], boosts[5], boosts[6], boosts[7], boosts[8], boosts[9], boosts[10], boosts[11], boosts[12],
        boosts[13], boosts[14], boosts[15], boosts[16], boosts[17], boosts[18], boosts[19], boosts[20], boosts[21], boosts[22], boosts[23], boosts[24], boosts[25], boosts[26],
        boosts[27], boosts[28], boosts[29], boosts[30], boosts[31],  boosts[32], boosts[33]} }
    };
    return gameState;
}
