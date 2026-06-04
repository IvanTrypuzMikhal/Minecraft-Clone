#pragma once
#include "TSQueue.h"
#include "ChunkPackage.h"

class TerrainThread
{
public:
	TerrainThread(ShaderProgram* shader, TerrainGenerator& terrain) : m_terrainRunning{ true }, m_shader{ shader }, m_terrain{ terrain } {
		m_terrainThread = std::thread(&TerrainThread::asyncTerrainLoading, this);
	}
	~TerrainThread() {
		m_terrainRunning = false;
		m_terrainCond.notify_one();
		m_terrainThread.join();
	}

	void asyncTerrainLoading();
	void notifyThread();

	TSQueue<std::pair<int, int>>& terrainQueue();
	TSQueue<FinishedChunk>& finishedTerrainChunks();

private:
	std::thread m_terrainThread;
	bool m_terrainRunning;
	std::mutex m_terrainMutex;
	std::condition_variable m_terrainCond;
	TSQueue<FinishedChunk> m_finishedTerrainChunks;
	TSQueue<std::pair<int, int>> m_terrainQueue;

	ShaderProgram* m_shader;
	TerrainGenerator& m_terrain;
};