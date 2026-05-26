#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram;
class Camera;

class SpaceTransformation
{
public:
	SpaceTransformation(int width, int height);

	void setModel(ShaderProgram& shader) const;
	void setProjection(ShaderProgram& shader) const;
	void setView(ShaderProgram& shader, Camera& camera) const;
	
	glm::mat4 model() const;
	glm::mat4 projection() const;

private:
	glm::mat4 m_model		= glm::mat4(1.0f);
	glm::mat4 m_projection	= glm::mat4(1.0f);
};


