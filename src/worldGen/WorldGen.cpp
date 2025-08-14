#include "worldGen/WorldGen.h"

using namespace WorldGen;

namespace WorldGen {
	Chunk GenerateChunk(const glm::ivec3& position) {
		Chunk chunk(position);

		int chunkWorldX = position.x * Chunk::width;
		int chunkWorldZ = position.z * Chunk::width;

		for (int y = 0; y < surfaceLevel; y++) {
			for (int x = 0; x < Chunk::width; x++) {
				for (int z = 0; z < Chunk::width; z++) {
					int heightOffset = chunkWorldX + x + chunkWorldZ + z;
					chunk.SetBlock(x, y, z, y <= surfaceLevel + heightOffset);
				}
			}
		}

		return chunk;
	}
}