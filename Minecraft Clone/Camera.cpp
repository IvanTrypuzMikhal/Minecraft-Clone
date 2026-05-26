#include "Camera.h"
#include "ShaderProgram.h"


Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::keyboardProcessInput(GLFWwindow* w, float delta) {
	if (m_movementActive) {
		if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
			m_cameraPosition += m_cameraFront * m_cameraSpeed * delta;
		}
		if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
			m_cameraPosition -= m_cameraFront * m_cameraSpeed * delta;
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
}


void Camera::mouseProcessInput(double xpos, double ypos) {
	
	if (m_firstInput) {
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstInput = false;
	}

	double xoffset = xpos - m_lastX;
	double yoffset = m_lastY - ypos;

	m_lastX = xpos;
	m_lastY = ypos;

	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;
	
	m_yaw	+= xoffset;
	m_pitch += yoffset;

	if (m_pitch >  89.0f) m_pitch =  89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;

	m_cameraFront = glm::vec3(	cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),	// x
								sin(glm::radians(m_pitch)),								// y
								sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))); // z

	m_cameraRight = glm::cross(m_cameraFront, m_up);
}


void Camera::scrollProcessInput(double xoffset, double yoffset) {
	m_fov -= (float)yoffset;
	if (m_fov < 1.0f)	m_fov = 1.0f;
	if (m_fov > 45.0f)	m_fov = 45.0f;
}

glm::mat4 Camera::view() const {
	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}

void Camera::setView(ShaderProgram& shader) const {
	shader.setMat4("view", view());
}

void Camera::setFirstInput() {
	m_firstInput = true;
}

void Camera::setMovement(bool trueFalse) {
	m_movementActive = trueFalse;
}

float Camera::getFov() const {
	return m_fov;
}