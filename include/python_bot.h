#pragma once

#include "RocketSim.h"
#include "utils.h"
#include <Sim/BallPredTracker/BallPredTracker.h>
#include "event_tracker.h"

using namespace RocketSim;

class PythonBot {
public:
	Car* car;
	std::string Name;
	InfoTracker* tracker;

	PythonBot();

	void Initialize(std::string name);

};