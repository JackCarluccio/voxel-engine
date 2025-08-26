#pragma once

#include <cstdint>

namespace World::Blocks {

	struct BlockData {
		uint32_t atlasIndex;
	};

	enum BlockType : int {
		AIR = 0,
		STONE,
		GRASS,
		DIRT,
		SAND,
		SANDSTONE,
		SNOW,
		ICE,
		BEDROCK,
		COBBLESTONE,
	};

	extern BlockData blockData[256];
}