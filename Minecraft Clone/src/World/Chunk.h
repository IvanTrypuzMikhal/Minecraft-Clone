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
#include <array>

struct ChunkPackage;

class Chunk : public std::enable_shared_from_this<Chunk>
{
public:
	Chunk(const ShaderProgram* shader, int worldX, int worldZ, const TerrainGenerator& terrain);
	~Chunk() = default;

	void buildMesh(const ChunkPackage& chunkPackage);

	void render(const glm::mat4& projection);

	void fillBlocks(const TerrainGenerator& terrain);

	void generateTrees(const ChunkPackage& chunkPackage, int seed);

	void pushVertex(uint32_t packedVertex);

	void setBuffers();

	bool isAir(int x, int y, int z, const ChunkPackage& chunkPackage) ;

	bool castsAO(BlockType type);

	std::vector<uint32_t> getMesh();

	[[nodiscard]] BlockType getBlock(int x, int y, int z) const;
	[[nodiscard]] const std::unordered_map<uint16_t, BlockType>& getDeltasChanges() const;
	void deleteBlock(int x, int y, int z);
	void addBlock(int x, int y, int z, BlockType blockType);
	void swapMesh();

private:
	std::array<std::array<std::array<BlockType, 16>, 256>,16> m_blocks = {BlockType::Air};
	std::unordered_map<uint16_t, BlockType> m_deltasChanges;
	std::vector<uint32_t> m_mesh;
	std::vector<uint32_t> m_buildMesh;
	const ShaderProgram* m_shader;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
	std::pair<int, int> m_worldPosition;
};