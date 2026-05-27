#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Vao.h"
#include "Vbo.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <memory>

enum BlockType : unsigned char
{
	Air,
	Grass,
	Dirt,
	Stone,
	Bedrock,
};

class Chunk
{
public:
	Chunk(const Texture* texture, const ShaderProgram* shader);
	~Chunk() = default;

	void buildMesh();
	void render(const glm::mat4& projection, const glm::mat4& model);
	void fillBlocks();
	bool isAir(int x, int y, int z) const;
	void setBuffers();

	std::vector<float> getMesh();

private:
	BlockType m_blocks[16][256][16] = {BlockType::Air};
	std::vector<float> m_mesh;
	const Texture* m_texture;
	const ShaderProgram* m_shader;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
};