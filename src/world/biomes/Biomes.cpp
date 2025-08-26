#include "world/biomes/Biomes.h"

namespace World::Biomes {

	BiomeData biomeData[16] = {
		{ Blocks::GRASS, Blocks::DIRT },
		{ Blocks::SAND, Blocks::SANDSTONE },
		{ Blocks::SNOW, Blocks::DIRT },
	};

	Biome DetermineBiome(float temperature, float humidity) noexcept {
		if (temperature < -0.333f) {
			return Tundra;
		} else if (temperature < 0.333f) {
			return Plains;
		} else {
			return Desert;
		}
	}

}
