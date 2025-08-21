#include "worldGen/WorldGen.h"

WorldGen::PerlinNoise2d terrainNoise(4, 2.0f, 0.5f);

namespace WorldGen {
	Chunk GenerateChunk(const glm::ivec2& position) {
		Chunk chunk(position);

		int chunkWorldX = position.x * width;
		int chunkWorldZ = position.y * width;

		// Generate a heightmap which controls the height of the terrain
		int heightMap[area];
		for (int x = 0; x < width; x++)
		for (int z = 0; z < width; z++) {
			float noise = terrainNoise.Sample(
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

			// Set surface block
			if (height > 0) {
				chunk.SetBlock(x, height - 1, z, Blocks::GRASS);
			}
			
			// Set subsurface blocks
			for (int y = height - 4; y < height - 1; y++) {
				chunk.SetBlock(x, y, z, Blocks::DIRT);
			}

			// Set all stone blocks
			for (int y = 0; y < height - 4; y++) {
				chunk.SetBlock(x, y, z, Blocks::STONE);
			}
		}

		return chunk;
	}
}