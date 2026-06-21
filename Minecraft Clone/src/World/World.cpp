#include "World.h"
#include <Gameplay/Raycaster.h>
#include <chrono>

void World::renderWorld(const glm::mat4& projection) {
	for (const auto& chunk : m_chunkManager.getRenderableChunks()) {
		std::pair<int, int> coords = chunk->getWorldPosition();

		AABB chunkAABB = {
			glm::vec3(coords.first * Globals::CHUNK_WIDTH, -Globals::CHUNK_HEIGHT, coords.second * Globals::CHUNK_WIDTH),
			glm::vec3((coords.first + 1) * Globals::CHUNK_WIDTH, -chunk->getMaxHeight() + 10, (coords.second + 1) * Globals::CHUNK_WIDTH)
		};

		if (m_frustum.isAABBInFrustum(chunkAABB)) {
			chunk->render(projection, getAmbientLightIntensity());
		}
	}
}

void World::updateWorldState() {
	m_chunkManager.update(m_cameraPosition);
}

void World::updateCamera(const glm::vec3& position, const Frustum& frustum, float worldTime) {
	m_cameraPosition.x = std::floor(position.x / Globals::CHUNK_WIDTH);
	m_cameraPosition.y = position.y;
	m_cameraPosition.z = std::floor(position.z / Globals::CHUNK_WIDTH);

	m_frustum = frustum;

	m_worldTime = worldTime;
}										

BlockType World::getBlockAt(int x, int y, int z) const {
	return m_chunkManager.getBlockAt(x, y, z);
}

void World::deleteBlock(BlockHit hit) {
	return m_chunkManager.deleteBlock(hit);
}

void World::addBlock(BlockHit hit, BlockType type, const AABB& playerAABB) {
	return m_chunkManager.addBlock(hit, type, playerAABB);
}

// We dont pass the player position by reference because we want to floor it to get the block coordinates
// So we avoid modifing the player position outside of this function

void World::checkCollisionRadious(glm::vec3 position, const AABB& playerAABB, CollisionRes& res) const{

	position.x = std::floor(position.x);
	position.y = std::floor(position.y);
	position.z = std::floor(position.z);
	
	for (int x = -1; x <= 1; x++) {
		for(int y = -1; y <= 1; y++) {
			for(int z = -1; z <= 1; z++) {
				glm::vec3 blockPos = position + glm::vec3(x, y, z);
				
				if (getBlockAt(blockPos.x, blockPos.y, blockPos.z) == BlockType::Air) continue;
				
				AABB blockAABB(blockPos, blockPos + glm::vec3(1.0f));
				
				if (playerAABB.intersects(blockAABB)) {
					res.collision = true;
					res.blockAABB = blockAABB;
					return;
				}
			}
		}
	}
	res.collision = false;
	res.blockAABB = AABB(glm::vec3(0), glm::vec3(0));
}

// We dont pass the player position by reference because we want to floor it to get the block coordinates
// So we avoid modifing the player position outside of this function

void World::getBlocksBellow(glm::vec3 position, std::vector<AABB>& blocksBellow) const{
	position.x = std::floor(position.x);
	position.y = std::floor(position.y);
	position.z = std::floor(position.z);

	//std::cout << "Player position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	for (int x = -1; x <= 1; x++) {
		for (int z = -1; z <= 1; z++) {
			glm::vec3 blockPos = position + glm::vec3(x, 1, z);
			if (getBlockAt(blockPos.x, blockPos.y, blockPos.z) == BlockType::Air) continue;
			AABB blockAABB(blockPos + glm::vec3(0.01f), blockPos + glm::vec3(0.99f));
			blocksBellow.push_back(blockAABB);
		}
	}
}

bool World::hasBlockBellow(AABB playerAABB, int yPos) const {
	//std::cout << "Checking if player has block bellow. Player AABB: min(" << playerAABB.min.x << ", " << playerAABB.min.y << ", " << playerAABB.min.z << ") max(" << playerAABB.max.x << ", " << playerAABB.max.y << ", " << playerAABB.max.z << ")" << std::endl;
	glm::vec3 position = glm::floor(playerAABB.max);
	position.y = yPos + 2;
	glm::vec3 blockPos = position;
	//std::cout << "Block position: " << blockPos.x << ", " << blockPos.y << ", " << blockPos.z << std::endl;
	if (getBlockAt(blockPos.x, blockPos.y, blockPos.z) != BlockType::Air) return true;

	return false;
}


float World::getAmbientLightIntensity() const {
	float ticks = fmod(m_worldTime, 24000.0f);
	if (ticks >= 0 && ticks < 12000) {
		return 1.0f;
	}

	if (ticks >= 12000 && ticks < 13670) {
		float factor = (float)(ticks - 12000) / (13670 - 12000);
		return 1.0f - (factor * (1.0f - 0.1f));
	}

	if (ticks >= 13670 && ticks < 22330) {
		return 0.1f;
	}

	if (ticks >= 22330 && ticks < 24000) {
		float factor = (float)(ticks - 22330) / (24000 - 22330);
		return 0.2666f + (factor * (1.0f - 0.1f));
	}

	return 1.0f;
}
