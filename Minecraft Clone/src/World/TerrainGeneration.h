#pragma once
#include <Externals/FastNoiseLite.h>



class TerrainGenerator
{
public:
	TerrainGenerator(int seed = 1234567);
	~TerrainGenerator() = default;

	[[nodiscard]] int getHeight(int worldX, int worldZ) const;

private:
	FastNoiseLite m_noise;
};
