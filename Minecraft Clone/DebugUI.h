#pragma once
#include "Text.h"
#include "Camera.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Time.h"
#include <format>

class DebugUI
{
public:
	DebugUI() : m_shader{ ShaderProgram("text.vert", "text.frag") }, m_textRenderer{ Text("1_Minecraft-Regular.otf", 30, m_shader) } {}
	~DebugUI() = default;

	void renderText(const Camera& cam, const Window& window, const Time& time);

private:
	Text m_textRenderer;
	ShaderProgram m_shader;
};
