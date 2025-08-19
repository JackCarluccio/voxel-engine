#include "worldGen/ChunkManager.h"

bool AttemptMeshCreation(const WorldGen::Chunk& chunk) {
	// All neighbors must exist to construct a mesh
	if (!WorldGen::ChunkManager::HasAllNeighbors(chunk.position))
		return false;

	std::array<WorldGen::Chunk*, 4> neighbors = WorldGen::ChunkManager::GetNeighbors(chunk.position);
	Graphics::ChunkMesh mesh = Graphics::ChunkMesh::BuildChunkMesh(chunk, neighbors.data());
	WorldGen::ChunkManager::meshes.insert({ chunk.position, std::move(mesh) });
	return true;
}

namespace WorldGen::ChunkManager {
	std::unordered_map<glm::ivec2, WorldGen::Chunk, IVec2Hash, IVec2Equal> chunks;
	std::unordered_map<glm::ivec2, Graphics::ChunkMesh, IVec2Hash, IVec2Equal> meshes;

	// Checks if a chunk has all 4 neighbors loaded
	bool HasAllNeighbors(const glm::ivec2& chunkCoord) noexcept {
		for (const glm::ivec2& offset : WorldGen::chunkOffsets) {
			glm::ivec2 neighborCoord = chunkCoord + offset;
			if (chunks.find(neighborCoord) == chunks.end()) {
				return false;
			}
		}

		return true;
	}

	// Get pointers to the neighboring 4 chunks from the specified chunk. Nullptr if they don't exist
	std::array<WorldGen::Chunk*, 4> GetNeighbors(const glm::ivec2& chunkCoord) noexcept {
		std::array<WorldGen::Chunk*, 4> ptrs = { nullptr, nullptr, nullptr, nullptr };
		for (int i = 0; i < 4; i++) {
			glm::ivec2 neighborCoord = chunkCoord + chunkOffsets[i];
			auto it = chunks.find(neighborCoord);
			if (it != chunks.end()) {
				ptrs[i] = &(it->second);
			}
		}

		return ptrs;
	}

	// Creates a chunk at the specified coordinates if it doesn't already exist
	bool CreateChunk(const glm::ivec2& chunkCoord) {
		// If the chunk already exists, don't create another
		if (chunks.find(chunkCoord) != chunks.end()) {
			return false;
		}

		chunks.insert({ chunkCoord, WorldGen::GenerateChunk(chunkCoord) });
		WorldGen::Chunk& chunk = chunks[chunkCoord];

		AttemptMeshCreation(chunk);
		// Neighboring chunks might have been waiting for this chunk, to create their meshes
		for (const glm::ivec2& offset : WorldGen::chunkOffsets) {
			glm::ivec2 neighborCoord = chunkCoord + offset;
			if (DoesChunkExist(neighborCoord) && !DoesChunkMeshExist(neighborCoord)) {
				AttemptMeshCreation(chunks[neighborCoord]);
			}
		}

		return true;
	}
}
