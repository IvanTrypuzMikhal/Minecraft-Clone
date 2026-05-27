#include "Cube.h"
#include "Globals.h"


void Cube::renderCube(const glm::mat4& projection, const glm::mat4& model){
	m_shader->setMat4("projection", projection);
	m_shader->setMat4("model", model);
	m_texture->setTexture();
	m_shader->use();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}


