#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class ShaderProgram;

// TODO: Update camera to use 

class Camera
{
public:
	Camera();
	~Camera();

	void processInput(GLFWwindow* camera, float delta);
	void keyboardProcessInput(GLFWwindow* camera, float delta);
	void mouseProcessInput(double xpos, double ypos);
	void scrollProcessInput(double xoffset, double yoffset);

	float getFov() const;

	glm::mat4 view() const;

	void setView(ShaderProgram& shader) const;
	void setFirstInput();
	void setMovement(bool trueFalse);
private:
	glm::vec3 m_cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Camear axes
	glm::vec3 m_cameraFront = glm::normalize(m_cameraTarget - m_cameraPosition);
	glm::vec3 m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_up));
	glm::vec3 m_cameraUp = glm::cross(m_cameraRight, m_cameraFront);

	// Euler angles
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;

	//	Other variables
	float m_cameraSpeed = 1.5f;
	double m_lastX = 0, m_lastY = 0;
	const float m_sensitivity = 0.1f;
	float m_fov = 45.0f;

	// Booleans
	bool m_firstInput = true;
	bool m_movementActive = true;
};