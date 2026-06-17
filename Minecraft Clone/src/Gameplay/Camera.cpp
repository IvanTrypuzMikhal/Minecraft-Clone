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

void Camera::getFrustum(const glm::mat4& projection, Frustum& frustum) const {
	// No model matrix is ever applied to the camera, so we can just use the view matrix directly
	glm::mat4 M = projection * view();
	Plane leftPlane;
	leftPlane.normal.x = M[0][3] + M[0][0];
	leftPlane.normal.y = M[1][3] + M[1][0];
	leftPlane.normal.z = M[2][3] + M[2][0];
	leftPlane.distance = M[3][3] + M[3][0];

	float length = glm::length(leftPlane.normal);
	leftPlane.normal /= length;
	leftPlane.distance /= length;

	Plane rightPlane;
	rightPlane.normal.x = M[0][3] - M[0][0];
	rightPlane.normal.y = M[1][3] - M[1][0];
	rightPlane.normal.z = M[2][3] - M[2][0];
	rightPlane.distance = M[3][3] - M[3][0];

	length = glm::length(rightPlane.normal);
	rightPlane.normal /= length;
	rightPlane.distance /= length;

	Plane bottomPlane;
	bottomPlane.normal.x = M[0][3] + M[0][1];
	bottomPlane.normal.y = M[1][3] + M[1][1];
	bottomPlane.normal.z = M[2][3] + M[2][1];
	bottomPlane.distance = M[3][3] + M[3][1];

	length = glm::length(bottomPlane.normal);
	bottomPlane.normal /= length;
	bottomPlane.distance /= length;

	Plane topPlane;
	topPlane.normal.x = M[0][3] - M[0][1];
	topPlane.normal.y = M[1][3] - M[1][1];
	topPlane.normal.z = M[2][3] - M[2][1];
	topPlane.distance = M[3][3] - M[3][1];

	length = glm::length(topPlane.normal);
	topPlane.normal /= length;
	topPlane.distance /= length;

	Plane nearPlane;
	nearPlane.normal.x = M[0][3] + M[0][2];
	nearPlane.normal.y = M[1][3] + M[1][2];
	nearPlane.normal.z = M[2][3] + M[2][2];
	nearPlane.distance = M[3][3] + M[3][2];

	length = glm::length(nearPlane.normal);
	nearPlane.normal /= length;
	nearPlane.distance /= length;

	Plane farPlane;
	farPlane.normal.x = M[0][3] - M[0][2];
	farPlane.normal.y = M[1][3] - M[1][2];
	farPlane.normal.z = M[2][3] - M[2][2];
	farPlane.distance = M[3][3] - M[3][2];

	length = glm::length(farPlane.normal);
	farPlane.normal /= length;
	farPlane.distance /= length;

	frustum.planes[0] = leftPlane;
	frustum.planes[1] = rightPlane;
	frustum.planes[2] = bottomPlane;
	frustum.planes[3] = topPlane;
	frustum.planes[4] = nearPlane;
	frustum.planes[5] = farPlane;
}