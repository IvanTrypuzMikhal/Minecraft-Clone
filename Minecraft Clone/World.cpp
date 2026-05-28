#include "World.h"

World::~World() {
	m_running = false;
	m_cond.notify_one();
	m_workerThread.join();
}


void World::renderWorld(const glm::mat4& projection, const glm::mat4& model) {
	int positiveZ = m_cameraPosition.z + Globals::RENDER_RADIOUS;
	int negativeZ = m_cameraPosition.z - Globals::RENDER_RADIOUS;
	int positiveX = m_cameraPosition.x + Globals::RENDER_RADIOUS;
	int negativeX = m_cameraPosition.x - Globals::RENDER_RADIOUS;

	while (!m_finishedChunks.empty()) {
		FinishedChunk fc = m_finishedChunks.pop();
		fc.chunk->setBuffers();
		m_chunks.insert({ fc.coords, std::move(fc.chunk) });
		m_requestedChunks.erase(fc.coords);
		std::cout << "Popping chunk: " <<  fc.coords.first << " " << fc.coords.second << std::endl;
	}

	for (int z = negativeZ; z < positiveZ; z++) {
		for (int x = negativeX; x < positiveX; x++) {

			std::pair<int, int> coords = std::pair<int, int>(x, z);
			if (m_chunks.contains(coords)) {
				m_chunks[coords]->render(projection, glm::translate(model, glm::vec3(16.0f * x, 0.0f, 16.0f * z)));
			}
			else {
				if (!m_requestedChunks.contains(coords)) {
					m_requestedChunks.insert(coords);
					m_chunksQueue.push(coords);
				}
				if (!m_requestedChunks.empty()) m_cond.notify_one();
			}
		}
	}
}

void World::update(float x, float y, float z) {
	m_cameraPosition.x = std::floor(x / Globals::CHUNK_WIDTH);
	m_cameraPosition.y = std::floor(y / Globals::CHUNK_WIDTH);
	m_cameraPosition.z = std::floor(z / Globals::CHUNK_WIDTH);
}

void World::asyncChunkLoading() {
	std::cout << "Worker thread started!" << std::endl;

	while (m_running) {
		std::pair<int, int> coords;

		{
			std::unique_lock<std::mutex> lock(m_mutex);

			m_cond.wait(lock, [this] {
				return !m_chunksQueue.empty() || !m_running;
				});

			if (!m_running) break;

			coords = m_chunksQueue.pop();
		}
		std::cout << "Generating chunk: " << coords.first << " " << coords.second << std::endl;

		auto newChunk = std::make_unique<Chunk>(m_shader);
		m_finishedChunks.push({ std::move(newChunk), coords});
	}
}
