#include "TerrainThread.h"
#include <chrono>

void TerrainThread::asyncTerrainLoading() {
	std::cout << "Terrain thread started!" << std::endl;

	while (m_terrainRunning) {
		std::pair<int, int> coords;

		{
			std::unique_lock<std::mutex> lock(m_terrainMutex);

			m_terrainCond.wait(lock, [this] {
				return !m_terrainQueue.empty() || !m_terrainRunning;
				});

			if (!m_terrainRunning) break;

			coords = m_terrainQueue.pop();
			//std::cout << "Creating terrain for chunk: " << coords.first << " " << coords.second << std::endl;

		}
		auto start = std::chrono::high_resolution_clock::now();

		auto newChunk = std::make_shared<Chunk>(m_shader, coords.first, coords.second, m_terrain);

		auto end = std::chrono::high_resolution_clock::now();
		float ms = std::chrono::duration<float, std::milli>(
			std::chrono::high_resolution_clock::now() - start).count();

		ChunkState chunkState = { std::move(newChunk), TERRAIN_READY, ms };
		m_finishedTerrainChunks.push({ std::move(chunkState), coords });
	}
}


void TerrainThread::notifyThread() {
	m_terrainCond.notify_one();
}
TSQueue<std::pair<int, int>>& TerrainThread::terrainQueue() {
	return m_terrainQueue;
}
TSQueue<FinishedChunk>& TerrainThread::finishedTerrainChunks() {
	return m_finishedTerrainChunks;
}
