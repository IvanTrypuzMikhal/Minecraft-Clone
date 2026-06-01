#pragma once
#include "pch.h" 


class CubeSelection
{
public:
	CubeSelection();
	~CubeSelection() = default;

	void renderOutline(int x, int y, int z, const glm::mat4& projection, const glm::mat4& view) const;

private:
	ShaderProgram m_shaderProgram;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
};
