#pragma once
#include "Camera.h"
#include "Window.h"
#include "World.h"

struct AppContext {
	Window* window;
	Camera* camera;
	World* world;
};