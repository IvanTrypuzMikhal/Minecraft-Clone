#include "World.h"
#include <Gameplay/Raycaster.h>
#include <chrono>


// Will need to take care of the nested if statements for them not to be too deep, maybe create a function for state handling.
void World::renderWorld(const glm::mat4& projection) {
	for (const auto& chunk : m_chunkManager.getRenderableChunks()) {
		std::pair<int, int> coords = chunk->getWorldPosition();

		AABB chunkAABB = {
			glm::vec3(coords.first * Globals::CHUNK_WIDTH, -Globals::CHUNK_HEIGHT, coords.second * Globals::CHUNK_WIDTH),
			glm::vec3((coords.first + 1) * Globals::CHUNK_WIDTH, -chunk->getMaxHeight() + 10, (coords.second + 1) * Globals::CHUNK_WIDTH)
		};

		if (m_frustum.isAABBInFrustum(chunkAABB)) {
			chunk->render(projection, getAmbientLightIntensity());
		}
	}
	/*
	const int positiveGenerationRadiousZ = static_cast<int>(m_cameraPosition.z + Globals::GENERATION_RADIOUS);
	const int negativeGenerationRadiousZ = static_cast<int>(m_cameraPosition.z - Globals::GENERATION_RADIOUS);
	const int positiveGenerationRadiousX = static_cast<int>(m_cameraPosition.x + Globals::GENERATION_RADIOUS);
	const int negativeGenerationRadiousX = static_cast<int>(m_cameraPosition.x - Globals::GENERATION_RADIOUS);
	
	// Chen in radious -+ x, -+ z which chunks are ready to render and which not
	for (int z = negativeGenerationRadiousZ; z < positiveGenerationRadiousZ; z++) {
		for (int x = negativeGenerationRadiousX; x < positiveGenerationRadiousX; x++) {
			std::pair<int, int> coords = std::pair<int, int>(x, z);
			
			if (m_chunks.contains(coords)) {
				if (m_chunks[coords].state == MESH_READY || m_chunks[coords].state == DIRTY) {
					AABB chunkAABB = {
						glm::vec3(x * Globals::CHUNK_WIDTH, -Globals::CHUNK_HEIGHT  , z * Globals::CHUNK_WIDTH),
						// - 10 to have a little bit of margin for the frustum culling to not cull chunks that are close to the camera but not in the frustum yet.
						glm::vec3((x + 1) * Globals::CHUNK_WIDTH, -m_chunks[coords].chunk->getMaxHeight() + 10 , (z + 1) * Globals::CHUNK_WIDTH)
					};

					if (m_frustum.isAABBInFrustum(chunkAABB)) {
						m_chunks[coords].chunk->render(projection, getAmbientLightIntensity());
					}
				}
			}
			else {
				// If not ready to render then we need to do several checks before enqueing it for terrain generation.
				// We first need to check if its in saving chunks, if it is then we need to rebuild the chunk from the deltas stored.
				// If its already saved in memory then we need to notify file thread to retreive a chunks deltas. Then we enqueue for terrain generation.
				// If not in saving chunks, not in memory and not requested then we just generate it as usual.
				if (!m_requestedChunks.contains(coords)) {
					if (m_savingChunks.contains(coords)) {
						// We rebuild the snapshot from the deltas stored in memory and enqueue it for terrain generation.
						auto it = m_savingChunks.find(coords);
						ChunkSnapshot snapshot;
						snapshot.coords = it->first;
						snapshot.deltas_counts.count = it->second.count;
						snapshot.deltas_counts.deltas = it->second.deltas;
						auto job = getTerrainGenerationTask(coords);
						if (job == nullptr) throw std::exception("BAD::TERRAIN::GENERATION::TASK::TERMINATING");
						m_threadPool.taskQueue().push(job);
						m_pendingDeltas.insert({ coords, snapshot });
					}
					else if (m_mainMemSavedChunks.contains(coords)) {
						m_fileIOThread.loadFromMemoryQueue().push(coords);
						m_threadPool.taskQueue().push(getTerrainGenerationTask(coords));
						m_fileIOThread.notifyThread();
					}
					else {
						m_requestedChunks.insert(coords);
						m_threadPool.taskQueue().push(getTerrainGenerationTask(coords));
					}
				}
			}
		}
	}
	*/
}

void World::updateWorldState() {
	/*
	// Check chunks with generated terrain
	checkChunksWithTerrain();
	// Check chunks with finished decoration
	checkFinishedChunksWithDecoration();
	// We check chunks with finished lighting
	checkFinishedChunksWithLighting();
	// We update each chunk with finished mesh to be ready for rendering
	checkFinishedChunksWithMesh();
	// Check chunks that are out of range to be freed from memory
	checkChunksToBeFreed();
	// Check chunks with finished loading from memory
	checkFinishedChunksLoadedFromMemory();
	*/
	m_chunkManager.update(m_cameraPosition);
}
/*
void World::checkChunksWithTerrain() {
	while (!m_finishedTerrainChunks.empty()) {

		FinishedChunk fc = m_finishedTerrainChunks.pop();
		std::pair<int, int> current = fc.coords;
		m_chunks.insert({ current, std::move(fc.chunkState) });
 		m_requestedChunks.erase(current);

		promoteChunk(fc.coords);
		for (const auto& neighbor : get8Neighbors(fc.coords)) {
			promoteChunk(neighbor);
		}
	}
}

void World::checkFinishedChunksWithDecoration() {
	while (!m_finishedDecoratedChunks.empty()) {
		std::pair<int, int> coords = m_finishedDecoratedChunks.pop();
		auto it = m_chunks.find(coords);
		if (it != m_chunks.end()) {
			if (it->second.state == DECORATING) {
				it->second.state = DECORATED;
				free(m_pendingDeltas[coords].deltas_counts.deltas);
				m_pendingDeltas.erase(coords);

				promoteChunk(coords);
 				for (const auto& neighbor : get8Neighbors(coords)) {
					promoteChunk(neighbor);
				}
			}
		}
	}
}

void World::checkFinishedChunksWithLighting() {
	while (!m_finishedLightingChunks.empty()) {
		std::pair<int, int> coords = m_finishedLightingChunks.pop();
		auto it = m_chunks.find(coords);
		if (it != m_chunks.end()) {
			if (it->second.state == CALCULATING_LIGHTING) {
				it->second.state = LIGHTING_READY;

				promoteChunk(coords);
				for (const auto& neighbor : get8Neighbors(coords)) {
					promoteChunk(neighbor);
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
			auto start = std::chrono::high_resolution_clock::now();

			it->second.chunk->swapMesh();
			it->second.chunk->setBuffers();
			if (it->second.state != DIRTY) {
				it->second.state = MESH_READY;
			}
		}
	}
}

void World::checkChunksToBeFreed() {
	int minX = static_cast<int>(m_cameraPosition.x - Globals::RENDER_RADIOUS) - 5;
	int maxX = static_cast<int>(m_cameraPosition.x + Globals::RENDER_RADIOUS) + 5;
	int minZ = static_cast<int>(m_cameraPosition.z - Globals::RENDER_RADIOUS) - 5;
	int maxZ = static_cast<int>(m_cameraPosition.z + Globals::RENDER_RADIOUS) + 5;

	// We cant delete chunks while iterating the map because it will cause undefined behavior, so we just store the chunks to be deleted and after iterating we delete them.
	std::vector<std::pair<int, int>> chunksToErase;

	for (const auto& [coords, chunkState] : m_chunks) {
		//std::cout << "Checking chunk " << x << "," << z << " for freeing.\n";
		
		if (coords.first >= minX && coords.first <= maxX && coords.second >= minZ && coords.second <= maxZ)	continue;
		
		// Just delete the chunk and free its memory. Not modifyed so it can be rebuilded again if needed.
		if (chunkState.state == MESH_READY ||
			chunkState.state == TERRAIN_READY ||
			chunkState.state == DECORATED ||
			chunkState.state == MESH_BUILDING) {
			chunksToErase.push_back(coords);
		}
		// If its in an other state fron dirty then we dont touch it. It has to be dirty to be saved to memory if we want to free it from RAM.
		else if (chunkState.state == DIRTY) {
			chunksToErase.push_back(coords);
			//std::cout << "Chunk " << coords.first << "," << coords.second << " is dirty, saving to memory and erasing from chunk map.\n";
			// Just for learning purposes I want to use C style memory management instead of using modern C++ ones.
			// So I can learn how to manage memory by myself and understand better how it works.

			// We create a "copy" of the chunks blocks (deltas) to be able to save it to memory and then free the chunk itself.
			// This way we dont need to wait for the IO thread to finish saving the chunk to free its memory and we can do it in parallel.
			// Also if the player comes back to the chunk before the chunk is saved we can just load the chunks data from memory.
			// When the chunk is saved to disk we can free the memory used for the snapshot.
			// I think this is a good approach for parllel computing and memory management.
			ChunkSnapshot snapshot;

			auto deltas = chunkState.chunk->getDeltasChanges();

			snapshot.coords = { coords.first, coords.second };
			snapshot.deltas_counts.deltas = (Delta*)malloc(deltas.size() * sizeof(Delta));
			if (!snapshot.deltas_counts.deltas) throw std::exception("BAD::MEMORY::ALLOCATION::TERMINATING");
			snapshot.deltas_counts.count = deltas.size();
			int i = 0;
			for (const auto& [index, blockType] : deltas) {
				Delta delta;
				delta.index = index;
				//std::cout << "Saving delta with index: " << index << " and block type: " << static_cast<int>(blockType) << std::endl;
				delta.blockType = static_cast<uint8_t>(blockType);
				snapshot.deltas_counts.deltas[i] = delta;
				i++;
			}

			m_fileIOThread.saveInMemoryQueue().push(snapshot);
			m_savingChunks.insert({ snapshot.coords, { snapshot.deltas_counts.deltas, deltas.size() } });
			m_fileIOThread.notifyThread();
		}
	}

	for (const auto& coords : chunksToErase) {
		m_chunks.erase(coords);
	}

	while (!m_fileIOThread.finishedFileIOChunks().empty()) {
		ChunkSnapshot snapshot = m_fileIOThread.finishedFileIOChunks().pop();
		m_savingChunks.erase(snapshot.coords);
		free(snapshot.deltas_counts.deltas);
		m_mainMemSavedChunks.insert(snapshot.coords);
	}
}

void World::checkFinishedChunksLoadedFromMemory() {
	while (!m_fileIOThread.finishedLoadQueue().empty()) {
		ChunkSnapshot snapshot = m_fileIOThread.finishedLoadQueue().pop();
		m_pendingDeltas[snapshot.coords] = snapshot;
	}
}
*/
void World::updateCamera(const glm::vec3& position, const Frustum& frustum, float worldTime) {
	m_cameraPosition.x = std::floor(position.x / Globals::CHUNK_WIDTH);
	m_cameraPosition.y = position.y;
	m_cameraPosition.z = std::floor(position.z / Globals::CHUNK_WIDTH);

	m_frustum = frustum;

	m_worldTime = worldTime;
}										

BlockType World::getBlockAt(int x, int y, int z) const {
	return m_chunkManager.getBlockAt(x, y, z);
	/*
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
	*/
}

void World::deleteBlock(BlockHit hit) {
	return m_chunkManager.deleteBlock(hit);
	/*
	std::pair<int, int> chunkPos = std::pair(hit.x >> 4, hit.z >> 4);

	auto it = m_chunks.find(chunkPos);
	if (it == m_chunks.end()) return;

	int localX = hit.x & 15;
	int localZ = hit.z & 15;
	int localY = -hit.y - 1;

	it->second.chunk->deleteBlock(localX, localY, localZ);

	if (checkNearbyChunksSameState(chunkPos, DECORATED)) {
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
	it->second.state = DIRTY;
	*/
}
/*
void World::enqueMeshByCoords(std::pair<int, int> chunkPos) {
	auto it = m_chunks.find({ chunkPos.first, chunkPos.second});
	if (it == m_chunks.end()) return;
	ChunkPackage package;
	package.coords = std::pair(chunkPos.first, chunkPos.second);
	package.center = it->second.chunk;
	getNearbyChunks(std::pair(chunkPos.first, chunkPos.second), package);
	m_threadPool.taskQueue().push(getMeshBuildingTask(package));
}
*/
void World::addBlock(BlockHit hit, BlockType type, const AABB& playerAABB) {
	return m_chunkManager.addBlock(hit, type, playerAABB);
	/*
	int globalX = hit.x;
	int globalY = -hit.y - 1;
	int globalZ = hit.z;

	if (hit.face == BlockFace::Top) globalY -= 1;
	else if (hit.face == BlockFace::Bottom) globalY += 1;
	else if (hit.face == BlockFace::Right) globalX += 1;
	else if (hit.face == BlockFace::Left) globalX -= 1;
	else if (hit.face == BlockFace::Front) globalZ += 1;
	else if (hit.face == BlockFace::Back) globalZ -= 1;

	if (playerAABB.intersects(AABB(glm::vec3((float)globalX, (float)globalY, (float)globalZ), glm::vec3((float)globalX + 1.05, (float)globalY, (float)globalZ + 1.05))) ||
		playerAABB.intersects(AABB(glm::vec3((float)globalX, (float)globalY + 1, (float)globalZ), glm::vec3((float)globalX + 1.05, (float)globalY + 1, (float)globalZ + 1.05)))) {
		return;
	}

	std::pair<int, int> chunkPos = std::pair(globalX >> 4, globalZ >> 4);

	auto it = m_chunks.find(chunkPos);
	if (it == m_chunks.end()) return;

	int localX = globalX & 15;;
	int localY = globalY;
	int localZ = globalZ & 15;

	it->second.chunk->addBlock(localX, localY, localZ, type);

	if (checkNearbyChunksSameState(chunkPos, DECORATED)) {

		ChunkPackage package;
		package.coords = chunkPos;
		package.center = it->second.chunk;
		getNearbyChunks(chunkPos, package);
		m_threadPool.taskQueue().push(getMeshBuildingTask(package));
	}
	it->second.state = DIRTY;
	*/
}
/*
void World::getNearbyChunks(std::pair<int, int> chunkPos, ChunkPackage& package) {
	 
	auto it = m_chunks.find({chunkPos.first - 1, chunkPos.second});
	if (it == m_chunks.end()) return;
	package.left = it->second.chunk;

	it = m_chunks.find({ chunkPos.first + 1, chunkPos.second });
	if (it == m_chunks.end()) return;
	package.right = it->second.chunk;

	it = m_chunks.find({ chunkPos.first, chunkPos.second + 1 });
	if (it == m_chunks.end()) return;
	package.front = it->second.chunk;

	it = m_chunks.find({ chunkPos.first, chunkPos.second - 1 });
	if (it == m_chunks.end()) return;
	package.back = it->second.chunk;

	// Diagonals
	it = m_chunks.find({ chunkPos.first - 1, chunkPos.second - 1 });
	if (it == m_chunks.end()) return;
	package.topLeft = it->second.chunk;

	it = m_chunks.find({ chunkPos.first + 1, chunkPos.second - 1 });
	if (it == m_chunks.end()) return;
	package.topRight = it->second.chunk;

	it = m_chunks.find({ chunkPos.first - 1, chunkPos.second + 1 });
	if (it == m_chunks.end()) return;
	package.bottomLeft = it->second.chunk;

	it = m_chunks.find({ chunkPos.first + 1, chunkPos.second + 1 });
	if (it == m_chunks.end()) return;
	package.bottomRight = it->second.chunk;
}
*/
// We dont pass the player position by reference because we want to floor it to get the block coordinates
// So we avoid modifing the player position outside of this function

void World::checkCollisionRadious(glm::vec3 position, const AABB& playerAABB, CollisionRes& res) const{

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
					res.collision = true;
					res.blockAABB = blockAABB;
					return;
				}
			}
		}
	}
	res.collision = false;
	res.blockAABB = AABB(glm::vec3(0), glm::vec3(0));
}

// We dont pass the player position by reference because we want to floor it to get the block coordinates
// So we avoid modifing the player position outside of this function

void World::getBlocksBellow(glm::vec3 position, std::vector<AABB>& blocksBellow) const{
	position.x = std::floor(position.x);
	position.y = std::floor(position.y);
	position.z = std::floor(position.z);

	//std::cout << "Player position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	for (int x = -1; x <= 1; x++) {
		for (int z = -1; z <= 1; z++) {
			glm::vec3 blockPos = position + glm::vec3(x, 1, z);
			if (getBlockAt(blockPos.x, blockPos.y, blockPos.z) == BlockType::Air) continue;
			AABB blockAABB(blockPos + glm::vec3(0.01f), blockPos + glm::vec3(0.99f));
			blocksBellow.push_back(blockAABB);
		}
	}
}

bool World::hasBlockBellow(AABB playerAABB, int yPos) const {
	//std::cout << "Checking if player has block bellow. Player AABB: min(" << playerAABB.min.x << ", " << playerAABB.min.y << ", " << playerAABB.min.z << ") max(" << playerAABB.max.x << ", " << playerAABB.max.y << ", " << playerAABB.max.z << ")" << std::endl;
	glm::vec3 position = glm::floor(playerAABB.max);
	position.y = yPos + 2;
	glm::vec3 blockPos = position;
	//std::cout << "Block position: " << blockPos.x << ", " << blockPos.y << ", " << blockPos.z << std::endl;
	if (getBlockAt(blockPos.x, blockPos.y, blockPos.z) != BlockType::Air) return true;

	return false;
}


float World::getAmbientLightIntensity() const {
	float ticks = fmod(m_worldTime, 24000.0f);
	if (ticks >= 0 && ticks < 12000) {
		return 1.0f;
	}

	if (ticks >= 12000 && ticks < 13670) {
		float factor = (float)(ticks - 12000) / (13670 - 12000);
		return 1.0f - (factor * (1.0f - 0.2666f));
	}

	if (ticks >= 13670 && ticks < 22330) {
		return 0.2666f;
	}

	if (ticks >= 22330 && ticks < 24000) {
		float factor = (float)(ticks - 22330) / (24000 - 22330);
		return 0.2666f + (factor * (1.0f - 0.2666f));
	}

	return 1.0f;
}
/*
std::function<void()> World::getTerrainGenerationTask(std::pair<int, int> coords) {
	return [this, coords]() {
		auto newChunk = std::make_shared<Chunk>(this->m_shader, coords.first, coords.second, this->m_terrain);
		ChunkState chunkState = { std::move(newChunk), TERRAIN_READY};

		this->m_finishedTerrainChunks.push({ std::move(chunkState), coords });
	};
}

std::function<void()> World::getMeshBuildingTask(ChunkPackage package) {
	return [this, package]() {
		package.center->buildMesh(package);
		this->m_finishedMeshChunks.push(package.coords);
	};
}

std::function<void()> World::getDecorationTask(ChunkPackage package) {
	return [this, package]() {
		package.center->generateTrees(package, m_terrain.getSeed());

		if (package.hasDeltas) {
			package.center->applyDeltas(package.snapshot);
		}

		this->m_finishedDecoratedChunks.push(package.coords);
	};
}

std::function<void()> World::getLightingTask(ChunkPackage package) {
	return [this, package]() {
		package.center->calculateLightingPropagation(package);
		this->m_finishedLightingChunks.push(package.coords);
	};
}

void World::promoteChunk(std::pair<int, int> coords) {
	auto neighbors = get8Neighbors(coords);

	auto it = m_chunks.find(coords);
	if (it == m_chunks.end()) return;

	State currentState = it->second.state;
	if (!checkNearbyChunksSameState(coords, currentState)) return;

	if (it->second.state == TERRAIN_READY) {
		it->second.state = DECORATING;
		ChunkPackage package;
		package.coords = coords;
		package.center = it->second.chunk;
		getNearbyChunks(coords, package);
		
		if (m_pendingDeltas.contains(coords)) {
			package.hasDeltas = true;
			package.snapshot = m_pendingDeltas[coords];
		}
		m_threadPool.taskQueue().push(getDecorationTask(package));		
	}
	else if (it->second.state == DECORATED) {
		it->second.state = CALCULATING_LIGHTING;

		ChunkPackage package;
		package.coords = coords;
		package.center = it->second.chunk;
		getNearbyChunks(coords, package);

		m_threadPool.taskQueue().push(getLightingTask(package));
	}
	else if (it->second.state == LIGHTING_READY) {
		it->second.state = MESH_BUILDING;
		enqueMeshByCoords(coords);
	}
}

bool World::checkNearbyChunksSameState(std::pair<int, int> coords, State state) {
	int x = coords.first;
	int z = coords.second;
	std::pair<int, int> targets[9] = {
		{x,   z},
		{x + 1, z}, {x - 1, z}, {x, z + 1}, {x, z - 1},
		{x + 1, z + 1}, {x - 1, z + 1}, {x + 1, z - 1}, {x - 1, z - 1}
	};

	for (const auto& target : targets) {
		auto it = m_chunks.find(target);
		if (it == m_chunks.end() || it->second.state < state) {
			return false;
		}
	}
	return true;
}

std::array<std::pair<int, int>, 8> World::get8Neighbors(std::pair<int, int> coords) const {
	int x = coords.first;
	int z = coords.second;

	return {
		std::pair{x + 1, z}, {x - 1, z}, {x, z + 1}, {x, z - 1},
		{x + 1, z + 1}, {x - 1, z + 1}, {x + 1, z - 1}, {x - 1, z - 1}
	};
}
*/