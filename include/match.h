#pragma once

#include "../RocketSim/src/Sim/Arena/Arena.h"
#include "event_tracker.h"
#include "python_interface.h"
#include "render.h"

namespace chr = std::chrono;
using namespace RocketSim;

class Match {
public:
	int matchId;
	bool render;
	int ticks;
	int blueScore;
	int orangeScore;
	float _kickoffTime;
	bool isKickoff;
	bool isOvertime;
	bool updatePred;
	int bTick;
	int oTick;
	float timeInOvertime;

	std::unique_ptr<Visualizer> visualizer;

	Arena* arena;
	PythonInterface* pythonInterface;
	BallPredTracker* ballPredTracker;
	GameEventTracker eventTracker;

	Match(int matchId, std::vector<std::string> names, int ticks, bool render = false);
	~Match();

	void Run();

};
