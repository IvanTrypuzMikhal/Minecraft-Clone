#pragma once
#include <Gameplay/Camera.h>
#include "Window.h"
#include <World/World.h>

struct AppContext {
	Window* window;
	Camera* camera;
	World* world;
};