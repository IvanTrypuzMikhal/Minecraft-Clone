#pragma once
#include "UIRenderer.h"
#include <Rendering/Vao.h>
#include <Rendering/Vbo.h>
#include <Rendering/Texture.h>
#include <Rendering/ShaderProgram.h>
#include <Core/Window.h>

class UIRenderer
{
public:
	UIRenderer();
	~UIRenderer() = default;

	void begin();
	void drawQuad(float x, float y, float w, float h, float u0, float v0, float u1, float v1);
	void flush(const Window* window);
	void fushInverted(const Window* window);

private:
	std::vector<float> m_batch;
	
	ShaderProgram m_shaderProgram;
	Texture m_texture;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
};
