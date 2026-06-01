#pragma once
#include "World.h"
#include "Camera.h"

struct BlockHit
{
	int x, y, z;
	BlockFace face;
};

class Raycaster
{
public:
	static bool traceRay(const std::unique_ptr<World>& world, const Camera& camera, float maxDistance, BlockHit& outHit);

};