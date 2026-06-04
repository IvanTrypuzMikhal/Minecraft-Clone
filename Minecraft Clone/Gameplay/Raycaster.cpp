#include "Raycaster.h"

bool Raycaster::traceRay(const std::unique_ptr<World>& world, const Camera& camera, float maxDistance, BlockHit& outHit) {
    return traceRay(world.get(), camera, maxDistance, outHit);
}

bool Raycaster::traceRay(World* world, const Camera* camera, float maxDistance, BlockHit& outHit) {
    return traceRay(world, *camera, maxDistance, outHit);
}


bool Raycaster::traceRay(World* world, const Camera& camera, float maxDistance, BlockHit& outHit) {
    glm::vec3 camPos(camera.getCameraPositionX(), camera.getCameraPositionY(), camera.getCameraPositionZ());
    glm::vec3 camDir(camera.getCameraLookAtX(), camera.getCameraLookAtY(), camera.getCameraLookAtZ());

    glm::vec3 position = glm::vec3(std::floor(camPos.x), std::floor(camPos.y), std::floor(camPos.z));
    glm::vec3 step = glm::vec3((camDir.x > 0.0f) ? 1.0f : -1.0f, (camDir.y > 0.0f) ? 1.0f : -1.0f, (camDir.z > 0.0f) ? 1.0f : -1.0f);
    glm::vec3 delta = glm::vec3(std::abs(1.0f / camDir.x), std::abs(1.0f / camDir.y), std::abs(1.0f / camDir.z));

    glm::vec3 tMax;
    tMax.x = (step.x > 0) ? (position.x + 1.0f - camPos.x) * delta.x : (camPos.x - position.x) * delta.x;
    tMax.y = (step.y > 0) ? (position.y + 1.0f - camPos.y) * delta.y : (camPos.y - position.y) * delta.y;
    tMax.z = (step.z > 0) ? (position.z + 1.0f - camPos.z) * delta.z : (camPos.z - position.z) * delta.z;

    float accumulatedDistance = 0.0f;
    BlockFace hitFace = BlockFace::Top;

    while (accumulatedDistance < maxDistance) {
        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                accumulatedDistance = tMax.x;
                position.x += step.x;
                tMax.x += delta.x;
                hitFace = (step.x > 0) ? BlockFace::Left : BlockFace::Right;
            }
            else {
                accumulatedDistance = tMax.z;
                position.z += step.z;
                tMax.z += delta.z;
                hitFace = (step.z > 0) ? BlockFace:: Back : BlockFace::Front;
            }
        }
        else {
            if (tMax.y < tMax.z) {
                accumulatedDistance = tMax.y;
                position.y += step.y;
                tMax.y += delta.y;

                hitFace = (step.y > 0) ? BlockFace::Bottom : BlockFace::Top;
            }
            else {
                accumulatedDistance = tMax.z;
                position.z += step.z;
                tMax.z += delta.z;
                hitFace = (step.z > 0) ? BlockFace::Back : BlockFace::Front;
            }
        }

        int arrayY = -static_cast<int>(position.y);

        if (arrayY >= 0 && arrayY < Globals::CHUNK_HEIGHT) {
            BlockType block = world->getBlockAt(
                static_cast<int>(position.x),
                arrayY,
                static_cast<int>(position.z)
            );

            if (block != BlockType::Air) {
                outHit.x = static_cast<int>(position.x);
                outHit.y = static_cast<int>(position.y);
                outHit.z = static_cast<int>(position.z);
                outHit.face = hitFace;
                return true;
            }
        }
    }

    return false;
}
