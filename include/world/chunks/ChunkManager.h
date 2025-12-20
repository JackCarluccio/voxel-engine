#pragma once

#include <glm/glm.hpp>

#include <array>
#include <unordered_map>

#include "graphics/ChunkMesh.h"
#include "world/chunks/Chunk.h"
#include "world/generation/Generator.h"
#include "util/IVec2Hash.h"
#include "util/IVec2Equal.h"

namespace World::Chunks::ChunkManager {

	extern std::unordered_map<glm::ivec2, Chunks::Chunk, Util::IVec2Hash, Util::IVec2Equal> chunks;
	extern std::unordered_map<glm::ivec2, Graphics::ChunkMesh, Util::IVec2Hash, Util::IVec2Equal> meshes;

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
