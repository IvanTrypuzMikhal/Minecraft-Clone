#include "Camera.h"
#include <Rendering/ShaderProgram.h>

void Camera::mouseProcessInput() {
	if (m_controlsActive == false) return;

	if (m_firstInput) {
		m_lastX = InputManager::GetCursorScreenX();
		m_lastY = InputManager::GetCursorScreenY();
		m_firstInput = false;
	}

	double xoffset = InputManager::GetMouseOffsetX();
	double yoffset = -InputManager::GetMouseOffsetY();

	m_lastX = InputManager::GetCursorScreenX();
	m_lastY = InputManager::GetCursorScreenY();

	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;
	
	m_yaw	+= static_cast<float>(xoffset);
	m_pitch += static_cast<float>(yoffset);

	if (m_pitch >  89.0f) m_pitch =  89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;
	if (m_yaw > 360.0f) m_yaw -= 360.0f;
	if (m_yaw < -360.0f) m_yaw += 360.0f;

	m_cameraFront = glm::vec3(	cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),	// x
								sin(glm::radians(m_pitch)),								// y
								sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))); // z

	m_cameraRight = glm::cross(m_cameraFront, m_up);
}

void Camera::updatePosition(glm::vec3 newPos) {
	m_cameraPosition = newPos;
}

void Camera::scrollProcessInput() {
	m_fov -= static_cast<float>(InputManager::GetMouseWheelValue());
	if (m_fov < 1.0f)	m_fov = 1.0f;
	if (m_fov > 90.0f)	m_fov = 90.0f;
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

void Camera::setControls(bool trueFalse) {
	m_controlsActive = trueFalse;
}

glm::vec3& Camera::getCameraPosition() {
	return m_cameraPosition;
}

glm::vec3 Camera::getCameraLookAt() const{
	return m_cameraFront;
}

float Camera::getFov() const { 
	return m_fov; 
}

float Camera::getYaw() const {
	return m_yaw;
}

float Camera::getPitch() const {
	return m_pitch;
}

const glm::vec3& Camera::getCameraPosition() const {
	return m_cameraPosition;
}
float Camera::getCameraPositionX() const {
	return m_cameraPosition.x;
}
float Camera::getCameraPositionY() const {
	return m_cameraPosition.y;
}
float Camera::getCameraPositionZ() const {
	return m_cameraPosition.z;
}
float Camera::getCameraLookAtX() const {
	return m_cameraFront.x;
}
float Camera::getCameraLookAtY() const {
	return m_cameraFront.y;
}
float Camera::getCameraLookAtZ() const {
	return m_cameraFront.z;
}