#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::processInput(GLFWwindow* w, float delta) {
	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
		m_cameraPosition += m_cameraSpeed * m_cameraFront * delta;
	}
	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
		m_cameraPosition -= m_cameraSpeed * m_cameraFront * delta;
	}
	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
		m_cameraPosition -= m_cameraRight * m_cameraSpeed * delta;
	}
	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
		m_cameraPosition += m_cameraRight * m_cameraSpeed * delta;
	}
	if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_cameraPosition -= m_cameraUp * m_cameraSpeed * delta;
	}
	if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_cameraPosition += m_cameraUp * m_cameraSpeed * delta;
	}
}

glm::mat4 Camera::view() const {
	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}

void Camera::setView(ShaderProgram& shader) const {
	shader.setMat4("view", view());
}