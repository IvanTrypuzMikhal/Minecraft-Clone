#include "SpaceTransformation.h"
#include "ShaderProgram.h"
#include "camera.h"

SpaceTransformation::SpaceTransformation(int width, int height) {
	m_projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
}


glm::mat4 SpaceTransformation::model() const {
	return m_model;
}

glm::mat4 SpaceTransformation::projection() const {
	return m_projection;
}

void SpaceTransformation::setModel(ShaderProgram& shader) const{
	shader.setMat4("model", m_model);
}

void SpaceTransformation::setProjection(ShaderProgram& shader) const {
	shader.setMat4("projection", m_projection);
}

void SpaceTransformation::setView(ShaderProgram& shader, Camera& camera) const {
	shader.setMat4("view", camera.view());
}