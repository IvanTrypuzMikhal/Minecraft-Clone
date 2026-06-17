#pragma once
#include <glm/glm.hpp>
#include <array>
#include <Gameplay/AABB.h>

struct Plane
{
	glm::vec3 normal;
	float distance;
};

struct Frustum
{
	std::array<Plane, 6> planes;


	bool isAABBInFrustum(const AABB& aabb) const {
		for (int i = 0; i < 6; ++i) {
			const Plane& plane = planes[i];
			// Compute the positive vertex of the AABB with respect to the plane normal
			glm::vec3 positiveVertex = aabb.min;
			if (plane.normal.x >= 0) positiveVertex.x = aabb.max.x;
			if (plane.normal.y >= 0) positiveVertex.y = aabb.max.y;
			if (plane.normal.z >= 0) positiveVertex.z = aabb.max.z;
			// If the positive vertex is outside the plane, the AABB is outside the frustum
			if (glm::dot(plane.normal, positiveVertex) + plane.distance < 0) {
				return false;
			}
		}
		return true;
	}
};

