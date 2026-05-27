#include "World.h"

void World::renderWorld(const glm::mat4& projection, const glm::mat4& model) {
	int positiveZ = m_cameraPosition.z + Globals::RENDER_RADIOUS;
	int negativeZ = m_cameraPosition.z - Globals::RENDER_RADIOUS;
	int positiveX = m_cameraPosition.x + Globals::RENDER_RADIOUS;
	int negativeX = m_cameraPosition.x - Globals::RENDER_RADIOUS;
	std::cout << "Dibujando desde X: " << negativeX << " hasta " << positiveX << std::endl;
	for (int z = negativeZ; z < positiveZ; z++) {
		for (int x = negativeX; x < positiveX; x++) {

			std::pair<int, int> coords = std::pair<int, int>(x, z);
			if (m_chunks.contains(coords)) {
				m_chunks[coords]->render(projection, glm::translate(model, glm::vec3(16.0f * x, 0.0f, 16.0f * z)));
			}
			else {
				auto newChunk = std::make_unique<Chunk>(m_shader);
				newChunk->render(projection, glm::translate(model, glm::vec3(16.0f * x, 0.0f, 16.0f * z)));
				m_chunks.insert({ coords, std::move(newChunk) });
			}
		}
	}
}

void World::update(float x, float y, float z) {
	m_cameraPosition.x = std::floor(x / Globals::CHUNK_WIDTH);
	m_cameraPosition.y = std::floor(y / Globals::CHUNK_WIDTH);
	m_cameraPosition.z = std::floor(z / Globals::CHUNK_WIDTH);
}