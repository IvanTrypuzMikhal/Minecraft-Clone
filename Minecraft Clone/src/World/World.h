#pragma once
#include "Chunk.h"
#include <Rendering/ShaderProgram.h>
#include "TSQueue.h"
#include "TerrainGeneration.h"
#include <Gameplay/Camera.h>
#include "MeshThread.h"
#include "TerrainThread.h"
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
	World(std::shared_ptr<ShaderProgram> shader) : m_terrain{ TerrainGenerator() }, m_shader{ shader } {
	}
	~World() = default;

	void renderWorld(const glm::mat4& projection);
	void updateCamera(const glm::vec3& position, const Frustum& frustum, float worldTime);
	bool checkNearbyChunksTerrainReady(int x, int z);
	bool checkNearbyChunksDecorationReady(int x, int z);
	
	void deleteBlock(BlockHit hit);
	void addBlock(BlockHit hit, BlockType type, const AABB& playerAABB);
	
	void updateWorldState();
	void checkChunksWithTerrain();
	void checkFinishedChunksWithMesh();
	void checkChunksToBeFreed();
	void checkFinishedChunksLoadedFromMemory();
	void enqueMeshByCoords(std::pair<int, int> chunkPos);
	void checkCollisionRadious(glm::vec3 position, const AABB& playerAABB, CollisionRes& res) const;
	void getBlocksBellow(glm::vec3 position, std::vector<AABB>& blocksBellow) const;
	bool hasBlockBellow(AABB playerAABB, int yPos) const;
	BlockType getBlockAt(int x, int y, int z) const;

	void getNearbyChunks(std::pair<int, int> chunkPos, ChunkPackage& package);
	float getAmbientLightIntensity() const;

	// Lambdas
	std::function<void()> getTerrainGenerationTask(std::pair<int, int> coords);
	std::function<void()> getMeshBuildingTask(ChunkPackage package);
private:

	std::unordered_map<std::pair<int, int>, ChunkState, PairHash> m_chunks;
	std::shared_ptr<ShaderProgram> m_shader;
	std::unordered_set<std::pair<int, int>, PairHash> m_requestedChunks;
	std::unordered_map<std::pair<int, int>, Deltas, PairHash> m_savingChunks;
	std::unordered_set<std::pair<int, int>, PairHash> m_mainMemSavedChunks;
	std::unordered_map<std::pair<int, int>, ChunkSnapshot, PairHash> m_pendingDeltas;
		

	//MeshThread m_meshThread;
	//TerrainThread m_terrainThread;
	FileIOThread m_fileIOThread;
	ThreadPool m_threadPool;

	// Buffers
	TSQueue<FinishedChunk> m_finishedTerrainChunks;
	TSQueue<std::pair<int, int>> m_finishedMeshChunks;

	//TSQueue<std::pair<int, int>> m_terrainQueue;



	glm::vec3 m_cameraPosition;
	Frustum& m_frustum = *new Frustum();
	float m_worldTime = 0.0f;

	TerrainGenerator m_terrain;
};