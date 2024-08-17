#pragma once

#include "../RocketSim/src/Sim/Arena/Arena.h"
#include "bot.h"
#include "render.h"

using namespace RocketSim;

class Match {
public:
	bool render;
	int blueScore;
	int orangeScore;

	std::unique_ptr<Visualizer> visualizer;

	Arena* arena;
	Bot* blueBot;
	Bot* orangeBot;

	Match(Bot* blueBot, Bot* orangeBot, bool render = false);
	~Match();

	void Run(int ticks);

};
