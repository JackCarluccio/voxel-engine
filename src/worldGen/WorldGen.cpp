#include "worldGen/WorldGen.h"

using namespace WorldGen;

namespace WorldGen {
	Chunk GenerateChunk(const glm::ivec3& position) {
		Chunk chunk(position);

		int chunkWorldX = position.x * width;
		int chunkWorldZ = position.z * width;

		// Generate a heightmap which controls the height of the terrain
		int heightMap[area];
		for (int x = 0; x < width; x++)
		for (int z = 0; z < width; z++) {
			int heightOffset = chunkWorldX + x + chunkWorldZ + z;
			heightMap[z + x * width] = surfaceLevel + heightOffset;
		}

		// Set each block below the terrain height to stone
		for (int x = 0; x < width; x++)
		for (int z = 0; z < width; z++) {
			int height = heightMap[z + x * width];
			for (int y = 0; y < height; y++) {
				chunk.SetBlock(x, y, z, 1);
			}
		}

		return chunk;
	}
}