#include "Player.h"
#include <Gameplay/Raycaster.h>
#include <vector>

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

	m_yaw += static_cast<float>(xoffset);

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
	calculateVelocity(delta);

	if (m_crouch && m_onGround) {
		checkShiftingCollisionAxis(delta, 0);
		checkShiftingCollisionAxis(delta, 2);
	}
	else {
		checkCollisionOnAxis(delta, 0);
		checkCollisionOnAxis(delta, 2);
	}
	checkCollisionOnYAxis(delta);

	m_camera.updatePosition(m_playerPosition + glm::vec3(0.0f, m_crouch ? 0.85f : 1.0, 0.0f));
	setSpeed();
}


void Player::updateAABB() {
	float halfWidth = 0.3f;
	float height = m_crouch ? 1.5 : 1.8f;
	float yOffset = m_crouch ? 0.0f : 0.15f;

	auto centerPos = m_worldPosition;
	centerPos.y -= yOffset;

	m_playerAABB.min = centerPos - glm::vec3(halfWidth, height / 2, halfWidth);
	m_playerAABB.max = centerPos + glm::vec3(halfWidth, height / 2, halfWidth);

	
}

void Player::setSpeed() {
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
		m_speed = 7.0f;
		m_lastFrameRunning = true;
	}
	else if (m_lastFrameRunning && !m_moveForward) {
		m_speed = 5.0f;
		m_lastFrameRunning = false;
	}

	m_lastFrameWalking = m_moveForward || m_moveBack || m_moveLeft || m_moveRight;

}

void Player::calculateVelocity(float delta) {
	auto dir = glm::vec3(0.0f);

	if (m_moveForward)	dir += m_playerFront;
	if (m_moveBack)		dir -= m_playerFront;
	if (m_moveLeft)		dir -= m_playerRight;
	if (m_moveRight)	dir += m_playerRight;

	if (glm::length(dir) > 0.0f) {
		m_velocity.x = glm::mix(m_velocity.x, dir.x * m_speed, m_acceleration * delta);
		m_velocity.z = glm::mix(m_velocity.z, dir.z * m_speed, m_acceleration * delta);
		glm::normalize(m_velocity);
	}
	else {
		m_velocity.x -= m_velocity.x * m_friction * delta;
		m_velocity.z -= m_velocity.z * m_friction * delta;
	}
	if (m_velocity.y > 0.0f) {
		m_velocity.y -= 2 * 9.81f * delta;
	}
	else {
		m_velocity.y -= 3 * 9.81f * delta;
	}
}

CollisionRes Player::moveAndCollide(float dx, float dy, float dz) {
	m_playerPosition.x += dx;
	m_playerPosition.y += dy;
	m_playerPosition.z += dz;
	m_worldPosition.x = m_playerPosition.x;
	m_worldPosition.z = m_playerPosition.z;
	m_worldPosition.y = -m_playerPosition.y;
	updateAABB();

	CollisionRes res;
	m_world->checkCollisionRadious(m_worldPosition, m_playerAABB, res);
	return res;
}

void Player::checkCollisionOnAxis(float delta, int axis) {
	glm::vec3 movement(0.0f);
	movement[axis] += m_velocity[axis] * delta;
	CollisionRes res = moveAndCollide(movement.x, movement.y, movement.z);
	if (res.collision) {
		m_playerPosition[axis] -= m_velocity[axis] * delta;
		m_worldPosition[axis] = m_playerPosition[axis];
		m_velocity[axis] = 0.0f;
	}
	updateAABB();
}

void Player::checkCollisionOnYAxis(float delta) {
	m_onGround = false;
	CollisionRes res = moveAndCollide(0.0f, m_velocity.y * delta, 0.0f);
	if (res.collision) {

		// If player is jumping and collides with a block above, we want to move the player down to the point where they are just touching the block, not inside it.
		if (m_velocity.y > 0) {
			m_playerPosition.y -= res.blockAABB.max.y - m_playerAABB.min.y;
			m_worldPosition.y = -m_playerPosition.y;
		}

		// If the player is falling and collides with a block below, we want to move the player up to the point where they are just touching the block, not inside it.
		else {
			m_playerPosition.y -= res.blockAABB.min.y - m_playerAABB.max.y;
			m_worldPosition.y = -m_playerPosition.y;
		}

		// If the player is falling and collides with a block below, we want to set the onGround flag to true so that the player can jump again.
		if (m_velocity.y < 0.0f) {
			m_onGround = true;
		}

		// Set velocity to 0 so that the player doesn't keep moving in the direction of the collision
		m_velocity.y = 0.0f;
	}
	updateAABB();
}

void Player::checkShiftingCollisionAxis(float delta, int axis) {
	bool hasBlockBellow = false;
	updateAABB();
	std::vector<AABB> blocksBellow;
	blocksBellow.reserve(9);
	m_world->getBlocksBellow(m_worldPosition, blocksBellow);

	glm::vec3 oldPosition = m_playerPosition;

	checkCollisionOnAxis(delta, axis);

	for (const AABB& abb : blocksBellow) {
		if (m_playerAABB.overAABB(abb)) {
			hasBlockBellow = true;
			break;
		}
	}
	if (!hasBlockBellow) {
		m_playerPosition[axis] = oldPosition[axis];
		m_worldPosition[axis] = oldPosition[axis];
	}
}