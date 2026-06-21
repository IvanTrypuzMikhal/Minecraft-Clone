#include "Chunk.h"
#include "ChunkPackage.h"

static BlockType getBlockType(int y, int surfaceY);
static std::pair<int, int> getAtlasCoordinates(BlockType type, BlockFace face);
uint32_t packVertex(float x, float y, float z, float u, float v, int textureId, int ao);
uint32_t packVertexAttributes(uint8_t lighting);

// TODO: Refactor chunkTarget to be a func.

// When creating a new chunk we first fill the chunk with the correct blocks based on the terrain generator in !WORLD COORDINATES!
Chunk::Chunk(const std::shared_ptr<ShaderProgram> shader, int x, int z, const TerrainGenerator& terrain) : m_shader{ shader }, m_worldPosition{x, z} {
	fillBlocks(terrain);
}

void Chunk::setBuffers() {
	std::vector<VertexAttribute> block{ { .index = 0, .size = 1, .offset = 0, .type = GL_UNSIGNED_INT }, {.index = 1, .size = 1, .offset = 1, .type = GL_UNSIGNED_INT } };
	m_vbo = std::make_unique<Vbo>();
	m_vao = std::make_unique<Vao>(m_vbo->get(), 2, block);
	m_vbo->upload(m_mesh);
	m_vertexCount = static_cast<unsigned int>(m_mesh.size() / 2);

	m_mesh.clear();
	m_mesh.shrink_to_fit();
}

// !WE WILL PASS THE WORLD POSITION OF THE CHUNK ONLY HERE!
void Chunk::render(const glm::mat4& projection, float ambientLightIntensity) {
	m_shader->use();
	m_shader->setMat4("projection", projection);
	glm::vec3 chunkPos(
		static_cast<float>(m_worldPosition.first * Globals::CHUNK_WIDTH),
		0.0f,
		static_cast<float>(m_worldPosition.second * Globals::CHUNK_WIDTH)
	);
	m_shader->setVec3("chunkPos", chunkPos);
	m_shader->setFloat("timeLightFactor", ambientLightIntensity);
	m_vao->use();
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}

// Here we build the mesh for the chunk in !LOCAL COORDINATES! 
void Chunk::buildMesh(const ChunkPackage& chunkPackage)

{
	m_buildMesh.clear();
	for (int x = 0; x < Globals::CHUNK_WIDTH; x++) {
		for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
			for (int z = 0; z < Globals::CHUNK_WIDTH; z++) {
				const BlockType type = m_blocks[x][y][z];
				if (type == BlockType::Air) continue;

				for (const auto& cubeData : CubeData::CUBE_FACES) {

					if (isAir(cubeData.dx + x, cubeData.dy + y, cubeData.dz + z, chunkPackage)) {
						for (int i = 0; i < 6; i++) {
							// Also here we can use [] instead of .at() because we have already checked the bounds in isAir function
							const float vx = x + cubeData.vertices[i].x;
							const float vy = y + cubeData.vertices[i].y; // Change to possitive to avoid packing wrong values
							const float vz = z + cubeData.vertices[i].z; // Change to possitive to avoid packing wrong values
							
							const float u = cubeData.vertices[i].u;
							const float v = cubeData.vertices[i].v;
							
							auto [textureX, textureY] = getAtlasCoordinates(type, cubeData.faceDirection);
							const int textureId = (textureY * 4) + textureX;


							const int ao1 = !isAir(x + cubeData.vertices[i].side1.x, y - cubeData.vertices[i].side1.y, z - cubeData.vertices[i].side1.z, chunkPackage);
							const int ao2 = !isAir(x + cubeData.vertices[i].side2.x, y - cubeData.vertices[i].side2.y, z - cubeData.vertices[i].side2.z, chunkPackage);
							const int ao3 = !isAir(x + cubeData.vertices[i].diagonal.x, y - cubeData.vertices[i].diagonal.y, z - cubeData.vertices[i].diagonal.z, chunkPackage);
							
							const int ao = (ao1 && ao2) ? 0 : 3 - (ao1 + ao2 + ao3);
							
							const uint32_t packedVertex = packVertex(vx, vy, vz, u, v, textureId, ao);
							const uint32_t packedAttributes = packVertexAttributes(m_lighting[x][y][z]);
							pushVertex(packedVertex, packedAttributes);
						}
					}

				}
			}
		}
	}
}

bool Chunk::castsAO(BlockType type) {
	switch (type) {
	case BlockType::Air:
	case BlockType::OakLeaf:
		return false;
	default:
		return true; 
	}
}

bool Chunk::isAir(int x, int y, int z, const ChunkPackage& chunkPackage) {

	if (y < 0 || y >= Globals::CHUNK_HEIGHT) return true;
	
	int chunkX = 0; 
	int chunkZ = 0; 

	if (x < 0) {
		chunkX = -1;
		x += Globals::CHUNK_WIDTH;
	}
	else if (x >= Globals::CHUNK_WIDTH) {
		chunkX = 1;
		x -= Globals::CHUNK_WIDTH;
	}

	if (z < 0) {
		chunkZ = -1;
		z += Globals::CHUNK_WIDTH;
	}
	else if (z >= Globals::CHUNK_WIDTH) {
		chunkZ = 1;
		z -= Globals::CHUNK_WIDTH;
	}
	
	std::shared_ptr<Chunk> targetChunk = nullptr;

	if (chunkX == 0 && chunkZ == 0)   targetChunk = shared_from_this();
	else if (chunkX == -1 && chunkZ == 0)  targetChunk = chunkPackage.left;
	else if (chunkX == 1 && chunkZ == 0)   targetChunk = chunkPackage.right;
	else if (chunkX == 0 && chunkZ == -1)  targetChunk = chunkPackage.back;
	else if (chunkX == 0 && chunkZ == 1)   targetChunk = chunkPackage.front;
	
	else if (chunkX == -1 && chunkZ == -1) targetChunk = chunkPackage.topLeft;
	else if (chunkX == 1 && chunkZ == -1)  targetChunk = chunkPackage.topRight;
	else if (chunkX == -1 && chunkZ == 1)  targetChunk = chunkPackage.bottomLeft;
	else if (chunkX == 1 && chunkZ == 1)   targetChunk = chunkPackage.bottomRight;

	if (!targetChunk) return true; 

	// [] is faster than .at() and we have already checked the bounds so it is safe to use
	return targetChunk->m_blocks[x][y][z] == BlockType::Air;
}

std::vector<uint32_t> Chunk::getMesh(){
	return m_mesh;
}


// Dont remember shit how does this work
// Also i have to change the inverted Y axis in my world. Too many problems
void Chunk::generateTrees(const ChunkPackage& chunkPackage, int seed) {
	
	std::random_device rd;
	
	int32_t chunkX = chunkPackage.coords.first;
	int32_t chunkZ = chunkPackage.coords.second;
	//std::cout << "Generating trees for chunk: " << chunkPackage.coords.first << ", " << chunkPackage.coords.second << std::endl;
	uint64_t chunkSeed = seed;
	chunkSeed ^= static_cast<uint64_t>(chunkX) * 0x451A4C37928A1499ULL;
	chunkSeed ^= static_cast<uint64_t>(chunkZ) * 0xB84B9A7A64C26815ULL;

	std::mt19937 gen(chunkSeed);


	std::uniform_int_distribution<> location(0, Globals::CHUNK_WIDTH-1);
	std::uniform_int_distribution<> randomHeight(5, 7);

	const int treeX = location(gen);
	int treeY = 0;
	const int treeZ = location(gen);
	const int treeHeight = randomHeight(gen);

	for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
		if (getBlock(treeX, y, treeZ) != BlockType::Air) {
			treeY = y - 1;
			break;
		}
	}

	for (int y = 0; y < treeHeight; y++) {
		m_blocks.at(treeX).at(treeY - y).at(treeZ) = BlockType::OakLog;
	}

	const int leafStartHeight = treeY - 7;
	for (int y = 0; y < 4; y++) {

		const int radius = (y >= 2) ? 2 : 1;

		for (int x = -radius; x <= radius; x++) {
			for (int z = -radius; z <= radius; z++) {

				if ((radius == 2 && std::abs(x) == 2 && std::abs(z) == 2) ||(radius == 1 && std::abs(x) == 1 && std::abs(z) == 1 && y == 0)) continue;

				int leafX = treeX + x;
				int leafY = leafStartHeight + y;
				int leafZ = treeZ + z;

				//if (leafY > m_maxHeight) m_maxHeight = leafY;

				std::shared_ptr<Chunk> targetChunk = shared_from_this();


				//TODO: Need to check spawned near the vertice of a chunk
				// Either dont spawn trees on those locations or pass the other 4 neigbor chunks
				if (leafX >= 16) {
					leafX = leafX - 16;
					targetChunk = chunkPackage.right;
				}
				
				else if (leafX < 0) {
					leafX = leafX + 16;
					targetChunk = chunkPackage.left;
				}

				if (leafZ >= 16) {
					leafZ = leafZ - 16;
					if (targetChunk == shared_from_this()) targetChunk = chunkPackage.front;
					else targetChunk = nullptr;
				}

				else if (leafZ < 0) {
					leafZ = leafZ + 16;
					if (targetChunk == shared_from_this()) targetChunk = chunkPackage.back;
					else targetChunk = nullptr;
				}

				if (targetChunk != nullptr) {
					if (targetChunk->m_blocks.at(leafX).at(leafY).at(leafZ) == BlockType::Air) {
						targetChunk->m_blocks.at(leafX).at(leafY).at(leafZ) = BlockType::OakLeaf;
					}
				}
			}
		}
	}
}

void Chunk::fillBlocks(const TerrainGenerator& terrain) {
	for (int x = 0; x < Globals::CHUNK_WIDTH; x++) {
		for (int z = 0; z < Globals::CHUNK_WIDTH; z++) {
			const int worldX = m_worldPosition.first * Globals::CHUNK_WIDTH + x;
			const int worldZ = m_worldPosition.second * Globals::CHUNK_WIDTH + z;
			const int surfaceY = terrain.getHeight(worldX, worldZ);
			if (surfaceY > m_maxHeight) m_maxHeight = surfaceY;
			// Not the best solution becouse what theres some sort of cliff and the first block gets the light level of 15 and the blocks under it get the light level of 0?
			// There should be a better solution to calculate the light level based on the blocks around it, but for now this will do.


			bool firstBlock = false;
			for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
				BlockType blockType = getBlockType(y, surfaceY);

				// Here we are iterating under the bounds so we can use [] instead of .at() to avoid the overhead of bounds checking
				m_blocks[x][y][z] = blockType;
				
				// After we found the first solid block we set every other block light to level 0
				uint8_t lightLevel = firstBlock ? 0 : 15;
				// For now we will set the indirect light level to 0 for every block, but we'll change this later
				lightLevel |= 0 << 4;
				m_lighting[x][y][z] = lightLevel;
				
				// Then we set the firstBlock flag to true so we know that we have found the first solid block and we can set the light level to 0 for every other block
				if (blockType != BlockType::Air && !firstBlock) firstBlock = true;
			}
		}
	}
}

void Chunk::pushVertex(uint32_t packedVertex, uint32_t packedAttributes) {
	m_buildMesh.push_back(packedVertex);
	m_buildMesh.push_back(packedAttributes);
}

uint32_t packVertex(float x, float y, float z, float u, float v, int textureId, int ao) {
	const uint32_t intX = static_cast<uint32_t>(x);
	const uint32_t intY = static_cast<uint32_t>(y);
	const uint32_t intZ = static_cast<uint32_t>(z);
	const uint32_t intU = static_cast<uint32_t>(u);
	const uint32_t intV = static_cast<uint32_t>(v);
	const uint32_t intTextureId = static_cast<uint32_t>(textureId);
	const uint32_t intAo = static_cast<uint32_t>(ao);
	
	uint32_t packedVertex{ 0 };
	packedVertex |= (intX & 0x1F);					// 5 bits
	packedVertex |= (intZ & 0x1F)  << 5;			// 5 bits
	packedVertex |= (intY & 0x1FF) << 10;			// 9 bits
	packedVertex |= (intU & 0x1)   << 19;			// 1 bit
	packedVertex |= (intV & 0x1)   << 20;			// 1 bit
	packedVertex |= (intTextureId & 0xFF) << 21;	// 8 bits
	packedVertex |= (intAo & 0x3) << 29;			// 2 bits	
	return packedVertex;
}

uint32_t packVertexAttributes(uint8_t lighting) {
	uint32_t packedAttributes{ 0 };

	packedAttributes |= lighting;			
	return packedAttributes;
}

static BlockType getBlockType(int y, int surfaceY) {

	if (y < surfaceY)							return BlockType::Air;
	else if (y == surfaceY) 					return BlockType::Grass;
	else if (y < surfaceY + 4) 					return BlockType::Dirt;
	else if (y > Globals::CHUNK_HEIGHT - 2) 	return BlockType::Bedrock;
	else 										return BlockType::Stone;

	return BlockType::Air;
}

static std::pair<int, int> getAtlasCoordinates(BlockType type, BlockFace face) {
	switch (type) {
	case BlockType::Bedrock:
		return std::pair(2,1);

	case BlockType::Stone:
		return std::pair(2, 0);

	case BlockType::Dirt:
		return std::pair(0, 1);

	case BlockType::Grass:
		if (face == BlockFace::Top) return std::pair(1,0);
		if (face == BlockFace::Bottom) return std::pair(0, 1);
		return std::pair(0, 0);

	case BlockType::OakLeaf:
		return std::pair(0, 3);

	case BlockType::OakLog:
		if (face == BlockFace::Top || face == BlockFace::Bottom) return std::pair(1,3);
		return std::pair(0, 2);
	}
	
	return std::pair(2, 3);
}

BlockType Chunk::getBlock(int x, int y, int z) const {
	return m_blocks.at(x).at(y).at(z);
}

const std::unordered_map<uint16_t, BlockType>& Chunk::getDeltasChanges() const {
	return m_deltasChanges;
}

void Chunk::deleteBlock(int x, int y, int z) {
	m_blocks.at(x).at(y).at(z) = BlockType::Air;
	addDelta(x, y, z, BlockType::Air);
	calculateFirstBlock(x, z);
}

void Chunk::addBlock(int x, int y, int z, BlockType blockType) {
	m_blocks.at(x).at(y).at(z) = blockType;
	addDelta(x, y, z, blockType);
	calculateFirstBlock(x, z);
	//std::cout << "Block added at: " << x << ", " << y << ", " << z << " with type: " << static_cast<int>(blockType) << std::endl;
	//std::cout << "New block with delta: " << deltaKey << std::endl;
}

void Chunk::swapMesh() {
	std::swap(m_mesh, m_buildMesh);
}

void Chunk::applyDeltas(const ChunkSnapshot& snapshot){
	for (int i = 0; i < snapshot.deltas_counts.count; i++) {
		const Delta& delta = snapshot.deltas_counts.deltas[i];
		int x = (delta.index >> 12) & 0xF;
		int y = (delta.index >> 4) & 0xFF;
		int z = delta.index & 0xF;
		BlockType blockType = blockTypeCast(delta.blockType);
		m_blocks.at(x).at(y).at(z) = blockType;
		// We want to restore the chunks deltas. If not done when first time reloaded the chunk from memory it'll will apply the deltas but not store them in the chunk.
		// Second time the chunk is created without deltas becouse when destroyed second time it had no deltas stored.
		addDelta(x, y, z, blockType);
	}
}

BlockType Chunk::blockTypeCast(unsigned char id) const{
	switch (id) {
	case 0: return BlockType::Air;
	case 1: return BlockType::Grass;
	case 2: return BlockType::Dirt;
	case 3: return BlockType::Stone;
	case 4: return BlockType::Bedrock;
	case 5: return BlockType::OakLeaf;
	case 6: return BlockType::OakLog;
	default: return BlockType::Air;
	}
}

void Chunk::addDelta(int x, int y, int z, BlockType blockType) {
	uint16_t deltaKey = (x << 12) | (y << 4) | z;
	m_deltasChanges[deltaKey] = blockType;
}

int Chunk::getMaxHeight() const {
	return m_maxHeight;
}

uint8_t Chunk::getSkyLight(int x, int y, int z) const {
	if (y < 0 || y >= Globals::CHUNK_HEIGHT) return 0;
	if (x < 0 || x >= Globals::CHUNK_WIDTH || z < 0 || z >= Globals::CHUNK_WIDTH) return 0;
	return m_lighting[x][y][z] & 0x0F;
}

uint8_t Chunk::getLightAt(int x, int y, int z, const ChunkPackage& package) const {
	if (y < 0 || y >= Globals::CHUNK_HEIGHT) return 0;
	std::shared_ptr<Chunk> targetChunk = nullptr;

	int chunkX = 0;
	int chunkZ = 0;

	if (x < 0) {
		chunkX = -1;
		x += Globals::CHUNK_WIDTH;
	}
	else if (x >= Globals::CHUNK_WIDTH) {
		chunkX = 1;
		x -= Globals::CHUNK_WIDTH;
	}

	if (z < 0) {
		chunkZ = -1;
		z += Globals::CHUNK_WIDTH;
	}
	else if (z >= Globals::CHUNK_WIDTH) {
		chunkZ = 1;
		z -= Globals::CHUNK_WIDTH;
	}

	if (chunkX == 0 && chunkZ == 0)   targetChunk = package.center;
	else if (chunkX == -1 && chunkZ == 0)  targetChunk = package.left;
	else if (chunkX == 1 && chunkZ == 0)   targetChunk = package.right;
	else if (chunkX == 0 && chunkZ == -1)  targetChunk = package.back;
	else if (chunkX == 0 && chunkZ == 1)   targetChunk = package.front;
														 
	else if (chunkX == -1 && chunkZ == -1) targetChunk = package.topLeft;
	else if (chunkX == 1 && chunkZ == -1)  targetChunk = package.topRight;
	else if (chunkX == -1 && chunkZ == 1)  targetChunk = package.bottomLeft;
	else if (chunkX == 1 && chunkZ == 1)   targetChunk = package.bottomRight;

	return targetChunk->getSkyLight(x, y, z);
}

void Chunk::setSkyLight(int x, int y, int z, uint8_t& value) {
	if (y < 0 || y >= Globals::CHUNK_HEIGHT) return;
	if (x < 0 || x >= Globals::CHUNK_WIDTH || z < 0 || z >= Globals::CHUNK_WIDTH) return;
	m_lighting[x][y][z] = (m_lighting[x][y][z] & 0xF0) | (value & 0x0F);
}

void Chunk::setLightAt(int x, int y, int z, uint8_t value, const ChunkPackage& package) {
	if (y < 0 || y >= Globals::CHUNK_HEIGHT) return;
	std::shared_ptr<Chunk> targetChunk = nullptr;

	int chunkX = 0;
	int chunkZ = 0;

	if (x < 0) {
		chunkX = -1;
		x += Globals::CHUNK_WIDTH;
	}
	else if (x >= Globals::CHUNK_WIDTH) {
		chunkX = 1;
		x -= Globals::CHUNK_WIDTH;
	}

	if (z < 0) {
		chunkZ = -1;
		z += Globals::CHUNK_WIDTH;
	}
	else if (z >= Globals::CHUNK_WIDTH) {
		chunkZ = 1;
		z -= Globals::CHUNK_WIDTH;
	}

	if (chunkX == 0 && chunkZ == 0)   targetChunk = package.center;
	else if (chunkX == -1 && chunkZ == 0)  targetChunk = package.left;
	else if (chunkX == 1 && chunkZ == 0)   targetChunk = package.right;
	else if (chunkX == 0 && chunkZ == -1)  targetChunk = package.back;
	else if (chunkX == 0 && chunkZ == 1)   targetChunk = package.front;

	else if (chunkX == -1 && chunkZ == -1) targetChunk = package.topLeft;
	else if (chunkX == 1 && chunkZ == -1)  targetChunk = package.topRight;
	else if (chunkX == -1 && chunkZ == 1)  targetChunk = package.bottomLeft;
	else if (chunkX == 1 && chunkZ == 1)   targetChunk = package.bottomRight;

	if (targetChunk) {
		targetChunk->setSkyLight(x, y, z, value);

		if (targetChunk != package.center) {
			//targetChunk->markAsDirty();
		}
	}
}

void Chunk::calculateLightingPropagation(const ChunkPackage& chunkPackage) {
	std::queue<glm::vec3> lightQueue;

	for (int x = 0; x < Globals::CHUNK_WIDTH; x++) {
		for (int z = 0; z < Globals::CHUNK_WIDTH; z++) {
			for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
				if (m_blocks[x][y][z] != BlockType::Air) {
					
					uint8_t lightLevel = m_lighting[x][y][z];

					if (lightLevel == 15) lightQueue.push(glm::vec3(x, y, z));
				}
			}
		}
	}

	while (!lightQueue.empty()) {
		glm::ivec3 curr = lightQueue.front();
		lightQueue.pop();
		uint8_t currentLight = getLightAt(curr.x, curr.y, curr.z, chunkPackage);

		glm::ivec3 directions[6] = {
			{1, 0, 0}, {-1, 0, 0},
			{0, 1, 0}, {0, -1, 0},
			{0, 0, 1}, {0, 0, -1}
		};
		for (const auto& dir : directions) {
			glm::ivec3 neighbor = curr + dir;

			if (neighbor.y < 0 || neighbor.y >= 256) continue;
			
			uint8_t neighborLight = getLightAt(neighbor.x, neighbor.y, neighbor.z, chunkPackage);
			if (neighborLight < currentLight - 1) {
			    setLightAt(neighbor.x, neighbor.y, neighbor.z, currentLight - 1, chunkPackage);
			    lightQueue.push(neighbor);
			}
		}
	}
}

void Chunk::calculateFirstBlock(int x, int z) {
	int lastBlockY;
	for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
		if (m_blocks[x][y][z] != BlockType::Air) {
			m_lighting[x][y][z] = 15;
			lastBlockY = y;
			break;
		}
	}
	for (int y = lastBlockY + 1; y < Globals::CHUNK_HEIGHT; y++) {
		m_lighting[x][y][z] = 0;
	}
}