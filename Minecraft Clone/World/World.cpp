#include "World.h"
#include <Gameplay/Raycaster.h>

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
					m_terrainThread.terrainQueue().push(coords);
				}
				if (!m_requestedChunks.empty()) m_terrainThread.notifyThread();
			}
		}
	}
}

void World::updateWorldState() {
	// Check chunks with generated terrain
	checkChunksWithTerrain();
	// We update each chunk with finished mesh to be ready for rendering
	checkFinishedChunksWithMesh();
}
// TODO: Change approach to a buffer/queued based one.
// When a new chunk arrives check for all chunks stored in some buffer which need to be updated
// instead of checking in a 5x5 grid
void World::checkChunksWithTerrain() {
	while (!m_terrainThread.finishedTerrainChunks().empty()) {

		FinishedChunk fc = m_terrainThread.finishedTerrainChunks().pop();
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

					// This shit needs to be fixed
					ChunkPackage package;
					package.coords = coord;
					package.center = it->second.chunk.get();
					getNearbyChunks(coord, package);

					// Notify the mesh generation thread
					m_meshThread.meshQueue().push(package);
					m_meshThread.notifyThread();
				}
			}
		}
	}
}

void World::checkFinishedChunksWithMesh() {
	while (!m_meshThread.finishedMeshChunks().empty()) {
		std::pair<int, int> coords = m_meshThread.finishedMeshChunks().pop();

		auto it = m_chunks.find(coords);
		if (it != m_chunks.end()) {
			it->second.chunk->swapMesh();
			it->second.chunk->setBuffers();
			it->second.state = MESH_READY;
		}
	}
}

void World::updateCameraPosition(const glm::vec3& position) {
	m_cameraPosition.x = std::floor(position.x / Globals::CHUNK_WIDTH);
	m_cameraPosition.y = position.y;
	m_cameraPosition.z = std::floor(position.z / Globals::CHUNK_WIDTH);
}										

bool World::checkNearbyChunksTerrainReady(int x, int z) {
	std::pair<int, int> targets[9] = {
			{x,   z},
			{x + 1, z}, {x - 1, z}, {x, z + 1}, {x, z - 1},
			{x + 1, z + 1}, {x - 1, z + 1}, {x + 1, z - 1}, {x - 1, z - 1}
	};
	for (const auto& target : targets) {
		auto it = m_chunks.find(target);
		if (it == m_chunks.end() || it->second.state < TERRAIN_READY) {
			return false;
		}
	}
	return true;
}

bool World::checkNearbyChunksDecorationReady(int x, int z) {
	std::pair<int, int> targets[9] = {
		{x,   z},
		{x + 1, z}, {x - 1, z}, {x, z + 1}, {x, z - 1},
		{x + 1, z + 1}, {x - 1, z + 1}, {x + 1, z - 1}, {x - 1, z - 1}
	};

	for (const auto& target : targets) {
		auto it = m_chunks.find(target);
		if (it == m_chunks.end() || it->second.state < DECORATED) {
			return false;
		}
	}
	return true;
}

BlockType World::getBlockAt(int x, int y, int z) const {
	std::pair<int, int> chunkPos = std::pair(x >> 4, z >> 4);

	auto it = m_chunks.find(chunkPos);
	if (it == m_chunks.end()) {
		return BlockType::Air;
	}

	int localX = x & 15;
	int localZ = z & 15;
	int localY = y - 1;

	if (localY < 0 || localY >= 256) {
		return BlockType::Air;
	}

	return it->second.chunk->getBlock(localX, localY, localZ);
}

void World::mouseButtonProcessInput(Camera* cam, int button, int action, int mods) {
	std::cout << "Click" << std::endl;
	BlockHit hit;
	if (!Raycaster::traceRay(this, *cam, Globals::INTERACTION_DISTANCE, hit)) return;
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

		std::pair<int, int> chunkPos = std::pair(hit.x >> 4, hit.z >> 4);

		auto it = m_chunks.find(chunkPos);
		if (it == m_chunks.end()) return;
	
		int localX = hit.x & 15;
		int localZ = hit.z & 15;
		int localY = -hit.y - 1;

		it->second.chunk->deleteBlock(localX, localY, localZ);

		if (checkNearbyChunksDecorationReady(chunkPos.first, chunkPos.second)) {

			ChunkPackage package;
			package.coords = chunkPos;
			package.center = it->second.chunk.get();
			getNearbyChunks(chunkPos, package);
			m_meshThread.meshQueue().push(package);
			m_meshThread.notifyThread();
		}

		// If break cube in chunk edge also update neighbor chunks

		if (localX == 0) {
			auto it = m_chunks.find({chunkPos.first - 1, chunkPos.second});
			if (it == m_chunks.end()) return;
			ChunkPackage package;
			package.coords = std::pair(chunkPos.first - 1, chunkPos.second);
			package.center = it->second.chunk.get();
			getNearbyChunks(std::pair(chunkPos.first - 1, chunkPos.second), package);
			m_meshThread.meshQueue().push(package);
			m_meshThread.notifyThread();
		}

		else if (localX == 15) {
			auto it = m_chunks.find({ chunkPos.first + 1, chunkPos.second });
			if (it == m_chunks.end()) return;
			ChunkPackage package;
			package.coords = std::pair(chunkPos.first + 1, chunkPos.second);
			package.center = it->second.chunk.get();
			getNearbyChunks(std::pair(chunkPos.first + 1, chunkPos.second), package);
			m_meshThread.meshQueue().push(package);
			m_meshThread.notifyThread();
		}

		if (localZ == 0) {
			auto it = m_chunks.find({ chunkPos.first, chunkPos.second - 1});
			if (it == m_chunks.end()) return;
			ChunkPackage package;
			package.coords = std::pair(chunkPos.first, chunkPos.second - 1);
			package.center = it->second.chunk.get();
			getNearbyChunks(std::pair(chunkPos.first, chunkPos.second - 1), package);
			m_meshThread.meshQueue().push(package);
			m_meshThread.notifyThread();
		}

		else if (localZ == 15) {
			auto it = m_chunks.find({ chunkPos.first, chunkPos.second + 1 });
			if (it == m_chunks.end()) return;
			ChunkPackage package;
			package.coords = std::pair(chunkPos.first, chunkPos.second + 1);
			package.center = it->second.chunk.get();
			getNearbyChunks(std::pair(chunkPos.first, chunkPos.second + 1), package);
			m_meshThread.meshQueue().push(package);
			m_meshThread.notifyThread();
		}

	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {

		int globalX = hit.x;
		int globalY = - hit.y - 1;
		int globalZ = hit.z;

		if (hit.face == BlockFace::Top) globalY -= 1;
		else if (hit.face == BlockFace::Bottom) globalY += 1;
		else if (hit.face == BlockFace::Right) globalX += 1;
		else if (hit.face == BlockFace::Left) globalX -= 1;
		else if (hit.face == BlockFace::Front) globalZ += 1;
		else if (hit.face == BlockFace::Back) globalZ -= 1;

		std::pair<int, int> chunkPos = std::pair(globalX >> 4, globalZ >> 4);

		auto it = m_chunks.find(chunkPos);
		if (it == m_chunks.end()) return;

		int localX = globalX & 15;;
		int localY = globalY + 1;
		int localZ = globalZ & 15;

		it->second.chunk->addBlock(localX, localY - 1, localZ, BlockType::Dirt);

		if (checkNearbyChunksDecorationReady(chunkPos.first, chunkPos.second)) {

			ChunkPackage package;
			package.coords = chunkPos;
			package.center = it->second.chunk.get();
			getNearbyChunks(chunkPos, package);
			m_meshThread.meshQueue().push(package);
			m_meshThread.notifyThread();
		}
	}
}

void World::getNearbyChunks(std::pair<int, int> chunkPos, ChunkPackage& package) {
	package.left = m_chunks[{chunkPos.first - 1, chunkPos.second}].chunk.get();
	package.right = m_chunks[{chunkPos.first + 1, chunkPos.second}].chunk.get();
	package.front = m_chunks[{chunkPos.first, chunkPos.second + 1}].chunk.get();
	package.back = m_chunks[{chunkPos.first, chunkPos.second - 1}].chunk.get();
	// Diagonals
	package.topLeft = m_chunks[{chunkPos.first - 1, chunkPos.second - 1}].chunk.get();
	package.topRight = m_chunks[{chunkPos.first + 1, chunkPos.second - 1}].chunk.get();
	package.bottomLeft = m_chunks[{chunkPos.first - 1, chunkPos.second + 1}].chunk.get();
	package.bottomRight = m_chunks[{chunkPos.first + 1, chunkPos.second + 1}].chunk.get();
}