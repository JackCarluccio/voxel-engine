#pragma once

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "util/IndexableMinHeap.h"
#include "util/IVec2Hash.h"
#include "util/IVec2Equal.h"

namespace World::Chunks::ChunkScheduler {

    void schedule(const glm::ivec2 chunkPos) noexcept;
    void unschedule(const glm::ivec2 chunkPos) noexcept;

    bool hasNextChunkToGenerate() noexcept;
    glm::ivec2 getNextChunkToGenerate() noexcept;

}
