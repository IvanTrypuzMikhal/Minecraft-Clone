#include "Chunk.h"

static BlockType getBlockType(int x, int y, int z);
static std::pair<int, int> getAtlasCoordinates(BlockType type, BlockFace face);

Chunk::Chunk(const ShaderProgram* shader, int x, int z) : m_shader{ shader }, m_worldPosition{x, z} {
	fillBlocks();
}

void Chunk::setBuffers() {
	std::vector<VertexAttribute> block{ { 0, 3, 0 }, { 1, 2, 3 } };
	m_vbo = std::make_unique<Vbo>();
	m_vao = std::make_unique<Vao>(m_vbo->get(), 5, block);
	m_vbo->upload(m_mesh);

}

void Chunk::render(const glm::mat4& projection, const glm::mat4& model) {
	m_shader->setMat4("projection", projection);
	m_shader->setMat4("model", model);
	m_vao->use();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh.size()/5);
}

void Chunk::buildMesh(
	const Chunk* left, const Chunk* right,
	const Chunk* front, const Chunk* back)

{
	m_mesh.clear();
	for (int x = 0; x < Globals::CHUNK_WIDTH; x++) {
		for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
			for (int z = 0; z < Globals::CHUNK_WIDTH; z++) {
				BlockType type = m_blocks[x][y][z];
				if (type == BlockType::Air) continue;

				for (const auto& cubeData : CubeData::CUBE_FACES) {

					if (isAir(cubeData.dx + x, cubeData.dy + y, cubeData.dz + z, left, right, front, back) || type == BlockType::OakLog) {
						auto [atlasX, atlasY] = getAtlasCoordinates(type, cubeData.faceDirection);

						float uMin = atlasX * Globals::TEXTURE_SIZE;
						float vMin = atlasY * Globals::TEXTURE_SIZE;
						float uMax = uMin + Globals::TEXTURE_SIZE;
						float vMax = vMin + Globals::TEXTURE_SIZE;
						
						for (int i = 0; i < 6; i++) {
							float vx = x + cubeData.vertices[i][0];
							float vy = - y + cubeData.vertices[i][1];
							float vz = - z + cubeData.vertices[i][2];
							
							float u = (cubeData.vertices[i][3] == 1.0f) ? uMax : uMin;
							float v = (cubeData.vertices[i][4] == 1.0f) ? vMax : vMin;
							
							pushVertex(vx, vy, vz, u, v);
						}
					}

				}
				
			}
		}
	}
}


bool Chunk::isAir(int x, int y, int z, const Chunk* left, const Chunk* right, const Chunk* front, const Chunk* back) const {
	if (y < 0 || y >= Globals::CHUNK_HEIGHT)
		return true;

	if (x < 0)
		return left ? (left->getBlock(Globals::CHUNK_WIDTH - 1, y, z) == BlockType::Air) : true;

	if (x >= Globals::CHUNK_WIDTH)
		return right ? (right->getBlock(0, y, z) == BlockType::Air) : true;

	if (z < 0)
		return front ? (front->getBlock(x, y, Globals::CHUNK_WIDTH - 1) == BlockType::Air) : true;

	if (z >= Globals::CHUNK_WIDTH)
		return back ? (back->getBlock(x, y, 0) == BlockType::Air) : true;

	return m_blocks[x][y][z] == BlockType::Air;
}

std::vector<float> Chunk::getMesh(){
	return m_mesh;
}


void Chunk::generateTrees(
	Chunk* left, Chunk* right,
	Chunk* front, Chunk* back) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> location(0, Globals::CHUNK_WIDTH-1);
	std::uniform_int_distribution<> randomHeight(5, 8);

	int treeX = location(gen);
	int treeY = 0;
	int treeZ = location(gen);
	int treeHeight = randomHeight(gen);

	for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
		if (getBlock(treeX, y, treeZ) != BlockType::Air) {
			treeY = y - 1;
			break;
		}
	}

	for (int y = 0; y < treeHeight; y++) {
		m_blocks[treeX][treeY - y][treeZ] = BlockType::OakLog;
	}

	int leafStartHeight = treeY - 7;
	for (int x = -2; x <= 2; x++) {
		for (int y = 0; y < 4; y++) {
			for (int z = -2; z <= 2; z++) {
				int leafX = treeX + x;
				int leafY = leafStartHeight + y;
				int leafZ = treeZ + z;

				Chunk* targetChunk = this;


				//TODO: Need to check spawned near the vertice of a chunk
				// Either dont spawn trees on those locations or pass the other 4 neigbor chunks
				if (leafX >= 16) {
					leafX = leafX - 16;
					targetChunk = right;
				}
				
				else if (leafX < 0) {
					leafX = leafX + 16;
					targetChunk = left;
				}

				if (leafZ >= 16) {
					leafZ = leafZ - 16;
					if (targetChunk == this) targetChunk = back;
					else targetChunk = nullptr;
				}

				else if (leafZ < 0) {
					leafZ = leafZ + 16;
					if (targetChunk == this) targetChunk = front;
					else targetChunk = nullptr;
				}

				if (targetChunk != nullptr) {
					if (targetChunk->m_blocks[leafX][leafY][leafZ] == BlockType::Air) {
						targetChunk->m_blocks[leafX][leafY][leafZ] = BlockType::OakLeaf;
					}
				}
			}
		}
	}

}

void Chunk::fillBlocks() {
	for (int x = 0; x < Globals::CHUNK_WIDTH; x++) {
		for (int y = 0; y < Globals::CHUNK_HEIGHT; y++) {
			for (int z = 0; z < Globals::CHUNK_WIDTH; z++) {
				m_blocks[x][y][z] = getBlockType(x, y, z);
			}
		}
	}
}

void Chunk::pushVertex(float x, float y, float z, float u, float v) {
	m_mesh.push_back(x);
	m_mesh.push_back(y);
	m_mesh.push_back(z);
	m_mesh.push_back(u);
	m_mesh.push_back(v);
}

static BlockType getBlockType(int x, int y, int z) {

	if (y == Globals::GRASS_LEVEL) {
		return BlockType::Grass;
	}
	else if (y > Globals::GRASS_LEVEL && y < Globals::DIRT_DEPTH) {
		return BlockType::Dirt;
	}
	else if (y >= Globals::DIRT_DEPTH && y < Globals::STONE_DEPTH) {
		return BlockType::Stone;
	}
	else if (y >= Globals::CHUNK_HEIGHT-2) {
		return BlockType::Bedrock;
	}
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
	return m_blocks[x][y][z];
}