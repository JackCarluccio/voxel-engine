#include "world/chunks/ChunkManager.h"

bool AttemptMeshCreation(const World::Chunks::Chunk& chunk) {
	// All neighbors must exist to construct a mesh
	if (!World::Chunks::ChunkManager::HasAllNeighbors(chunk.coord))
		return false;

	std::array<World::Chunks::Chunk*, 4> neighbors = World::Chunks::ChunkManager::GetNeighbors(chunk.coord);
	Graphics::ChunkMesh mesh = Graphics::ChunkMesh::BuildChunkMesh(chunk, neighbors.data());
	World::Chunks::ChunkManager::meshes.insert({ chunk.coord, std::move(mesh) });
	return true;
}

namespace World::Chunks::ChunkManager {

	std::unordered_map<glm::ivec2, Chunk, Util::IVec2Hash, Util::IVec2Equal> chunks;
	std::unordered_map<glm::ivec2, Graphics::ChunkMesh, Util::IVec2Hash, Util::IVec2Equal> meshes;

	// Checks if a chunk has all 4 neighbors loaded
	bool HasAllNeighbors(const glm::ivec2& chunkCoord) noexcept {
		for (const glm::ivec2& offset : chunkOffsets) {
			glm::ivec2 neighborCoord = chunkCoord + offset;
			if (chunks.find(neighborCoord) == chunks.end()) {
				return false;
			}
		}

		return true;
	}

	// Get pointers to the neighboring 4 chunks from the specified chunk. Nullptr if they don't exist
	std::array<Chunk*, 4> GetNeighbors(const glm::ivec2& chunkCoord) noexcept {
		std::array<Chunk*, 4> ptrs = { nullptr, nullptr, nullptr, nullptr };
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

		chunks.insert({ chunkCoord, Generation::Generator::GenerateChunk(chunkCoord) });
		Chunk& chunk = chunks[chunkCoord];

		AttemptMeshCreation(chunk);
		// Neighboring chunks might have been waiting for this chunk, to create their meshes
		for (const glm::ivec2& offset : chunkOffsets) {
			glm::ivec2 neighborCoord = chunkCoord + offset;
			if (DoesChunkExist(neighborCoord) && !DoesChunkMeshExist(neighborCoord)) {
				AttemptMeshCreation(chunks[neighborCoord]);
			}
		}

		return true;
	}

}
