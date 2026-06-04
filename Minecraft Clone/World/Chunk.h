#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Rendering/Vao.h>
#include <Rendering/Vbo.h>
#include <Rendering/Texture.h>
#include <Rendering/ShaderProgram.h>
#include <Core/Globals.h>
#include "CubeData.h"
#include "TerrainGeneration.h"
#include <memory>
#include <random>

class Chunk
{
public:
	Chunk(const ShaderProgram* shader, int worldX, int worldZ, const TerrainGenerator& terrain);
	~Chunk() = default;

	void buildMesh(	
		 Chunk* left = nullptr,  Chunk* right = nullptr,
		 Chunk* front = nullptr,  Chunk* back = nullptr,
		 Chunk* topLeft = nullptr,  Chunk* topRight = nullptr,
		 Chunk* bottomLeft = nullptr,  Chunk* bottomRight = nullptr);

	void render(const glm::mat4& projection);

	void fillBlocks(const TerrainGenerator& terrain);

	void generateTrees(
		Chunk* left = nullptr, Chunk* right = nullptr,
		Chunk* front = nullptr, Chunk* back = nullptr);

	void pushVertex(uint32_t packedVertex);

	void setBuffers();

	bool isAir(
		int x, int y, int z, 
		 Chunk* left = nullptr,  Chunk* right = nullptr,
		 Chunk* front = nullptr,  Chunk* back = nullptr,
		 Chunk * topLeft = nullptr,  Chunk * topRight = nullptr,
		 Chunk * bottomLeft = nullptr,  Chunk * bottomRight = nullptr) ;

	bool castsAO(BlockType type);

	std::vector<uint32_t> getMesh();

	BlockType getBlock(int x, int y, int z) const;
	void deleteBlock(int x, int y, int z);
	void addBlock(int x, int y, int z, BlockType blockType);
	void swapMesh();

private:
	BlockType m_blocks[16][256][16] = {BlockType::Air};
	std::vector<uint32_t> m_mesh;
	std::vector<uint32_t> m_buildMesh;
	const ShaderProgram* m_shader;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
	std::pair<int, int> m_worldPosition;
};