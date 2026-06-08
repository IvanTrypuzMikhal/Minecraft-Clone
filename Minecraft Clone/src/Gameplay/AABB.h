#pragma once
#include <glm/glm.hpp>



struct AABB
{
	// 
	AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {};
	
	[[nodiscard]] bool intersects(const AABB& other) const {
		return (min.x <= other.max.x && max.x >= other.min.x) &&
			   (min.y <= other.max.y && max.y >= other.min.y) &&
			   (min.z <= other.max.z && max.z >= other.min.z);
	}
	
	glm::vec3 min;
	glm::vec3 max;
};

struct CollisionRes {
	bool collision;
	AABB blockAABB;
};