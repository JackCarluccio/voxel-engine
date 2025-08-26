#pragma once

#include "world/blocks/Blocks.h"

namespace World::Biomes {
	
	struct BiomeData {
		Blocks::BlockType surfaceBlock;
		Blocks::BlockType subsurfaceBlock;
	};

	enum Biome {
		Plains = 0,
		Desert,
		Tundra,
	};

	extern BiomeData biomeData[16];
	
	Biome DetermineBiome(float temperature, float humidity) noexcept;

}
