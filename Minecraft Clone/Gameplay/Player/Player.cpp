#include "Player.h"
#include <Gameplay/Raycaster.h>



void Player::keyboardProcessInput(GLFWwindow* window) {
	if (m_controlsActive == false) return;
	m_moveForward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	m_moveBack = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	m_moveLeft = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	m_moveRight = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	m_crouch = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	m_run = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && m_onGround) {
		m_velocity.y += 7.0f;
		m_onGround = false;
	}
	

	//m_camera.updatePosition(m_playerPosition);
}

void Player::mouseProcessInput(double xpos, double ypos) {
	if (m_controlsActive == false) return;

	if (m_firstMouseInput) {
		m_lastX = xpos;
		m_firstMouseInput = false;
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

void Player::mouseButtonProcessInput(int button, int action, int mods) {

	BlockHit hit;
	if (!Raycaster::traceRay(m_world, m_camera, Globals::INTERACTION_DISTANCE, hit)) return;

	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) m_world->deleteBlock(hit);
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) m_world->addBlock(hit, BlockType::Dirt, m_playerAABB);

}



Camera* Player::getCamera() {
	return &m_camera;
}

void Player::update(float delta) {
	glm::vec3 dir = glm::vec3(0.0f);

	if (m_moveForward)	dir += m_playerFront;
	if (m_moveBack)		dir -= m_playerFront;
	if (m_moveLeft)		dir -= m_playerRight;
	if (m_moveRight)	dir += m_playerRight;

	if (glm::length(dir) > 0.0f) dir = glm::normalize(dir);

	m_velocity.x = dir.x * m_speed;
	m_velocity.z = dir.z * m_speed;

	if (!m_onGround) {
		if(m_velocity.y > 0.0f) m_velocity.y -= 2*9.81f * delta;
		else m_velocity.y -= 3 * 9.81f * delta;
	}
	else {
		m_velocity.y = 0.0f;
		if (m_jump) {
			m_velocity.y = 5.0f;
			m_onGround = false;
		}
		else if (m_crouch) {
			//m_velocity.y = -5.0f;
		}
	}

	
	// X axis collision
	m_playerPosition.x += m_velocity.x * delta;
	m_worldPosition.x = m_playerPosition.x; 
	updateAABB();

	if (m_world->checkCollisionRadious(m_worldPosition, m_playerAABB)) {
		m_playerPosition.x -= m_velocity.x * delta; 
		m_worldPosition.x = m_playerPosition.x;     
		updateAABB();                               
		m_velocity.x = 0.0f;
	}
	
	// Z axis collision
	m_playerPosition.z += m_velocity.z * delta;
	m_worldPosition.z = m_playerPosition.z;
	updateAABB();

	if (m_world->checkCollisionRadious(m_worldPosition, m_playerAABB)) {
		m_playerPosition.z -= m_velocity.z * delta;
		m_worldPosition.z = m_playerPosition.z;
		updateAABB();
		m_velocity.z = 0.0f;
	}

	
	m_onGround = false;
	m_playerPosition.y += m_velocity.y * delta;
	m_worldPosition.y = -m_playerPosition.y;
	updateAABB();
	if (m_world->checkCollisionRadious(m_worldPosition, m_playerAABB)) {
		m_playerPosition.y -= m_velocity.y * delta;
		m_worldPosition.y = -m_playerPosition.y;
		updateAABB();
		
		if (m_velocity.y < 0.0f) {
			m_onGround = true;
		}
		m_velocity.y = 0.0f;
	}


	
	float cameraHeight = m_crouch ? 0.85f : 1.0;
	m_camera.updatePosition(m_playerPosition + glm::vec3(0.0f, cameraHeight, 0.0f));
	if (m_crouch) {
		m_speed = 2.5f;
		m_lastFrameRunning = false;
		m_lastFrameCrouching = true;
	}
	if (!m_crouch && m_lastFrameCrouching) {
		m_speed = 5.0f;
		m_lastFrameCrouching = false;
	}
	else if (m_run && m_moveForward) {
		m_speed = 8.0f;
		m_lastFrameRunning = true;
	}
	else if (m_lastFrameRunning && !m_moveForward) {
		m_speed = 5.0f;
		m_lastFrameRunning = false;
	}
}


void Player::updateAABB() {
	if (m_crouch) {

		m_playerAABB.min = m_worldPosition - glm::vec3(0.3f, 0.9 + 0.15, 0.3f);
		m_playerAABB.max = m_worldPosition + glm::vec3(0.3f, 0.9 - 0.15, 0.3f);
	}
	else {
		// Hack, but needs fixing. The AABB is centered around the player's position, but the player's position is at their feet, so we need to offset the AABB downwards by 0.25f to make it fit better.
		m_worldPosition.y -= 0.15f;
		m_playerAABB.min = m_worldPosition - glm::vec3(0.3f, 0.9f, 0.3f);
		m_playerAABB.max = m_worldPosition + glm::vec3(0.3f, 0.9f, 0.3f);
		m_worldPosition.y += 0.15f;
	}
}