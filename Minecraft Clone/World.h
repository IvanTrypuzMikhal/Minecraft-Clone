#pragma once
#include "Chunk.h"
#include "ShaderProgram.h"
#include "TSQueue.h"
#include <unordered_set>
#include <utility>
#include <map>
#include <memory>


struct PairHash {
	size_t operator()(const std::pair<int, int>& p) const {
		return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
};

enum State
{
	NOT_CREATED,
	TERRAIN_READY,
	MESH_BUILDING,
	MESH_READY
};

struct ChunkState
{
	std::unique_ptr<Chunk> chunk;
	State state;
};

struct FinishedChunk 
{
	ChunkState chunkState;
	std::pair<int, int> coords;
};

struct ChunkPackage
{
	std::pair<int, int> coords;
	Chunk* center;
	Chunk* left;
	Chunk* right;
	Chunk* front;
	Chunk* back;
};

class World
{
public:
	World(ShaderProgram* shader) : m_shader{ shader }, m_terrainRunning{ true }, m_meshRunning{ true } {
		m_terrainThread = std::thread(&World::asyncTerrainLoading, this);
		m_meshThread = std::thread(&World::asyncMeshLoading, this);
	}
	~World();

	void renderWorld(const glm::mat4& projection, const glm::mat4& model);
	void update(const glm::vec3& position);
	bool checkNearbyChunks(int x, int z);

	// Threads
	void asyncTerrainLoading();
	void asyncMeshLoading();
private:
	void rebuildChunkMesh(int x, int z);


	std::map<std::pair<int, int>, ChunkState> m_chunks;
	ShaderProgram* m_shader;
	std::unordered_set<std::pair<int, int>, PairHash> m_requestedChunks;
	TSQueue<std::pair<int, int>> m_terrainQueue;
	TSQueue<ChunkPackage> m_meshQueue;

	
	// Terrain Thread
	std::thread m_terrainThread;
	bool m_terrainRunning;
	std::mutex m_terrainMutex;
	std::condition_variable m_terrainCond;
	TSQueue<FinishedChunk> m_finishedTerrainChunks;


	// Mesh Thread
	std::thread m_meshThread;
	bool m_meshRunning;
	std::mutex m_meshMutex;
	std::condition_variable m_meshCond;
	TSQueue<std::pair<int, int>> m_finishedMeshChunks;

	// Camera
	glm::vec3 m_cameraPosition;
};