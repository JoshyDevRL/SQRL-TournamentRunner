#include "../include/python_bot.h"

PythonBot::PythonBot(std::string name) {
	Name = name;
}

void PythonBot::Initialize(int totalTicks) {
	this->totalTicks = totalTicks;
	std::string pipeName = "\\\\.\\pipe\\PythonInterFace-" + Name + "_" + std::to_string(car->id - 1);
	pythonInterface = new PythonInterface(pipeName);
	std::cout << "[SQRL] Initialized Pipe with name:" << pipeName << std::endl;

	std::cout << "[SQRL] " << Name << std::to_string(car->id - 1) << "is waiting for a connection..." << std::endl;
	if (!pythonInterface->WaitForClient()) {
	}
	std::cout << "[SQRL] " << Name << std::to_string(car->id - 1) << "has established a connection..." << std::endl;
}

void PythonBot::SetController(Arena* arena, Car* opp, bool isKickoff) {
	std::string serializedGamestate = PythonInterface::SerializeGamestate(arena, car->team == Team::BLUE ? car : opp, car->team == Team::BLUE ? opp : car, totalTicks, isKickoff);
	pythonInterface->Write(serializedGamestate);

	std::string action;
	pythonInterface->Read(action, &(car->controls));
}
