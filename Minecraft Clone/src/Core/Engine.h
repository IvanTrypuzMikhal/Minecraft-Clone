#pragma once
#include "pch.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void run();
	void update();
	void tick();
	void render();
	void processInput();
	void stateUpdate();
	void initAssets();

private:

	// Window
	std::unique_ptr<Window> m_window;
	AppContext m_context;
	Time m_time;

	// World
	std::unique_ptr<World> m_world;
	std::unique_ptr<SkyBox> m_skybox;

	// Player
	std::unique_ptr<Player> m_player;

	// UI's / Visuals
	std::unique_ptr<CubeSelection> m_cubeSelection;
	std::unique_ptr<UIRenderer> m_uiRenderer;
	std::unique_ptr<HUD> m_hud;

	bool m_escapePressed = false;

	bool m_tabPressed = false;
	bool m_lastTabState = false;
};
