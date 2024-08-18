#pragma once

#include "../RocketSim/src/Sim/Arena/Arena.h"
#include "python_bot.h"
#include "render.h"

using namespace RocketSim;

class Match {
public:
	bool render;
	int ticks;
	int blueScore;
	int orangeScore;
	float _kickoffTime;
	bool isKickoff;

	std::unique_ptr<Visualizer> visualizer;

	Arena* arena;
	PythonBot* blueBot;
	PythonBot* orangeBot;

	Match(PythonBot* blueBot, PythonBot* orangeBot, int ticks, bool render = false);
	~Match();

	void Run();

};
