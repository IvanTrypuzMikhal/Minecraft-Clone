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
	Window m_window;
	AppContext m_context;
	Time m_time;

	// Shader Programs
	std::shared_ptr<ShaderProgram> m_worldShaderProgram;
	std::shared_ptr<ShaderProgram> m_textShaderProgram;

	// Textures
	std::shared_ptr<Texture> m_texture;

	// World
	std::unique_ptr<World> m_world;

	// Player
	std::unique_ptr<Player> m_player;

	// UI's / Visuals
	CubeSelection m_cubeSelection;
	UIRenderer m_uiRenderer;
	HUD m_hud;
};
