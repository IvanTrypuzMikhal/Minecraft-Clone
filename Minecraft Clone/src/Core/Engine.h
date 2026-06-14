#pragma once
#include "pch.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void run();
	void update();
	void render();
	void processInput();

private:

	// Window
	std::unique_ptr<Window> m_window;
	AppContext m_context;
	Time m_time;

	// World
	std::unique_ptr<World> m_world;

	// Player
	std::unique_ptr<Player> m_player;

	// UI's / Visuals
	std::unique_ptr<CubeSelection> m_cubeSelection;
	std::unique_ptr<UIRenderer> m_uiRenderer;
	std::unique_ptr<HUD> m_hud;
};
