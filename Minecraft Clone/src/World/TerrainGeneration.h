#pragma once
#include <Externals/FastNoiseLite.h>



class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator() = default;

	[[nodiscard]] int getHeight(int worldX, int worldZ) const;
	[[nodiscard]] int getSeed() const { return m_seed; }

private:
	FastNoiseLite m_noise;
	int m_seed = 1234567;
};
