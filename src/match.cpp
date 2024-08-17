#include "..\include\match.h"

Match::Match(Bot* blueBot, Bot* orangeBot, bool render) {
	this->blueBot = blueBot;
	this->orangeBot = orangeBot;
	this->render = render;
	blueScore = 0;
	orangeScore = 0;

	if (render) {
		visualizer = std::make_unique<Visualizer>();
	}

	arena = Arena::Create(GameMode::SOCCAR);
	this->blueBot->car = arena->AddCar(Team::BLUE);
	this->orangeBot->car = arena->AddCar(Team::ORANGE);
	arena->ResetToRandomKickoff();
}

Match::~Match() {
	delete arena;
}

void Match::Run(int ticks) {
	for (int i = 0; i < ticks; i++)
	{
		if (arena->IsBallScored()) {
			if (arena->ball->GetState().pos.y > 0)
				blueScore += 1;
			else
				orangeScore += 1;
			arena->ResetToRandomKickoff();
		}

		blueBot->SetController(arena);
		orangeBot->SetController(arena);

		arena->Step();
		if (render) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000/120));
			json gameState = Visualizer::MakeGameState(arena, blueBot, orangeBot);
			visualizer->SendGameState(gameState);
		}

	}
}


