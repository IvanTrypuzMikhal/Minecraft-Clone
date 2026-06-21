#pragma once
#include "Chunk.h"
#include <cstring>

struct PairHash {
	size_t operator()(const std::pair<int, int>& p) const {
		return (static_cast<size_t>(p.first) << 32) | static_cast<uint32_t>(p.second);
	}
};


enum State : unsigned char
{
	TERRAIN_READY,
	DECORATING,
	DECORATED,
	CALCULATING_LIGHTING,
	LIGHTING_READY,
	MESH_BUILDING,
	MESH_READY,
	SAVING,
	LOADING,
};

struct ChunkState
{
	std::shared_ptr<Chunk> chunk;
	State state;
};

struct FinishedChunk
{
	ChunkState chunkState;
	std::pair<int, int> coords;
};

struct Delta
{
	uint16_t index;
	unsigned char blockType;
};

struct Deltas
{
	Delta* deltas;
	size_t count;
};

struct ChunkSnapshot
{
	std::pair<int, int> coords;
	Deltas deltas_counts;
};

struct ChunkPackage
{
	std::pair<int, int> coords;
	std::shared_ptr<Chunk> center;
	std::shared_ptr<Chunk> left;
	std::shared_ptr<Chunk> right;
	std::shared_ptr<Chunk> front;
	std::shared_ptr<Chunk> back;
	std::shared_ptr<Chunk> topLeft;
	std::shared_ptr<Chunk> topRight;
	std::shared_ptr<Chunk> bottomLeft;
	std::shared_ptr<Chunk> bottomRight;
	ChunkSnapshot snapshot;
	bool hasDeltas = false;
};