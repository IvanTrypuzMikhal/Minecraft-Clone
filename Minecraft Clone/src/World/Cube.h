#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Rendering/ShaderProgram.h>
#include <Rendering/Texture.h>

class Cube
{
public:
	Cube(const Texture* texture,const ShaderProgram* shader) :
		m_texture{ texture }, m_shader{ shader }{}

	~Cube() = default;

	void renderCube(const glm::mat4& projection, const glm::mat4& model);

private:
	const Texture* m_texture;
	const ShaderProgram* m_shader;

};
