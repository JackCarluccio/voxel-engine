#pragma once

namespace World::Blocks {
	struct BlockData {
		float u, v;
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
		COBBLESTONE,
		BEDROCK,
	};

	extern BlockData blockData[256];
}