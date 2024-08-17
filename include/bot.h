#pragma once

#include "RocketSim.h"

using namespace RocketSim;

class Bot {
public:
	Car* car;

	Bot();
		
	void SetController(Arena* arena);

};