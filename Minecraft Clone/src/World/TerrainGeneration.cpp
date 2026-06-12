#include "TerrainGeneration.h"

TerrainGenerator::TerrainGenerator() {

    m_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noise.SetSeed(m_seed);

    m_noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_noise.SetFractalOctaves(5);
    m_noise.SetFractalLacunarity(2.0f);
    m_noise.SetFractalGain(0.5f);

    m_noise.SetFrequency(0.004f);
}

int TerrainGenerator::getHeight(int worldX, int worldZ) const {
    float n = m_noise.GetNoise((float)worldX, (float)worldZ); 
    float normalized = (n + 1.0f) / 2.0f; 
    return static_cast<int>(40.0f + normalized * 90.0f);
}