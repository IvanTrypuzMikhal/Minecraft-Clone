#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Vao.h"
#include "Vbo.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Globals.h"
#include <memory>

enum BlockType : unsigned char
{
	Air,
	Grass,
	Dirt,
	Stone,
	Bedrock,
};

enum BlockFace : unsigned char {
	Front,
	Back,
	Left,
	Right,
	Top,
	Bottom
};

class Chunk
{
public:
	Chunk(const ShaderProgram* shader);
	~Chunk() = default;

	void buildMesh(	
		const Chunk* left = nullptr, const Chunk* right = nullptr,
		const Chunk* front = nullptr, const Chunk* back = nullptr);
	void render(const glm::mat4& projection, const glm::mat4& model);
	void fillBlocks();
	void pushVertex(float x, float y, float z, float u, float v);
	void setBuffers();
	bool isAir(
		int x, int y, int z, 
		const Chunk* left = nullptr, const Chunk* right = nullptr,
		const Chunk* front = nullptr, const Chunk* back = nullptr) const;

	std::vector<float> getMesh();
	BlockType getBlock(int x, int y, int z) const;

private:
	BlockType m_blocks[16][256][16] = {BlockType::Air};
	std::vector<float> m_mesh;
	const ShaderProgram* m_shader;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
};