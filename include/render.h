#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "../JSON/json.hpp"
#include "../RocketSim/src/Sim/Arena/Arena.h"
#include "python_bot.h"

#pragma comment(lib, "ws2_32.lib")

using json = nlohmann::json;
using namespace RocketSim;

class Visualizer {
public:
	const int PORT = 9273;
	const std::string HOST = "127.0.0.1";

	WSADATA wsaData;
	SOCKET sock;

	Visualizer();
	~Visualizer();

	void SendGameState(const json& gameState);

	static json MakeGameState(Arena* arena, PythonBot* blueBot, PythonBot* orangeBot);

};