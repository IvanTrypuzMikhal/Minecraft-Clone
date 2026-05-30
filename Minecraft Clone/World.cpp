#include "World.h"

World::~World() {
	m_terrainRunning = false;
	m_terrainCond.notify_one();
	m_terrainThread.join();

	m_meshRunning = false;
	m_meshCond.notify_one(); 
	m_meshThread.join();     
}


// TODO: Rewrite this behemoth
void World::renderWorld(const glm::mat4& projection) {
	int positiveZ = static_cast<int>(m_cameraPosition.z + Globals::RENDER_RADIOUS);
	int negativeZ = static_cast<int>(m_cameraPosition.z - Globals::RENDER_RADIOUS);
	int positiveX = static_cast<int>(m_cameraPosition.x + Globals::RENDER_RADIOUS);
	int negativeX = static_cast<int>(m_cameraPosition.x - Globals::RENDER_RADIOUS);
	
	// Chen in radious -+ x, -+ z which chunks are ready to render and which not
	for (int z = negativeZ; z < positiveZ; z++) {
		for (int x = negativeX; x < positiveX; x++) {
			std::pair<int, int> coords = std::pair<int, int>(x, z);
			if (m_chunks.contains(coords)) {
				if (m_chunks[coords].state == MESH_READY) {
					m_chunks[coords].chunk->render(projection);
				}
			}
			else {
				// If not ready to render then we enqueue the chunk to generate its terrain
				if (!m_requestedChunks.contains(coords)) {
					m_requestedChunks.insert(coords);
					m_terrainQueue.push(coords);
				}
				if (!m_requestedChunks.empty()) m_terrainCond.notify_one();
			}
		}
	}
}

void World::asyncTerrainLoading() {
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
			std::cout << "Creating terrain for chunk: " << coords.first << " " << coords.second << std::endl;

		}

		auto newChunk = std::make_unique<Chunk>(m_shader, coords.first, coords.second);
		ChunkState chunkState = { std::move(newChunk), TERRAIN_READY };
		m_finishedTerrainChunks.push({ std::move(chunkState), coords});
	}
}

void World::asyncMeshLoading() {
	std::cout << "Mesh thread started!" << std::endl;
	while (m_meshRunning) {
		ChunkPackage package;

		{
			std::unique_lock<std::mutex> lock(m_meshMutex);

			m_meshCond.wait(lock, [this] {
				return !m_meshQueue.empty() || !m_meshRunning;
				});

			if (!m_meshRunning) break;

			package = m_meshQueue.pop();
			std::cout << "Creating mesh around chunk: " << package.coords.first << " " << package.coords.second << std::endl;
		}
		package.center->buildMesh(package.left, package.right, package.front, package.back);
		m_finishedMeshChunks.push(package.coords);
	}
}

void World::updateWorldState() {
	// Check chunks with generated terrain
	checkChunksWithTerrain();
	// We update each chunk with finished mesh to be ready for rendering
	checkFinishedChunksWithMesh();
}

void World::checkChunksWithTerrain() {
	while (!m_finishedTerrainChunks.empty()) {

		FinishedChunk fc = m_finishedTerrainChunks.pop();
		std::pair<int, int> current = fc.coords;

		// Add it to the chunk map
		m_chunks.insert({ current, std::move(fc.chunkState) });
		m_requestedChunks.erase(fc.coords);

		// Retreive its neighbors in a 5x5 grid
		std::pair<int, int> candidatos[25];
		for (int dz = -2; dz <= 2; dz++) {
			for (int dx = -2; dx <= 2; dx++) {
				// Index is calculated as: 
				//int index = (dz + dx + 4) + 4 * (dz + 2);
				int index = 5 * dz + dx + 12;
				candidatos[index] = { current.first + dx, current.second + dz };
			}
		}

		// For each candidate chunk to be decorated
		for (const auto& coord : candidatos) {
			auto it = m_chunks.find(coord);

			// We check that its ready for mesh generation and its neighbors
			if (it != m_chunks.end() && it->second.state == TERRAIN_READY) {
				if (checkNearbyChunksTerrainReady(coord.first, coord.second)) {

					Chunk* left = m_chunks[{coord.first - 1, coord.second}].chunk.get();
					Chunk* right = m_chunks[{coord.first + 1, coord.second}].chunk.get();
					Chunk* front = m_chunks[{coord.first, coord.second + 1}].chunk.get();
					Chunk* back = m_chunks[{coord.first, coord.second - 1}].chunk.get();

					it->second.chunk->generateTrees(left, right, front, back);
					it->second.state = DECORATED;
				}
			}
		}

		// For each candidate chunk to be rendered
		for (const auto& coord : candidatos) {
			auto it = m_chunks.find(coord);

			// We check that its ready for mesh generation and its neighbors
			if (it != m_chunks.end() && it->second.state == DECORATED) {
				if (checkNearbyChunksDecorationReady(coord.first, coord.second)) {

					// If every chunk its redy then we build mesh around the center chunk
					it->second.state = MESH_BUILDING;

					ChunkPackage package;
					package.coords = coord;
					package.center = it->second.chunk.get();
					package.left = m_chunks[{coord.first - 1, coord.second}].chunk.get();
					package.right = m_chunks[{coord.first + 1, coord.second}].chunk.get();
					package.front = m_chunks[{coord.first, coord.second + 1}].chunk.get();
					package.back = m_chunks[{coord.first, coord.second - 1}].chunk.get();

					// Notify the mesh generation thread
					m_meshQueue.push(package);
					m_meshCond.notify_one();
				}
			}
		}

	}
}

void World::checkFinishedChunksWithMesh() {
	while (!m_finishedMeshChunks.empty()) {
		std::pair<int, int> coords = m_finishedMeshChunks.pop();

		auto it = m_chunks.find(coords);
		if (it != m_chunks.end()) {
			it->second.chunk->setBuffers();
			it->second.state = MESH_READY;
		}
	}
}

void World::updateCameraPosition(const glm::vec3& position) {
	m_cameraPosition.x = std::floor(position.x / Globals::CHUNK_WIDTH);
	m_cameraPosition.y = std::floor(position.y / Globals::HEIGHT);
	m_cameraPosition.z = std::floor(position.z / Globals::CHUNK_WIDTH);
}										

bool World::checkNearbyChunksTerrainReady(int x, int z) {
	std::pair<int, int> targets[5] = { {x, z}, {x + 1, z}, {x - 1, z}, {x, z + 1}, {x, z - 1} };

	for (const auto& target : targets) {
		auto it = m_chunks.find(target);
		if (it == m_chunks.end() || it->second.state < TERRAIN_READY) {
			return false;
		}
	}
	return true;
}

bool World::checkNearbyChunksDecorationReady(int x, int z) {
	std::pair<int, int> targets[5] = { {x, z}, {x + 1, z}, {x - 1, z}, {x, z + 1}, {x, z - 1} };

	for (const auto& target : targets) {
		auto it = m_chunks.find(target);
		if (it == m_chunks.end() || it->second.state < DECORATED) {
			return false;
		}
	}
	return true;
}