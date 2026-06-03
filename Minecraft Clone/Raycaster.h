#pragma once
#include "Camera.h"
#include "World.h"

struct BlockHit
{
	int x, y, z;
	BlockFace face;
};

class Raycaster
{
public:
	static bool traceRay(const std::unique_ptr<World>& world, const Camera& camera, float maxDistance, BlockHit& outHit);
	static bool traceRay(World* world, const Camera& camera, float maxDistance, BlockHit& outHit);

};