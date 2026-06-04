#pragma once
#include <Gameplay/Camera.h>

class Player
{
public:
	Player(
		glm::vec3 playerPosition = glm::vec3(0.0f, -100.0f, 0.0f),
		glm::vec3 playerTarget = glm::vec3(0.0f, -100.0f, -3.0f)
		):	m_playerPosition {playerPosition}, 
			m_playerTarget {playerTarget}, 
			m_playerFront{glm::normalize(glm::vec3(playerTarget.x - playerPosition.x, 0.0f, playerTarget.z - playerPosition.z))},
			m_playerRight{ glm::normalize(glm::cross(m_playerFront, glm::vec3(0.0f, 1.0f, 0.0f)))},
			m_camera{ Camera(m_playerPosition, m_playerTarget) } { }
	
	~Player() = default;

	void keyboardProcessInput(GLFWwindow* window, float delta);
	void mouseProcessInput(double xpos, double ypos);
	Camera* getCamera();

private:
	glm::vec3 m_playerPosition;
	glm::vec3 m_playerTarget;

	glm::vec3 m_playerFront;
	glm::vec3 m_playerUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_playerRight;

	float m_speed = 10.0f;
	
	float m_yaw = -90.0f;
	double m_lastX = 0;
	const float m_sensitivity = 0.1f;
	bool m_firstInput = true;

	// Camera
	Camera m_camera;
};