#include "../include/python_bot.h"

PythonBot::PythonBot() {
	tracker = new InfoTracker();
}

void PythonBot::Initialize(std::string name) {
	Name = name;
}