#pragma once

#include "RocketSim.h"
#include "python_interface.h"
#include "utils.h"

using namespace RocketSim;

class PythonBot {
public:
	Car* car;
	PythonInterface* pythonInterface;
	std::string Name;
	int totalTicks;

	PythonBot(std::string name);
		
	void Initialize(int totalTicks);
	void SetController(Arena* arena, Car* opp, bool isKickoff);

};