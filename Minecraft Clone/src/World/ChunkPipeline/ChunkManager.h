#pragma once
#include <World/TSQueue.h>
#include <World/ChunkPackage.h>
#include <World/FileIOThread.h>
#include <World/ThreadPool.h>
#include <World/TerrainGeneration.h>
#include <Rendering/ShaderProgram.h>
#include <Gameplay/Frustum.h>

struct BlockHit;

class ChunkManager
{
public:
	ChunkManager(std::shared_ptr<ShaderProgram> shader) : m_terrain{ TerrainGenerator() }, m_shader{ shader } {};
	~ChunkManager() = default;

	void update(const glm::vec3& cameraPosition);
	void generateNearbyChunksRequests();

	std::vector<std::shared_ptr<Chunk>> getRenderableChunks();

	void checkChunksWithTerrain();
	void checkFinishedChunksWithDecoration();
	void checkFinishedChunksWithLighting();
	void checkFinishedChunksWithMesh();
	void checkChunksToBeFreed();
	void checkFinishedChunksLoadedFromMemory();
	void promoteChunk(std::pair<int, int> coords);
	void enqueMeshByCoords(std::pair<int, int> chunkPos);
	void enqueLightingByCoords(std::pair<int, int> chunkPos, bool isDirty);
	bool checkNearbyChunksSameState(std::pair<int, int> coords, State state);
	void getNearbyChunks(std::pair<int, int> chunkPos, ChunkPackage& package);
	std::array<std::pair<int, int>, 8> get8Neighbors(std::pair<int, int> coords) const;


	std::function<void()> getTerrainGenerationTask(std::pair<int, int> coords);
	std::function<void()> getDecorationTask(ChunkPackage package);
	std::function<void()> getLightingTask(ChunkPackage package);
	std::function<void()> getMeshBuildingTask(ChunkPackage package);


	// Block management
	BlockType getBlockAt(int x, int y, int z) const;
	void deleteBlock(BlockHit hit);
	void addBlock(BlockHit hit, BlockType type, const AABB& playerAABB);


	// Getters
	[[nodiscard]] const std::unordered_map<std::pair<int, int>, ChunkState, PairHash>& getChunks() const { return m_chunks; }
	[[nodiscard]] const std::unordered_set<std::pair<int, int>, PairHash>& getRequestedChunks() const { return m_requestedChunks; }
	[[nodiscard]] const std::unordered_map<std::pair<int, int>, Deltas, PairHash>& getSavingChunks() const { return m_savingChunks; }
	[[nodiscard]] const std::unordered_set<std::pair<int, int>, PairHash>& getMainMemSavedChunks() const { return m_mainMemSavedChunks; }
	[[nodiscard]] const std::unordered_map<std::pair<int, int>, ChunkSnapshot, PairHash>& getPendingDeltas() const { return m_pendingDeltas; }

private:

	// Ready to render chunks
	std::unordered_map<std::pair<int, int>, ChunkState, PairHash> m_chunks;

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
	TerrainGenerator m_terrain;
	std::shared_ptr<ShaderProgram> m_shader;
	glm::vec3 m_cameraPosition;

	// Queues for finished terrain and mesh chunks
	TSQueue<FinishedChunk> m_finishedTerrainChunks;
	TSQueue<std::pair<int, int>> m_finishedDecoratedChunks;
	TSQueue<std::pair<int, int>> m_finishedLightingChunks;
	TSQueue<std::pair<int, int>> m_finishedMeshChunks;
};
