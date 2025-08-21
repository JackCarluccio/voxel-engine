#include "world/generation/Generator.h"

World::Generation::PerlinNoise::PerlinNoise2d terrainNoise(4, 2.0f, 0.5f);

namespace World::Generation::Generator {

	// Generates a chunk at the specified position.
	// Includes chunk data, meshing, and meshing neighbors.
	Chunks::Chunk GenerateChunk(const glm::ivec2& position) {
		Chunks::Chunk chunk(position);

		int chunkWorldX = position.x * Chunks::width;
		int chunkWorldZ = position.y * Chunks::width;

		// Generate a heightmap which controls the height of the terrain
		int heightMap[Chunks::area];
		for (int x = 0; x < Chunks::width; x++)
		for (int z = 0; z < Chunks::width; z++) {
			float noise = terrainNoise.Sample(
				static_cast<float>(chunkWorldX + x) / 128.0f,
				static_cast<float>(chunkWorldZ + z) / 128.0f
			);

			float height = noise * 96.0f + 100.0f; // Scale the noise to a reasonable height
			heightMap[z + x * Chunks::width] = std::clamp(static_cast<int>(height), 2, 250);
		}

		// Set each block below the terrain height to stone
		for (int x = 0; x < Chunks::width; x++)
		for (int z = 0; z < Chunks::width; z++) {
			int height = heightMap[z + x * Chunks::width];

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
