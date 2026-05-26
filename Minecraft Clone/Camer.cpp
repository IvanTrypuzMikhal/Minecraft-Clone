#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::processInput(GLFWwindow* w) {
	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
		m_cameraPosition += m_cameraSpeed * m_cameraFront;
	}
	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
		m_cameraPosition -= m_cameraSpeed * m_cameraFront;
	}
	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
		m_cameraPosition -= m_cameraRight * m_cameraSpeed;

	}
	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
		m_cameraPosition += m_cameraRight * m_cameraSpeed;
	}
}

glm::mat4 Camera::view() const {
	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}