#include "../include/bot.h"

Bot::Bot() {

}

void Bot::SetController(Arena* arena) {
	CarControls controller = CarControls();
	controller.throttle = 1;
	controller.boost = true;
	Vec ballLocal = car->GetState().rotMat.Dot(arena->ball->GetState().pos - car->GetState().pos);
	controller.steer = atan2(ballLocal.y, ballLocal.x);
	car->controls = controller;
}
