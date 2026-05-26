#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera
{
public:
	Camera();
	~Camera();

	void processInput(GLFWwindow* camera);
	glm::mat4 view() const;

private:
	glm::vec3 m_cameraPosition	= glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraTarget	= glm::vec3(0.0f, 0.0f,-1.0f);
	glm::vec3 m_up				= glm::vec3(0.0f, 1.0f, 0.0f);

	// Camear axes
	glm::vec3 m_cameraFront		= glm::normalize(m_cameraTarget - m_cameraPosition);
	glm::vec3 m_cameraRight		= glm::normalize(glm::cross(m_cameraFront, m_up));
	glm::vec3 m_cameraUp		= glm::cross(m_cameraRight, m_cameraFront);
	
	//Other variables
	float m_cameraSpeed = 0.05f;
};
