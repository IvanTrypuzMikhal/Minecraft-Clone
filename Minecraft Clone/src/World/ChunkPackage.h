#pragma once
#include "Chunk.h"


enum State : unsigned char
{
	TERRAIN_READY,
	DECORATED,
	MESH_BUILDING,
	MESH_READY
};


struct ChunkState
{
	std::unique_ptr<Chunk> chunk;
	State state;
};


struct ChunkPackage
{
	std::pair<int, int> coords;
	Chunk* center;
	Chunk* left;
	Chunk* right;
	Chunk* front;
	Chunk* back;
	Chunk* topLeft;
	Chunk* topRight;
	Chunk* bottomLeft;
	Chunk* bottomRight;
};

struct FinishedChunk
{
	ChunkState chunkState;
	std::pair<int, int> coords;
};
