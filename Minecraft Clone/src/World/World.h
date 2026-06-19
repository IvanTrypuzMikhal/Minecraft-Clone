#pragma once
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
	World(std::shared_ptr<ShaderProgram> shader) : m_terrain{ TerrainGenerator() }, m_shader{ shader } {

	}
	~World() = default;

	void renderWorld(const glm::mat4& projection);
	void updateCamera(const glm::vec3& position, const Frustum& frustum, float worldTime);
	
	void deleteBlock(BlockHit hit);
	void addBlock(BlockHit hit, BlockType type, const AABB& playerAABB);
	
	void updateWorldState();
	void checkChunksWithTerrain();
	void checkFinishedChunksWithDecoration();
	void checkFinishedChunksWithLighting();
	void checkFinishedChunksWithMesh();
	void checkChunksToBeFreed();
	void checkFinishedChunksLoadedFromMemory();
	void enqueMeshByCoords(std::pair<int, int> chunkPos);
	void checkCollisionRadious(glm::vec3 position, const AABB& playerAABB, CollisionRes& res) const;
	void getBlocksBellow(glm::vec3 position, std::vector<AABB>& blocksBellow) const;
	void promoteChunk(std::pair<int, int> coords);
	bool checkNearbyChunksSameState(std::pair<int, int> coords, State state);
	bool hasBlockBellow(AABB playerAABB, int yPos) const;
	BlockType getBlockAt(int x, int y, int z) const;


	void getNearbyChunks(std::pair<int, int> chunkPos, ChunkPackage& package);
	float getAmbientLightIntensity() const;
	std::array<std::pair<int, int>, 8> get8Neighbors(std::pair<int, int> coords) const;

	// Lambdas
	std::function<void()> getTerrainGenerationTask(std::pair<int, int> coords);
	std::function<void()> getDecorationTask(ChunkPackage package);
	std::function<void()> getLightingTask(ChunkPackage package);
	std::function<void()> getMeshBuildingTask(ChunkPackage package);
private:
	// All chunks that are loaded in memory
	std::unordered_map<std::pair<int, int>, ChunkState, PairHash> m_chunks;
	
	std::shared_ptr<ShaderProgram> m_shader;
	
	// Chunks that are requested ( used to avoid requesting the same chunk multiple times )
	std::unordered_set<std::pair<int, int>, PairHash> m_requestedChunks;

	// Chunks that are being saved to memory ( used to avoid saving the same chunk multiple times / to reconstruct the chunk directly from mem )
	std::unordered_map<std::pair<int, int>, Deltas, PairHash> m_savingChunks;
	
	// Chunks that are saved in memory ( used to know which chunks are saved in memory and can be loaded from memory )
	std::unordered_set<std::pair<int, int>, PairHash> m_mainMemSavedChunks;

	// Chunks that are loaded from memory and need to be reconstructed
	std::unordered_map<std::pair<int, int>, ChunkSnapshot, PairHash> m_pendingDeltas;
		
	FileIOThread m_fileIOThread;
	ThreadPool m_threadPool;

	// Queues for finished terrain and mesh chunks
	TSQueue<FinishedChunk> m_finishedTerrainChunks;
	TSQueue<std::pair<int, int>> m_finishedDecoratedChunks;
	TSQueue<std::pair<int, int>> m_finishedLightingChunks;
	TSQueue<std::pair<int, int>> m_finishedMeshChunks;

	glm::vec3 m_cameraPosition;
	Frustum& m_frustum = *new Frustum();
	float m_worldTime = 0.0f;

	TerrainGenerator m_terrain;
}; 