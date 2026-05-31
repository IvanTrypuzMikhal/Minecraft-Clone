#pragma once
#include "FastNoiseLite.h"



class TerrainGenerator
{
public:
	TerrainGenerator(int seed = 1337);
	~TerrainGenerator() = default;

	int getHeight(int worldX, int worldZ) const;

private:
	FastNoiseLite m_noise;
};
