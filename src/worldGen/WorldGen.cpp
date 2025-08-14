#include "worldGen/WorldGen.h"

using namespace WorldGen;

namespace WorldGen {
	Chunk GenerateChunk(const glm::ivec3& position) {
		Chunk chunk(position);

		int chunkWorldX = position.x * width;
		int chunkWorldZ = position.z * width;

		for (int y = 0; y < surfaceLevel; y++) {
			for (int x = 0; x < width; x++) {
				for (int z = 0; z < width; z++) {
					int heightOffset = chunkWorldX + x + chunkWorldZ + z;
					chunk.SetBlock(x, y, z, y <= surfaceLevel + heightOffset);
				}
			}
		}

		return chunk;
	}
}