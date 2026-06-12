#include "Chunk.h"
#include "ChunkPackage.h"

static BlockType getBlockType(int y, int surfaceY);
static std::pair<int, int> getAtlasCoordinates(BlockType type, BlockFace face);
uint32_t packVertex(float x, float y, float z, float u, float v, int textureId, int ao);

Chunk::Chunk(const ShaderProgram* shader, int x, int z, const TerrainGenerator& terrain) : m_shader{ shader }, m_worldPosition{x, z} {
	fillBlocks(terrain);
}

void Chunk::setBuffers() {
	std::vector<VertexAttribute> block{ {0, 1, 0, GL_UNSIGNED_INT } };
	m_vbo = std::make_unique<Vbo>();
	m_vao = std::make_unique<Vao>(m_vbo->get(), 1, block);
	m_vbo->upload(m_mesh);

}

void Chunk::render(const glm::mat4& projection) {
	m_shader->use();
	m_shader->setMat4("projection", projection);
	glm::vec3 chunkPos(
		static_cast<float>(m_worldPosition.first * Globals::CHUNK_WIDTH),
		0.0f,
		static_cast<float>(m_worldPosition.second * Globals::CHUNK_WIDTH)
	);
	m_shader->setVec3("chunkPos", chunkPos);
	m_vao->use();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh.size());
}

void Chunk::buildMesh(const ChunkPackage& chunkPackage)

{
	m_buildMesh.clear();
	for (int x = 0; x < Globals::CHUNK_WIDTH; x++) {
		for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
			for (int z = 0; z < Globals::CHUNK_WIDTH; z++) {
				const BlockType type = m_blocks.at(x).at(y).at(z);
				if (type == BlockType::Air) continue;

				for (const auto& cubeData : CubeData::CUBE_FACES) {

					if (isAir(cubeData.dx + x, cubeData.dy + y, cubeData.dz + z, chunkPackage)) {
						for (int i = 0; i < 6; i++) {
							const float vx = x + cubeData.vertices.at(i).x;
							const float vy = y + cubeData.vertices.at(i).y; // Change to possitive to avoid packing wrong values
							const float vz = z + cubeData.vertices.at(i).z; // Change to possitive to avoid packing wrong values
							
							const float u = cubeData.vertices.at(i).u;
							const float v = cubeData.vertices.at(i).v;
							
							auto [textureX, textureY] = getAtlasCoordinates(type, cubeData.faceDirection);
							const int textureId = (textureY * 4) + textureX;


							const int ao1 = !isAir(x + cubeData.vertices.at(i).side1.x, y - cubeData.vertices.at(i).side1.y, z - cubeData.vertices.at(i).side1.z, chunkPackage);
							const int ao2 = !isAir(x + cubeData.vertices.at(i).side2.x, y - cubeData.vertices.at(i).side2.y, z - cubeData.vertices.at(i).side2.z, chunkPackage);
							const int ao3 = !isAir(x + cubeData.vertices.at(i).diagonal.x, y - cubeData.vertices.at(i).diagonal.y, z - cubeData.vertices.at(i).diagonal.z, chunkPackage);
							
							const int ao = (ao1 && ao2) ? 0 : 3 - (ao1 + ao2 + ao3);
							
							const uint32_t packedVertex = packVertex(vx, vy, vz, u, v, textureId, ao);
							pushVertex(packedVertex);
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

	return targetChunk->m_blocks.at(x).at(y).at(z) == BlockType::Air;
}

std::vector<uint32_t> Chunk::getMesh(){
	return m_mesh;
}

void Chunk::generateTrees(const ChunkPackage& chunkPackage, int seed) {
	
	std::random_device rd;
	
	int32_t chunkX = chunkPackage.coords.first;
	int32_t chunkZ = chunkPackage.coords.second;
	std::cout << "Generating trees for chunk: " << chunkPackage.coords.first << ", " << chunkPackage.coords.second << std::endl;
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

			for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
				
				m_blocks.at(x).at(y).at(z) = getBlockType(y, surfaceY);
			}
		}
	}
}

void Chunk::pushVertex(uint32_t packedVertex) {
	m_buildMesh.push_back(packedVertex);
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
	uint16_t deltaKey = (x << 12) | (y << 4) | z;
	m_deltasChanges[deltaKey] = BlockType::Air;
}

void Chunk::addBlock(int x, int y, int z, BlockType blockType) {
	m_blocks.at(x).at(y).at(z) = blockType;
	uint16_t deltaKey = (x << 12) | (y << 4) | z;
	m_deltasChanges.insert({ deltaKey, blockType });
	//std::cout << "Block added at: " << x << ", " << y << ", " << z << " with type: " << static_cast<int>(blockType) << std::endl;
	//std::cout << "New block with delta: " << deltaKey << std::endl;
}

void Chunk::swapMesh() {
	std::swap(m_mesh, m_buildMesh);
}