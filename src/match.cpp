#include "..\include\match.h"

void IncShot(Car* car, void* userInfo) {
	if (!car)
		return;

	Match* match = (Match*)userInfo;
	if (match->pythonInterface->blueBot->car->id == car->id) {
		match->pythonInterface->blueBot->tracker->matchShots++;
	}
	if (match->pythonInterface->orangeBot->car->id == car->id) {
		match->pythonInterface->orangeBot->tracker->matchShots++;
	}
}

void IncGoal(Car* car, void* userInfo) {
	if (!car)
		return;

	Match* match = (Match*)userInfo;
	if (match->pythonInterface->blueBot->car->id == car->id) {
		match->pythonInterface->blueBot->tracker->matchGoals++;
	}
	if (match->pythonInterface->orangeBot->car->id == car->id) {
		match->pythonInterface->orangeBot->tracker->matchGoals++;
	}
}

void IncSave(Car* car, void* userInfo) {
	if (!car)
		return;

	Match* match = (Match*)userInfo;
	if (match->pythonInterface->blueBot->car->id == car->id) {
		match->pythonInterface->blueBot->tracker->matchSaves++;
	}
	if (match->pythonInterface->orangeBot->car->id == car->id) {
		match->pythonInterface->orangeBot->tracker->matchSaves++;
	}
}

void IncAssist(Car* car, void* userInfo) {
	if (!car)
		return;

	Match* match = (Match*)userInfo;
	if (match->pythonInterface->blueBot->car->id == car->id) {
		match->pythonInterface->blueBot->tracker->matchAssists++;
	}
	if (match->pythonInterface->orangeBot->car->id == car->id) {
		match->pythonInterface->orangeBot->tracker->matchAssists++;
	}
}

void _ShotEventCallback(Arena* arena, Car* shooter, Car* passer, void* userInfo) {
	IncShot(shooter, userInfo);
	//IncPlayerCounter(passer, userInfo);
}

void _GoalEventCallback(Arena* arena, Car* scorer, Car* passer, void* userInfo) {
	IncGoal(scorer, userInfo);
	IncAssist(passer, userInfo);
}

void _SaveEventCallback(Arena* arena, Car* saver, void* userInfo) {
	IncSave(saver, userInfo);
}


Match::Match(int matchId, std::vector<std::string> names, int ticks, bool render) {
	this->matchId = matchId;
	this->ticks = ticks;
	this->render = render;
	pythonInterface = new PythonInterface(matchId, ticks);
	blueScore = 0;
	orangeScore = 0;
	_kickoffTime = 0.0f;
	bTick = -1;
	oTick = -1;
	isKickoff = true;
	isOvertime = false;
	updatePred = false;

	if (render) {
		visualizer = std::make_unique<Visualizer>();
	}

	arena = Arena::Create(GameMode::SOCCAR);
	ballPredTracker = new BallPredTracker(arena, 722);
	this->pythonInterface->blueBot->car = arena->AddCar(Team::BLUE);
	this->pythonInterface->orangeBot->car = arena->AddCar(Team::ORANGE);
	this->pythonInterface->blueBot->Name = names[0];
	this->pythonInterface->orangeBot->Name = names[1];
	arena->ResetToRandomKickoff();

	eventTracker.SetShotCallback(_ShotEventCallback, this);
	eventTracker.SetGoalCallback(_GoalEventCallback, this);
	eventTracker.SetSaveCallback(_SaveEventCallback, this);
}

Match::~Match() {
	delete arena;
}

void Match::Run() {
	while (ticks >= arena->tickCount || blueScore == orangeScore || arena->ball->GetState().pos.z > 95) {
		if (updatePred) {
			ballPredTracker->UpdatePredFromArena(arena);
			eventTracker.Update(arena);
		}
		updatePred = !updatePred;
		if (ticks < arena->tickCount && blueScore == orangeScore && arena->ball->GetState().pos.z < 95 && !isOvertime) {
			arena->ResetToRandomKickoff();
			isOvertime = true;
		}
		if (arena->IsBallScored()) {
			if (arena->ball->GetState().pos.y > 0) {
				blueScore += 1;
			}
			else {
				orangeScore += 1;
			}
			std::cout << arena->tickCount / 120.0f << std::endl;
			arena->ResetToRandomKickoff();
			_kickoffTime = 0.0f;
			isKickoff = true;
		}
		if (_kickoffTime > 5 || abs(arena->ball->GetState().pos.x) > 1 || abs(arena->ball->GetState().pos.y) > 1) {
			isKickoff = false;
		}
		if (isKickoff) {
			_kickoffTime += 1 / 120.0f;
		}
		pythonInterface->SetControllers();
		pythonInterface->SendGameState(arena, ballPredTracker, isKickoff, isOvertime, blueScore, orangeScore);
		char* pBuf = pythonInterface->shm->GetPBuf();
		int tickCount = arena->tickCount;
		while (tickCount != bTick || tickCount != oTick) {
			memcpy(&bTick, pBuf + 14891, sizeof(int));
			memcpy(&oTick, pBuf + 14918, sizeof(int));
		}
		pythonInterface->GetControllers();

		arena->Step();
		if (render) {
			std::this_thread::sleep_for(std::chrono::milliseconds(300/120));
			json gameState = Visualizer::MakeGameState(arena, pythonInterface->blueBot, pythonInterface->orangeBot);
			visualizer->SendGameState(gameState);
		}

	}
}


