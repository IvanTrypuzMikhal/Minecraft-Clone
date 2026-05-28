#include "Chunk.h"

static BlockType getBlockType(int x, int y, int z);
static std::pair<int, int> getAtlasCoordinates(BlockType type, BlockFace face);

Chunk::Chunk(const ShaderProgram* shader) : m_shader{ shader } {
	fillBlocks();
	buildMesh();
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

void Chunk::buildMesh() {
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				BlockType type = m_blocks[x][y][z];
				if (type == BlockType::Air) continue;
				
				// Frontal face
				if (isAir(x, y, z - 1)) {
					auto [atlasX, atlasY] = getAtlasCoordinates(type, BlockFace::Front);

					float uMin = atlasX * Globals::TEXTURE_SIZE;
					float vMin = atlasY * Globals::TEXTURE_SIZE;
					float uMax = uMin + Globals::TEXTURE_SIZE;
					float vMax = vMin + Globals::TEXTURE_SIZE;
					// Top left
					pushVertex(0.0f + x, 0.0f - y, 0.0f - z, uMin, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, 0.0f - z, uMax, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, 0.0f - z, uMax, vMax);

					// Bottom right
					pushVertex(1.0f + x, -1.0f - y, 0.0f - z, uMax, vMin);
				}

				// Back face
				if (isAir(x, y, z + 1)) {
					auto [atlasX, atlasY] = getAtlasCoordinates(type, BlockFace::Back);

					float uMin = atlasX * Globals::TEXTURE_SIZE;
					float vMin = atlasY * Globals::TEXTURE_SIZE;
					float uMax = uMin + Globals::TEXTURE_SIZE;
					float vMax = vMin + Globals::TEXTURE_SIZE;

					// Vertices positions and texture

					// Top left
					pushVertex(0.0f + x, 0.0f - y, -1.0f - z, uMin, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, -1.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, -1.0f - z, uMax, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, -1.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, -1.0f - z, uMax, vMax);

					// Bottom right
					pushVertex(1.0f + x, -1.0f - y, -1.0f - z, uMax, vMin);
				}

				// Right face
				if (isAir(x + 1, y, z)) {
					auto [atlasX, atlasY] = getAtlasCoordinates(type, BlockFace::Right);

					float uMin = atlasX * Globals::TEXTURE_SIZE;
					float vMin = atlasY * Globals::TEXTURE_SIZE;
					float uMax = uMin + Globals::TEXTURE_SIZE;
					float vMax = vMin + Globals::TEXTURE_SIZE;

					// Vertices positions and texture

					// Top left
					pushVertex(1.0f + x, 0.0f - y, 0.0f - z, uMin, vMax);

					// Bottom left
					pushVertex(1.0f + x, -1.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, -1.0f - z, uMax, vMax);

					// Bottom left
					pushVertex(1.0f + x, -1.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, -1.0f - z, uMax, vMax);

					// Bottom right
					pushVertex(1.0f + x, -1.0f - y, -1.0f - z, uMax, vMin);

				}

				// Left face
				if (isAir(x - 1, y, z)) {
					auto [atlasX, atlasY] = getAtlasCoordinates(type, BlockFace::Left);

					float uMin = atlasX * Globals::TEXTURE_SIZE;
					float vMin = atlasY * Globals::TEXTURE_SIZE;
					float uMax = uMin + Globals::TEXTURE_SIZE;
					float vMax = vMin + Globals::TEXTURE_SIZE;

					// Vertices positions and texture

					// Top left
					pushVertex(0.0f + x, 0.0f - y, -1.0f - z, uMin, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, -1.0f - z, uMin, vMin);

					// Top right
					pushVertex(0.0f + x, 0.0f - y, 0.0f - z, uMax, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, -1.0f - z, uMin, vMin);
			
					// Top right
					pushVertex(0.0f + x, 0.0f - y, 0.0f - z, uMax, vMax);

					// Bottom right
					pushVertex(0.0f + x, -1.0f - y, 0.0f - z, uMax, vMin);
				}

				// Top face
				if (isAir(x, y - 1, z)) {
					auto [atlasX, atlasY] = getAtlasCoordinates(type, BlockFace::Top);

					float uMin = atlasX * Globals::TEXTURE_SIZE;
					float vMin = atlasY * Globals::TEXTURE_SIZE;
					float uMax = uMin + Globals::TEXTURE_SIZE;
					float vMax = vMin + Globals::TEXTURE_SIZE;

					// Vertices positions and texture

					// Top left
					pushVertex(0.0f + x, 0.0f - y, -1.0f - z, uMin, vMax);

					// Bottom left
					pushVertex(0.0f + x, 0.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, -1.0f - z, uMax, vMax);

					// Bottom left
					pushVertex(0.0f + x, 0.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, 0.0f - y, -1.0f - z, uMax, vMax);

					// Bottom right
					pushVertex(1.0f + x, 0.0f - y, 0.0f - z, uMax, vMin);
				}

				// Bottom face
				if (isAir(x, y + 1, z)) {
					auto [atlasX, atlasY] = getAtlasCoordinates(type, BlockFace::Bottom);

					float uMin = atlasX * Globals::TEXTURE_SIZE;
					float vMin = atlasY * Globals::TEXTURE_SIZE;
					float uMax = uMin + Globals::TEXTURE_SIZE;
					float vMax = vMin + Globals::TEXTURE_SIZE;

					// Vertices positions and texture

					// Top left
					pushVertex(0.0f + x, -1.0f - y, -1.0f - z, uMin, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, -1.0f - y, -1.0f - z, uMax, vMax);

					// Bottom left
					pushVertex(0.0f + x, -1.0f - y, 0.0f - z, uMin, vMin);

					// Top right
					pushVertex(1.0f + x, -1.0f - y, -1.0f - z, uMax, vMax);

					// Bottom right
					pushVertex(1.0f + x, -1.0f - y, 0.0f - z, uMax, vMin);
				}
			}
		}
	}
}

bool Chunk::isAir(int x, int y, int z) const{
	if (x < 0 || x >= Globals::CHUNK_WIDTH || y < 0 || y >= Globals::CHUNK_HEIGHT || z < 0 || z >= Globals::CHUNK_WIDTH)
		return true;
	return m_blocks[x][y][z] == BlockType::Air;
}

std::vector<float> Chunk::getMesh(){
	return m_mesh;
}

void Chunk::fillBlocks() {
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
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
	if (y == 0) {
		return BlockType::Grass;
	}
	else if (y > 0 && y < 10) {
		return BlockType::Dirt;
	}
	else if (y >= 10 && y < 254) {
		return BlockType::Stone;
	}
	else if(y >= 254){
		return BlockType::Bedrock;
	}
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
	}
}