#include "world/generation/Generator.h"

using namespace World::Generation;

constexpr float erosionFrequency = 1.0f / 256.0f;
constexpr float weirdnessFrequency = 1.0f / 64.0f;
constexpr float continentalnessFrequency = 1.0f / 128.0f;
PerlinNoise::PerlinNoise2d erosionNoise(erosionFrequency, 3, 2.0f, 0.5f);
PerlinNoise::PerlinNoise2d weirdnessNoise(weirdnessFrequency, 4, 2.0f, 0.5f);
PerlinNoise::PerlinNoise2d continentalnessNoise(continentalnessFrequency, 4, 2.0f, 0.5f);
PerlinNoise::PerlinNoise2d temperatureNoise(1.0f / 256.0f, 1, 2.0f, 0.5f);
PerlinNoise::PerlinNoise2d humidityNoise(1.0f / 256.0f, 1, 2.0f, 0.5f);

std::vector<Splines::SplinePoint> erosionPoints = {
	{ -1.000f, 1.000f },
	{ -0.900f, 0.750f },
	{ -0.500f, 0.500f },
	{ -0.400f, 0.580f },
	{ 0.000f, 0.150f },
	{ 0.400f, 0.120f },
	{ 0.600f, 0.120f },
	{ 0.700f, 0.333f },
	{ 0.800f, 0.333f },
	{ 0.900f, 0.120f },
	{ 1.000f, 0.000f },
};
Splines::LinearSpline erosionSpline(erosionPoints);

std::vector<Splines::SplinePoint> weirdnessPoints = {
	{ -1.000f, 0.000f },
	{ -0.333f, 0.330f },
	{ 0.000f, 0.400f },
	{ 0.666f, 0.850f },
	{ 1.000f, 1.000f },
};
Splines::LinearSpline weirdnessSpline(weirdnessPoints);

std::vector<Splines::SplinePoint> continentalnessPoints = {
	{ -1.000f, 0.000f },
	{ -0.500f, 0.000f },
	{ -0.450f, 0.400f },
	{ -0.180f, 0.400f },
	{ -0.160f, 0.800f },
	{ 0.200f, 0.900f },
	{ 1.000f, 1.000f },
};
Splines::LinearSpline continentalnessSpline(continentalnessPoints);

float SampleErosion(float x, float z) noexcept {
	float noise = erosionNoise.Sample(x, z);
	return erosionSpline.Sample(noise);
}

float SampleWeirdness(float x, float z) noexcept {
	float noise = weirdnessNoise.Sample(x, z);
	return weirdnessSpline.Sample(noise);
}

float SampleContinentalness(float x, float z) noexcept {
	float noise = continentalnessNoise.Sample(x, z);
	return continentalnessSpline.Sample(noise);
}

int mapsGenerated = 0;
double totalTime = 0.0;
namespace World::Generation::Generator {

	// Generates a chunk at the specified position.
	// Includes chunk data, meshing, and meshing neighbors.
	Chunks::Chunk GenerateChunk(const glm::ivec2& coord) {
		Chunks::Chunk chunk(coord);

		int chunkWorldX = coord.x * Chunks::width;
		int chunkWorldZ = coord.y * Chunks::width;

		float humidity = humidityNoise.Sample(chunkWorldX, chunkWorldZ);
		float temperature = temperatureNoise.Sample(chunkWorldX, chunkWorldZ);

		Biomes::Biome biome = Biomes::DetermineBiome(temperature, humidity);
		Blocks::BlockType surfaceBlock = Biomes::biomeData[biome].surfaceBlock;
		Blocks::BlockType subsurfaceBlock = Biomes::biomeData[biome].subsurfaceBlock;

		// Generate a heightmap which controls the height of the terrain
		int heightMap[Chunks::area];
		int minHeight = 256;
		for (int x = 0; x < Chunks::width; x++)
		for (int z = 0; z < Chunks::width; z++) {
			int worldX = x + chunkWorldX;
			int worldZ = z + chunkWorldZ;
			float erosion = SampleErosion(worldX, worldZ);
			float weirdness = SampleWeirdness(worldX, worldZ);
			float continentalness = SampleContinentalness(worldX, worldZ);

			int terrainHeight = 512.0f * (erosion * weirdness * continentalness);
			heightMap[z + x * Chunks::width] = std::clamp(static_cast<int>(terrainHeight), 5, 255);

			if (terrainHeight < minHeight) {
				minHeight = terrainHeight;
			}
		}

		// Fill the entire chunk with stone up to the minimum terrain height
		std::memset(chunk.blocks.data(), Blocks::STONE, static_cast<size_t>(minHeight * Chunks::area));

		// Set each block below the terrain height to stone
		for (int x = 0; x < Chunks::width; x++)
		for (int z = 0; z < Chunks::width; z++) {
			int height = heightMap[z + x * Chunks::width];

			// Set surface block
			chunk.SetBlock(x, height, z, surfaceBlock);
			
			// Set subsurface blocks
			for (int y = height - 3; y < height; y++) {
				chunk.SetBlock(x, y, z, subsurfaceBlock);
			}

			// Set all stone blocks
			for (int y = minHeight + 1; y < height - 3; y++) {
				chunk.SetBlock(x, y, z, Blocks::STONE);
			}
		}

		return chunk;
	}

}
