#include "world/chunks/ChunkScheduler.h"

#include <iostream>

int nextId = 0;
Util::IndexableMinHeap chunkHeap;
std::vector<int> availableIds;
std::unordered_map<glm::ivec2, int, Util::IVec2Hash, Util::IVec2Equal> chunkPosToId;
std::unordered_map<int, glm::ivec2> idToChunkPos;

// Computes priority for a chunk based on its position (e.g., distance from origin)
float computePriority(const glm::ivec2& chunkPos) noexcept {
    return chunkPos.x * chunkPos.x + chunkPos.y * chunkPos.y;
}

namespace World::Chunks::ChunkScheduler {

    // Schedules a chunk for generation
    void schedule(const glm::ivec2 chunkPos) noexcept {
        if (chunkPosToId.find(chunkPos) != chunkPosToId.end()) {
            return;
        }

        int id;
        if (!availableIds.empty()) {
            id = availableIds.back();
            availableIds.pop_back();
        } else {
            id = nextId++;
        }

        chunkPosToId[chunkPos] = id;
        idToChunkPos[id] = chunkPos;
        chunkHeap.insert(id, computePriority(chunkPos));

        float priority = computePriority(chunkPos);
    }

    // Unschedules a chunk from generation
    void unschedule(const glm::ivec2 chunkPos) noexcept {
        auto it = chunkPosToId.find(chunkPos);
        if (it == chunkPosToId.end()) {
            return; // Not scheduled
        }

        int id = it->second;
        chunkHeap.remove(id);
        chunkPosToId.erase(it);
        idToChunkPos.erase(id);
        availableIds.push_back(id);
    }

    // Checks if there is a next chunk to generate
    bool hasNextChunkToGenerate() noexcept {
        return !chunkHeap.isEmpty();
    }

    // Retrieves and removes the next chunk to generate
    glm::ivec2 getNextChunkToGenerate() noexcept {
        if (chunkHeap.isEmpty()) {
            return glm::ivec2(0, 0);
        }

        int id = chunkHeap.pop();
        glm::ivec2 chunkPos = idToChunkPos[id];

        chunkPosToId.erase(chunkPos);
        idToChunkPos.erase(id);
        availableIds.push_back(id);

        return chunkPos;
    }

}