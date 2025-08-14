#include "worldGen/WorldGen.h"

namespace WorldGen {
	WorldGen::Chunk GenerateChunk(const glm::ivec3& position) {
		WorldGen::Chunk chunk(position);

		for (int y = 0; y < WorldGen::surfaceLevel; y++) {
			for (int x = 0; x < WorldGen::Chunk::width; x++) {
				for (int z = 0; z < WorldGen::Chunk::width; z++) {
					chunk.SetBlock(x, y, z, 1);
				}
			}
		}

		return chunk;
	}
}