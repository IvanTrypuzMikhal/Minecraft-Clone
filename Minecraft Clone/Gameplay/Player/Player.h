#pragma once
#include <Gameplay/Camera.h>
#include <Gameplay/AABB.h>
#include <World/World.h>

class Player
{
public:
	Player(
		World* world,
		glm::vec3 playerPosition = glm::vec3(0.0f, -80.0f, 0.0f),
		glm::vec3 playerTarget = glm::vec3(0.0f, -80.0f, -3.0f)
		):	m_playerPosition {playerPosition}, 
		m_worldPosition{ glm::vec3(playerPosition.x, -playerPosition.y, playerPosition.z) },
			m_playerTarget {playerTarget}, 
			m_playerFront{glm::normalize(glm::vec3(playerTarget.x - playerPosition.x, 0.0f, playerTarget.z - playerPosition.z))},
			m_playerRight{ glm::normalize(glm::cross(m_playerFront, glm::vec3(0.0f, 1.0f, 0.0f)))},
			m_camera{ Camera(m_playerPosition, m_playerTarget) },
			m_world{ world } { }
	
	~Player() = default;

	void keyboardProcessInput(GLFWwindow* window);
	void mouseProcessInput(double xpos, double ypos);
	void mouseButtonProcessInput(int button, int action, int mods);
	void update(float delta);

	void setControlsActive(bool active) { 
		m_controlsActive = active; 
		m_camera.setControls(active);
	}
	void setFirstInput() { 
		m_firstMouseInput = true;
		m_camera.setFirstInput(); 
	}
	Camera* getCamera();

private:
	void updateAABB();


	glm::vec3 m_playerPosition;
	glm::vec3 m_worldPosition;
	glm::vec3 m_playerTarget;

	glm::vec3 m_playerFront;
	glm::vec3 m_playerUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_playerRight;
	glm::vec3 m_velocity;
	
	float m_speed = 10.0f;
	float m_yaw = -90.0f;
	double m_lastX = 0;
	const float m_sensitivity = 0.1f;

	AABB m_playerAABB{ m_playerPosition, m_playerPosition + glm::vec3(0.6f, 1.8f, 0.6f) };

	// Camera
	Camera m_camera;

	// World poitner
	World* m_world;

	// Flags
	bool m_controlsActive = true;
	bool m_firstMouseInput = true;

	bool m_moveForward	= false;
	bool m_moveBack		= false;
	bool m_moveRight	= false;
	bool m_moveLeft		= false;
	bool m_jump			= false;
	bool m_crouch		= false;
	bool m_onGround		= true;
};