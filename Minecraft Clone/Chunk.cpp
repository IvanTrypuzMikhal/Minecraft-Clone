#include "Chunk.h"

static BlockType getBlockType(int x, int y, int z);

Chunk::Chunk(const Texture* texture, const ShaderProgram* shader) : m_texture{ texture }, m_shader{ shader } {
	m_vbo = std::make_unique<Vbo>();
	std::cout << m_vbo.get() << std::endl;
	fillBlocks();
	buildMesh();
	setBuffers();
}

void Chunk::setBuffers() {
	std::vector<VertexAttribute> block{ { 0, 3, 0 }, { 1, 2, 3 } };

	m_vao = std::make_unique<Vao>(m_vbo->get(), 5, block);
	m_vbo->upload(m_mesh);

}

void Chunk::render(const glm::mat4& projection, const glm::mat4& model) {
	m_shader->setMat4("projection", projection);
	m_shader->setMat4("model", model);
	m_texture->setTexture();
	m_shader->use();
	m_vao->use();
	glDrawArrays(GL_TRIANGLES, 0, m_mesh.size()/5);
}

void Chunk::buildMesh() {
	for (int y = 0; y < 256; y++) {
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				if (m_blocks[x][y][z] == BlockType::Air) continue;

				// Frontal face
				if (isAir(x, y, z - 1)) {
					// Vertices positions and texture

					// Top left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.5f);


					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);
					
					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.0f);
				}

				// Back face
				if (isAir(x, y, z + 1)) {
					// Vertices positions and texture

					// Top left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.5f);


					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.0f);
				}

				// Right face
				if (isAir(x + 1, y, z)) {
					// Vertices positions and texture

					// Top left
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.5f);


					// Bottom left
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom left
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.0f);
				}

				// Left face
				if (isAir(x - 1, y, z)) {
					// Vertices positions and texture

					// Top left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.5f);

					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom right
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.0f);
				}

				// Top face
				if (isAir(x, y - 1, z)) {
					// Vertices positions and texture

					// Top left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);

					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(1.0f);
					m_mesh.push_back(0.5f);

					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.0f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(1.0f);
					m_mesh.push_back(0.5f);

					// Bottom right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(0.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(1.0f);
					m_mesh.push_back(0.0f);
				}

				// Bottom face
				if (isAir(x, y + 1, z)) {
					// Vertices positions and texture

					// Top left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(1.0f);


					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.5f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(1.0f);

					// Bottom left
					m_mesh.push_back(0.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.0f);
					m_mesh.push_back(0.5f);

					// Top right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(-1.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(1.0f);

					// Bottom right
					m_mesh.push_back(1.0f + x);
					m_mesh.push_back(-1.0f - y);
					m_mesh.push_back(0.0f - z);

					m_mesh.push_back(0.5f);
					m_mesh.push_back(0.5f);
				}
			}
		}
	}
	std::cerr << "Mesh size: " << m_mesh.size() << std::endl;
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

bool Chunk::isAir(int x, int y, int z) const{
	if (x < 0 || x >= 16 || y < 0 || y >= 256 || z < 0 || z >= 16)
		return true;
	return m_blocks[x][y][z] == BlockType::Air;
}

std::vector<float> Chunk::getMesh(){
	return m_mesh;
}



static BlockType getBlockType(int x, int y, int z) {
	if (y == 0) {
		return BlockType::Grass;
	}
	else if (y > 0 && y < 10) {
		return BlockType::Dirt;
	}
	else if (y > 10 && y < 254) {
		return BlockType::Stone;
	}
	else {
		return BlockType::Bedrock;
	}
}