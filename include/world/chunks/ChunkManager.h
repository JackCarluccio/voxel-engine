#pragma once

#include <glm/glm.hpp>

#include <array>
#include <unordered_map>

#include "graphics/ChunkMesh.h"
#include "world/chunks/Chunk.h"
#include "world/generation/Generator.h"

struct IVec2Hash {
	std::size_t operator()(const glm::ivec2& key) const noexcept {
		return std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1);
	}
};
struct IVec2Equal {
	bool operator()(const glm::ivec2& a, const glm::ivec2& b) const noexcept {
		return a.x == b.x && a.y == b.y;
	}
};

namespace World::Chunks::ChunkManager {

	extern std::unordered_map<glm::ivec2, Chunks::Chunk, IVec2Hash, IVec2Equal> chunks;
	extern std::unordered_map<glm::ivec2, Graphics::ChunkMesh, IVec2Hash, IVec2Equal> meshes;

	bool HasAllNeighbors(const glm::ivec2& chunkCoord) noexcept;
	std::array<Chunks::Chunk*, 4> GetNeighbors(const glm::ivec2& chunkCoord) noexcept;

	inline bool DoesChunkExist(const glm::ivec2& chunkCoord) noexcept {
		return chunks.find(chunkCoord) != chunks.end();
	}
	inline bool DoesChunkMeshExist(const glm::ivec2& chunkCoord) noexcept {
		return meshes.find(chunkCoord) != meshes.end();
	}

	bool CreateChunk(const glm::ivec2& chunkCoord);

}
