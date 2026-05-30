#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Vao.h"
#include "Vbo.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Globals.h"
#include "CubeData.h"
#include <memory>
#include <random>



class Chunk
{
public:
	Chunk(const ShaderProgram* shader, int worldX, int worldZ);
	~Chunk() = default;

	void buildMesh(	
		const Chunk* left = nullptr, const Chunk* right = nullptr,
		const Chunk* front = nullptr, const Chunk* back = nullptr);
	void render(const glm::mat4& projection);
	void fillBlocks();
	void generateTrees(
		Chunk* left = nullptr, Chunk* right = nullptr,
		Chunk* front = nullptr, Chunk* back = nullptr);
	void pushVertex(uint32_t packedVertex);
	void setBuffers();
	bool isAir(
		int x, int y, int z, 
		const Chunk* left = nullptr, const Chunk* right = nullptr,
		const Chunk* front = nullptr, const Chunk* back = nullptr) const;

	std::vector<uint32_t> getMesh();
	BlockType getBlock(int x, int y, int z) const;

private:
	BlockType m_blocks[16][256][16] = {BlockType::Air};
	std::vector<uint32_t> m_mesh;
	const ShaderProgram* m_shader;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
	std::pair<int, int> m_worldPosition;
};