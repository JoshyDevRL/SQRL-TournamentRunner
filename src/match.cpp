#include "..\include\match.h"

Match::Match(PythonBot* blueBot, PythonBot* orangeBot, int ticks, bool render) {
	this->blueBot = blueBot;
	this->orangeBot = orangeBot;
	this->render = render;
	this->ticks = ticks;
	blueScore = 0;
	orangeScore = 0;
	_kickoffTime = 0.0f;
	isKickoff = false;

	if (render) {
		visualizer = std::make_unique<Visualizer>();
	}

	arena = Arena::Create(GameMode::SOCCAR);
	this->blueBot->car = arena->AddCar(Team::BLUE);
	this->orangeBot->car = arena->AddCar(Team::ORANGE);
	this->blueBot->Initialize(ticks);
	this->orangeBot->Initialize(ticks);
	arena->ResetToRandomKickoff();
}

Match::~Match() {
	delete arena;
}

void Match::Run() {
	for (int i = 0; i < ticks; i++)
	{
		if (arena->IsBallScored()) {
			if (arena->ball->GetState().pos.y > 0)
				blueScore += 1;
			else
				orangeScore += 1;
			arena->ResetToRandomKickoff();
			_kickoffTime = 0.0f;
			isKickoff = true;
		}
		if (_kickoffTime > 5 || abs(arena->ball->GetState().pos.x) > 1 || abs(arena->ball->GetState().pos.y) > 1)
			isKickoff = false;
		
		if (isKickoff)
			_kickoffTime += 1 / 120.0f;

		blueBot->SetController(arena, orangeBot->car, isKickoff);
		orangeBot->SetController(arena, blueBot->car, isKickoff);

		arena->Step();
		if (render) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000/120));
			json gameState = Visualizer::MakeGameState(arena, blueBot, orangeBot);
			visualizer->SendGameState(gameState);
		}

	}
}


