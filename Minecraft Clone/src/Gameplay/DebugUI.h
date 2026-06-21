#pragma once
#include <Rendering/Text.h>
#include "Camera.h"
#include <Core/Window.h>
#include <Rendering/ShaderProgram.h>
#include <Core/Time.h>
#include "Raycaster.h"
#include <format>

class DebugUI
{
public:
	DebugUI() : m_shader{ ShaderProgram("src/Rendering/Shaders/text.vert", "src/Rendering/Shaders/text.frag") }, m_textRenderer{ Text("src/Assets/TextFont/1_Minecraft-Regular.otf", 30, m_shader) } {}
	~DebugUI() = default;

	void renderText(const std::unique_ptr<World>& world, const Camera* cam, const std::unique_ptr<Window>& window, const Time& time);

private:
	Text m_textRenderer;
	ShaderProgram m_shader;
};
