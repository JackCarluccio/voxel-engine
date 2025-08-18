#include "worldGen/ChunkManager.h"

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

	// Creates a chunk at the specified coordinates if it doesn't already exist
	bool CreateChunk(const glm::ivec2& chunkCoord) {
		// If the chunk already exists, don't create another
		if (chunks.find(chunkCoord) != chunks.end()) {
			return false;
		}

		WorldGen::Chunk chunk = WorldGen::GenerateChunk(chunkCoord);
		chunks.insert({ chunkCoord, std::move(chunk) });

		// Construct the mesh if all neighbors are loaded
		if (HasAllNeighbors(chunkCoord)) {
			Graphics::ChunkMesh mesh = Graphics::ChunkMesh::BuildChunkMesh(chunks[chunkCoord]);
			meshes.insert({ chunkCoord, std::move(mesh) });
		}

		// Neighboring chunks might have been waiting for this chunk, to create their meshes
		for (const glm::ivec2& offset : WorldGen::chunkOffsets) {
			glm::ivec2 neighborCoord = chunkCoord + offset;
			if (DoesChunkExist(neighborCoord) && !DoesChunkMeshExist(neighborCoord) && HasAllNeighbors(neighborCoord)) {
				Graphics::ChunkMesh mesh = Graphics::ChunkMesh::BuildChunkMesh(chunks[neighborCoord]);
				meshes.insert({ neighborCoord, std::move(mesh) });
			}
		}

		return true;
	}
}
