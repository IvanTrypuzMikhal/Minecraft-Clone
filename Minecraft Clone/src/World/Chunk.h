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
#include <queue>

struct ChunkPackage;
struct ChunkSnapshot;

class Chunk : public std::enable_shared_from_this<Chunk>
{
public:
	Chunk(const std::shared_ptr<ShaderProgram> shader, int worldX, int worldZ, const TerrainGenerator& terrain);
	~Chunk() = default;

	void buildMesh(const ChunkPackage& chunkPackage);
	void render(const glm::mat4& projection, float ambientLightIntensity);
	void fillBlocks(const TerrainGenerator& terrain);
	void generateTrees(const ChunkPackage& chunkPackage, int seed);
	void pushVertex(uint32_t packedVertex, uint32_t packedAttributes);
	void setBuffers();
	void applyDeltas(const ChunkSnapshot& snapshot);
	void calculateLightingPropagation(const ChunkPackage& chunkPackage);
	bool isAir(int x, int y, int z, const ChunkPackage& chunkPackage) ;
	bool castsAO(BlockType type);
	void addDelta(int x, int y, int z, BlockType blockType);
	int getMaxHeight() const;
	std::pair<int, int> getWorldPosition() const { return m_worldPosition; }

	std::vector<uint32_t> getMesh();

	[[nodiscard]] BlockType getBlock(int x, int y, int z) const;
	[[nodiscard]] const std::unordered_map<uint16_t, BlockType>& getDeltasChanges() const;
	[[nodiscard]] bool isDirty() const { return m_isDirty; }
	void deleteBlock(int x, int y, int z);
	void addBlock(int x, int y, int z, BlockType blockType);
	void swapMesh();
	BlockType blockTypeCast(unsigned char id) const;
	uint8_t getSkyLight(int x, int y, int z) const;
	uint8_t getLightAt(int x, int y, int z, const ChunkPackage& package) const;
	void setLightAt(int x, int y, int z, uint8_t value, const ChunkPackage& package);
	void setSkyLight(int x, int y, int z, uint8_t& value);
	void setDirty(bool dirty) { m_isDirty = dirty; }
	void calculateFirstBlock(int x, int z);
private:
	// Chunk data
	std::array<std::array<std::array<BlockType, 16>, 256>,16> m_blocks = {BlockType::Air};
	std::array<std::array<std::array<uint8_t, 16>, 256>, 16> m_lighting = { 0 }; 	// First 4 bits are global lighting and second 4 bits are indirect lighting. Each block can have a value from 0 to 15 for each lighting type.
	std::unordered_map<uint16_t, BlockType> m_deltasChanges;
	std::vector<uint32_t> m_mesh;
	std::vector<uint32_t> m_buildMesh;
	std::pair<int, int> m_worldPosition;
	bool m_isDirty = false;
	int m_maxHeight = 0;
	unsigned int m_vertexCount = 0;

	// Rendering data
	const std::shared_ptr<ShaderProgram> m_shader;
	std::unique_ptr<Vao> m_vao;
	std::unique_ptr<Vbo> m_vbo;
};