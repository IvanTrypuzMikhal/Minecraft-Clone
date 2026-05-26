#pragma once
#include <glm/glm.hpp>
#include <vector>

class Cube
{
public:
	Cube(unsigned int textureId, unsigned int shaderId, glm::vec3 position) : 
		m_textureId{ textureId }, m_shaderId{ shaderId }, m_position{ position } {}

	~Cube() = default;

	void createCube() const;
	void createVAO();

private:
	unsigned int m_textureId;
	unsigned int m_shaderId;
	glm::vec3 m_position;


	std::vector<VertexAttribute> m_vertexAttributes{ { 0, 3, 0 }, { 1, 2, 3 } };
	unsigned int m_VAO;
	unsigned int m_VBO;
};
