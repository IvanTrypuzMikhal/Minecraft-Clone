#pragma once
#include <Gameplay/Player/Player.h>
#include "Window.h"
#include <World/World.h>

struct AppContext {
	Window* window;
	Player* player;
	Camera* camera;
	World* world;
};