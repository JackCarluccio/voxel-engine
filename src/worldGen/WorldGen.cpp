#include "worldGen/WorldGen.h"

using namespace WorldGen;

namespace WorldGen {
	Chunk GenerateChunk(const glm::ivec2& position) {
		Chunk chunk(position);

		int chunkWorldX = position.x * width;
		int chunkWorldZ = position.y * width;

		// Generate a heightmap which controls the height of the terrain
		int heightMap[area];
		for (int x = 0; x < width; x++)
		for (int z = 0; z < width; z++) {
			float noise = PerlinNoise2d(
				static_cast<float>(chunkWorldX + x) / 128.0f,
				static_cast<float>(chunkWorldZ + z) / 128.0f
			);

			float height = noise * 96.0f + 100.0f; // Scale the noise to a reasonable height
			heightMap[z + x * width] = std::clamp(static_cast<int>(height), 2, 250);
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