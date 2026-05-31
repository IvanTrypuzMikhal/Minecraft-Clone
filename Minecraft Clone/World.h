#pragma once
#include "Chunk.h"
#include "ShaderProgram.h"
#include "TSQueue.h"
#include "TerrainGeneration.h"
#include <unordered_set>
#include <utility>
#include <map>
#include <memory>


struct PairHash {
	size_t operator()(const std::pair<int, int>& p) const {
		return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
};

enum State : unsigned char
{
	TERRAIN_READY,
	DECORATED,
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
	Chunk* topLeft;
	Chunk* topRight;
	Chunk* bottomLeft;
	Chunk* bottomRight;
};

class World
{
public:
	World(ShaderProgram* shader) : m_shader{ shader }, m_terrainRunning{ true }, m_meshRunning{ true } {
		m_terrainThread = std::thread(&World::asyncTerrainLoading, this);
		m_meshThread = std::thread(&World::asyncMeshLoading, this);
		m_terrain = TerrainGenerator();
	}
	~World();

	void renderWorld(const glm::mat4& projection);
	void updateCameraPosition(const glm::vec3& position);
	bool checkNearbyChunksTerrainReady(int x, int z);
	bool checkNearbyChunksDecorationReady(int x, int z);
	
	
	void updateWorldState();
	void checkChunksWithTerrain();
	void checkFinishedChunksWithMesh();


	// Threads
	void asyncTerrainLoading();
	void asyncMeshLoading();

private:


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

	// Terrain
	TerrainGenerator m_terrain;
};