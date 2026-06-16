#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Gameplay/Input/InputManager.h>

class ShaderProgram;



class Camera
{
public:
	Camera(
		glm::vec3 cameraPosition = glm::vec3(0.0f, -100.0f, 0.0f),
		glm::vec3 cameraTarget = glm::vec3(0.0f, -100.0f, -3.0f),
		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f)
	) : m_cameraPosition{ cameraPosition }, m_cameraTarget{ cameraTarget }, m_up{ upVector } {}
	~Camera() = default;

	void mouseProcessInput();
	void scrollProcessInput();

	void updatePosition(glm::vec3 newPos);

	[[nodiscard]] float getFov() const;
	[[nodiscard]] float getYaw() const;
	[[nodiscard]] float getPitch() const;
	[[nodiscard]] const glm::vec3& getCameraPosition() const;
	[[nodiscard]] float getCameraPositionX() const;
	[[nodiscard]] float getCameraPositionY() const;
	[[nodiscard]] float getCameraPositionZ() const;
	[[nodiscard]] float getCameraLookAtX() const;
	[[nodiscard]] float getCameraLookAtY() const;
	[[nodiscard]] float getCameraLookAtZ() const;
	[[nodiscard]] glm::vec3 getCameraLookAt() const;
	[[nodiscard]] glm::vec3& getCameraPosition();
	[[nodiscard]] glm::mat4 view() const;

	void setView(ShaderProgram& shader) const;
	void setFirstInput();
	void setControls(bool trueFalse);
private:
	// Setup axes
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_up;

	// Camear axes
	glm::vec3 m_cameraFront = glm::normalize(m_cameraTarget - m_cameraPosition);
	glm::vec3 m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_up));
	glm::vec3 m_cameraUp = glm::cross(m_cameraRight, m_cameraFront);

	// Euler angles
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;

	//	Other variables
	float m_cameraSpeed = 10.0f;
	double m_lastX = 0, m_lastY = 0;
	const float m_sensitivity = 0.1f;
	float m_fov = 90.0f;

	// Booleans
	bool m_firstInput = true;
	bool m_controlsActive = true;
};