#pragma once
#include "Chunk.h"
#include <Rendering/ShaderProgram.h>
#include "TSQueue.h"
#include "TerrainGeneration.h"
#include <Gameplay/Camera.h>
#include "MeshThread.h"
#include "TerrainThread.h"
#include "ChunkPackage.h"
#include <unordered_set>
#include <utility>
#include <map>
#include <memory>
#include <Gameplay/AABB.h>

struct BlockHit;

struct PairHash {
	size_t operator()(const std::pair<int, int>& p) const {
		return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
};

class World
{
public:
	World(ShaderProgram* shader) : m_terrain{ TerrainGenerator() }, m_shader { shader }, m_terrainThread{ TerrainThread(m_shader, m_terrain) } {
	}
	~World() = default;

	void renderWorld(const glm::mat4& projection);
	void updateCameraPosition(const glm::vec3& position);
	bool checkNearbyChunksTerrainReady(int x, int z);
	bool checkNearbyChunksDecorationReady(int x, int z);
	
	void deleteBlock(BlockHit hit);
	void addBlock(BlockHit hit, BlockType type, const AABB& playerAABB);
	
	void updateWorldState();
	void checkChunksWithTerrain();
	void checkFinishedChunksWithMesh();
	void enqueMeshByCoords(std::pair<int, int> chunkPos);
	void checkCollisionRadious(glm::vec3 position, const AABB& playerAABB, CollisionRes& res) const;
	void getBlocksBellow(glm::vec3 position, std::vector<AABB>& blocksBellow) const;
	bool hasBlockBellow(AABB playerAABB, int yPos) const;
	BlockType getBlockAt(int x, int y, int z) const;

	void getNearbyChunks(std::pair<int, int> chunkPos, ChunkPackage& package);

private:

	std::map<std::pair<int, int>, ChunkState> m_chunks;
	ShaderProgram* m_shader;
	std::unordered_set<std::pair<int, int>, PairHash> m_requestedChunks;

	MeshThread m_meshThread;
	TerrainThread m_terrainThread;

	// Camera
	glm::vec3 m_cameraPosition;

	// Terrain
	TerrainGenerator m_terrain;
};