#pragma once
#include "Chunk.h"
#include "ShaderProgram.h"
#include <utility>
#include <map>
#include <memory>

struct Position
{
	int x;
	int y;
	int z;
};

class World
{
public:
	World(ShaderProgram* shader) : m_shader{ shader } {}
	~World() = default;

	void renderWorld(const glm::mat4& projection, const glm::mat4& model);
	void update(float x, float y, float z);

private:
	std::map<std::pair<int, int>, std::unique_ptr<Chunk>> m_chunks;
	Position m_cameraPosition;
	ShaderProgram* m_shader;

};