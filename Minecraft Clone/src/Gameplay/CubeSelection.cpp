#include "CubeSelection.h"

CubeSelection::CubeSelection()
	: m_shaderProgram("src/Rendering/Shaders/cubeSelection.vert", "src/Rendering/Shaders/cubeSelection.frag")
{
	m_vbo = std::make_unique<Vbo>(CubeData::wireframeVertices);
	std::vector<VertexAttribute> attributes = { { .index = 0, .size = 3, .offset = 0 } };
	m_vao = std::make_unique<Vao>(m_vbo->get(), 3, attributes);
}

void CubeSelection::renderOutline(int x, int y, int z, const glm::mat4& projection, const glm::mat4& view) const {
	m_shaderProgram.use();
	m_vao->use();
	m_shaderProgram.setMat4("projection", projection);
	m_shaderProgram.setMat4("view", view);
	glm::vec3 cubePos(
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(z)
	);
	m_shaderProgram.setVec3("cubePos", cubePos);
	m_vao->use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f); 

	glDrawArrays(GL_LINES, 0, 24);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}