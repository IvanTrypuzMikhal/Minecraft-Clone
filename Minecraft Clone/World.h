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

struct FinishedChunk 
{
	std::unique_ptr<Chunk> chunk;
	std::pair<int, int> coords;
};

struct Position
{
	int x;
	int y;
	int z;
};

class World
{
public:
	World(ShaderProgram* shader) : m_shader{ shader }, m_running{true} {
		m_workerThread = std::thread(&World::asyncChunkLoading, this);
	}
	~World();

	void renderWorld(const glm::mat4& projection, const glm::mat4& model);
	void update(float x, float y, float z);
	void asyncChunkLoading();
private:

	std::map<std::pair<int, int>, std::unique_ptr<Chunk>> m_chunks;
	Position m_cameraPosition;
	ShaderProgram* m_shader;
	std::unordered_set<std::pair<int, int>, PairHash> m_requestedChunks;
	TSQueue<std::pair<int, int>> m_chunksQueue;
	TSQueue<FinishedChunk> m_finishedChunks;
	
	// Thread
	std::thread m_workerThread;
	bool m_running;  
	std::mutex m_mutex;
	std::condition_variable m_cond;

};