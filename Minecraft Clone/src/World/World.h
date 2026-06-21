#pragma once
#include <World/ChunkPipeline/ChunkManager.h>
#include "Chunk.h"
#include <Rendering/ShaderProgram.h>
#include "TSQueue.h"
#include "TerrainGeneration.h"
#include <Gameplay/Camera.h>
#include "ChunkPackage.h"
#include "FileIOThread.h"
#include <unordered_set>
#include <utility>
#include <map>
#include <memory>
#include <Gameplay/Frustum.h>
#include <Core/Time.h>
#include "ThreadPool.h"

struct BlockHit;

class World
{
public:
	World(std::shared_ptr<ShaderProgram> shader) : m_terrain{ TerrainGenerator() }, m_shader{ shader }, m_chunkManager{ shader } {};
	~World() = default;

	void renderWorld(const glm::mat4& projection);
	void updateCamera(const glm::vec3& position, const Frustum& frustum, float worldTime);
	
	void deleteBlock(BlockHit hit);
	void addBlock(BlockHit hit, BlockType type, const AABB& playerAABB);
	
	void updateWorldState();
	void checkCollisionRadious(glm::vec3 position, const AABB& playerAABB, CollisionRes& res) const;
	void getBlocksBellow(glm::vec3 position, std::vector<AABB>& blocksBellow) const;
	bool hasBlockBellow(AABB playerAABB, int yPos) const;
	BlockType getBlockAt(int x, int y, int z) const;
	float getAmbientLightIntensity() const;

private:

	std::shared_ptr<ShaderProgram> m_shader;
	glm::vec3 m_cameraPosition;
	Frustum& m_frustum = *new Frustum();
	float m_worldTime = 0.0f;
	TerrainGenerator m_terrain;

	ChunkManager m_chunkManager;

}; 