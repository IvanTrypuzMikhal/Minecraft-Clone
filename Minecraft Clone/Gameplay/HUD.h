#pragma once
#include "UIRenderer.h"
#include "HUDSprites.h"

class HUD
{
public:
	HUD(UIRenderer& renderer, Window& window) : m_renderer{ renderer }, m_window{ window } {};
	~HUD() = default;

	void render();

private:
	UIRenderer& m_renderer;
	Window& m_window;

	void drawCrosshair();
	void drawHotbar();
	void drawXPBar();
	void drawHearts();
	void drawFood();
};
