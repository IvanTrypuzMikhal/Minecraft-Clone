#include "Player.h"



void Player::keyboardProcessInput(GLFWwindow* window, float delta) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		m_playerPosition += m_playerFront * m_speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		m_playerPosition -= m_playerFront * m_speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		m_playerPosition -= m_playerRight * m_speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		m_playerPosition += m_playerRight * m_speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_playerPosition -= m_playerUp * m_speed * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_playerPosition += m_playerUp * m_speed * delta;
	}

	m_camera.updatePosition(m_playerPosition);

}

void Player::mouseProcessInput(double xpos, double ypos) {
	if (m_firstInput) {
		m_lastX = xpos;
		m_firstInput = false;
	}

	double xoffset = xpos - m_lastX;

	m_lastX = xpos;

	xoffset *= m_sensitivity;

	m_yaw += xoffset;

	if (m_yaw > 360.0f) m_yaw -= 360.0f;
	if (m_yaw < -360.0f) m_yaw += 360.0f;

	m_playerFront = glm::vec3(cos(glm::radians(m_yaw)), 0.0f, sin(glm::radians(m_yaw)));

	m_playerRight = glm::cross(m_playerFront, m_playerUp);

	m_camera.mouseProcessInput(xpos, ypos);
}

Camera* Player::getCamera() {
	return &m_camera;
}
