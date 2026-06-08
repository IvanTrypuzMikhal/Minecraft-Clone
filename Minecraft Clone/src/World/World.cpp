#include "World.h"
#include <Gameplay/Raycaster.h>

void World::renderWorld(const glm::mat4& projection) {
	const int positiveZ = static_cast<int>(m_cameraPosition.z + Globals::RENDER_RADIOUS);
	const int negativeZ = static_cast<int>(m_cameraPosition.z - Globals::RENDER_RADIOUS);
	const int positiveX = static_cast<int>(m_cameraPosition.x + Globals::RENDER_RADIOUS);
	const int negativeX = static_cast<int>(m_cameraPosition.x - Globals::RENDER_RADIOUS);
	
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

					ChunkPackage package;
					getNearbyChunks(coord, package);
					Chunk* left = m_chunks[{coord.first - 1, coord.second}].chunk.get();
					Chunk* right = m_chunks[{coord.first + 1, coord.second}].chunk.get();
					Chunk* front = m_chunks[{coord.first, coord.second + 1}].chunk.get();
					Chunk* back = m_chunks[{coord.first, coord.second - 1}].chunk.get();

					it->second.chunk->generateTrees(package);
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

					// If every chunk its redy then we build mesh around the center chunk and equeue it for rendering
					it->second.state = MESH_BUILDING;
					enqueMeshByCoords(coord);
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
	int localY = y;

	if (localY < 0 || localY >= 256) {
		return BlockType::Air;
	}
	//std::cout << "Block at: " << localX << ", " << localY << ", " << localZ << std::endl;
	return it->second.chunk->getBlock(localX, localY, localZ);
}

void World::deleteBlock(BlockHit hit) {
	std::pair<int, int> chunkPos = std::pair(hit.x >> 4, hit.z >> 4);

	auto it = m_chunks.find(chunkPos);
	if (it == m_chunks.end()) return;

	int localX = hit.x & 15;
	int localZ = hit.z & 15;
	int localY = -hit.y - 1;

	it->second.chunk->deleteBlock(localX, localY, localZ);

	if (checkNearbyChunksDecorationReady(chunkPos.first, chunkPos.second)) {
		enqueMeshByCoords(chunkPos);
	}

	// If break cube in chunk edge also update neighbor chunks

	if (localX == 0) {
		enqueMeshByCoords({ chunkPos.first - 1, chunkPos.second });
	}

	else if (localX == 15) {
		enqueMeshByCoords({ chunkPos.first + 1, chunkPos.second });
	}

	if (localZ == 0) {
		enqueMeshByCoords({ chunkPos.first, chunkPos.second - 1 });
	}

	else if (localZ == 15) {
		enqueMeshByCoords({ chunkPos.first, chunkPos.second + 1 });
	}
}

void World::enqueMeshByCoords(std::pair<int, int> chunkPos) {
	auto it = m_chunks.find({ chunkPos.first, chunkPos.second});
	if (it == m_chunks.end()) return;
	ChunkPackage package;
	package.coords = std::pair(chunkPos.first, chunkPos.second);
	package.center = it->second.chunk.get();
	getNearbyChunks(std::pair(chunkPos.first, chunkPos.second), package);
	m_meshThread.meshQueue().push(package);
	m_meshThread.notifyThread();
}

void World::addBlock(BlockHit hit, BlockType type, const AABB& playerAABB) {

	int globalX = hit.x;
	int globalY = -hit.y - 1;
	int globalZ = hit.z;

	if (hit.face == BlockFace::Top) globalY -= 1;
	else if (hit.face == BlockFace::Bottom) globalY += 1;
	else if (hit.face == BlockFace::Right) globalX += 1;
	else if (hit.face == BlockFace::Left) globalX -= 1;
	else if (hit.face == BlockFace::Front) globalZ += 1;
	else if (hit.face == BlockFace::Back) globalZ -= 1;

	if (playerAABB.intersects(AABB(glm::vec3((float)globalX, (float)globalY, (float)globalZ), glm::vec3((float)globalX + 1.01, (float)globalY, (float)globalZ + 1.01))) ||
		playerAABB.intersects(AABB(glm::vec3((float)globalX, (float)globalY + 1, (float)globalZ), glm::vec3((float)globalX + 1.01, (float)globalY + 1, (float)globalZ + 1.01)))) {
		std::cout << "Cannot place block, player is intersecting the block's AABB." << std::endl;
		return;
	}

	std::pair<int, int> chunkPos = std::pair(globalX >> 4, globalZ >> 4);

	auto it = m_chunks.find(chunkPos);
	if (it == m_chunks.end()) return;

	int localX = globalX & 15;;
	int localY = globalY;
	int localZ = globalZ & 15;

	it->second.chunk->addBlock(localX, localY, localZ, type);

	if (checkNearbyChunksDecorationReady(chunkPos.first, chunkPos.second)) {

		ChunkPackage package;
		package.coords = chunkPos;
		package.center = it->second.chunk.get();
		getNearbyChunks(chunkPos, package);
		m_meshThread.meshQueue().push(package);
		m_meshThread.notifyThread();
	}
}

void World::getNearbyChunks(std::pair<int, int> chunkPos, ChunkPackage& package) {
	 
	auto it = m_chunks.find({chunkPos.first - 1, chunkPos.second});
	if (it == m_chunks.end()) return;
	package.left = it->second.chunk.get();

	it = m_chunks.find({ chunkPos.first + 1, chunkPos.second });
	if (it == m_chunks.end()) return;
	package.right = it->second.chunk.get();

	it = m_chunks.find({ chunkPos.first, chunkPos.second + 1 });
	if (it == m_chunks.end()) return;
	package.front = it->second.chunk.get();

	it = m_chunks.find({ chunkPos.first, chunkPos.second - 1 });
	if (it == m_chunks.end()) return;
	package.back = it->second.chunk.get();

	// Diagonals
	it = m_chunks.find({ chunkPos.first - 1, chunkPos.second - 1 });
	if (it == m_chunks.end()) return;
	package.topLeft = it->second.chunk.get();

	it = m_chunks.find({ chunkPos.first + 1, chunkPos.second - 1 });
	if (it == m_chunks.end()) return;
	package.topRight = it->second.chunk.get();

	it = m_chunks.find({ chunkPos.first - 1, chunkPos.second + 1 });
	if (it == m_chunks.end()) return;
	package.bottomLeft = it->second.chunk.get();

	it = m_chunks.find({ chunkPos.first + 1, chunkPos.second + 1 });
	if (it == m_chunks.end()) return;
	package.bottomRight = it->second.chunk.get();
}

bool World::checkCollisionRadious(glm::vec3 position, AABB playerAABB) const{

	position.x = std::floor(position.x);
	position.y = std::floor(position.y);
	position.z = std::floor(position.z);
	
	for (int x = -1; x <= 1; x++) {
		for(int y = -1; y <= 1; y++) {
			for(int z = -1; z <= 1; z++) {
				glm::vec3 blockPos = position + glm::vec3(x, y, z);
				
				if (getBlockAt(blockPos.x, blockPos.y, blockPos.z) == BlockType::Air) continue;
				
				AABB blockAABB(blockPos, blockPos + glm::vec3(1.0f));
				
				if (playerAABB.intersects(blockAABB)) {
					return true;
				}
			}
		}
	}
	return false;
}