#pragma once

namespace WorldGen::Blocks {
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

	inline constexpr BlockData blockData[256] = {
		{ 0.0000f, 0.0000f }, // Air
		{ 0.0000f, 0.0000f }, // Stone
		{ 0.0625f, 0.0000f }, // Grass
		{ 0.0625f * 2, 0.0000f }, // Dirt
		// Add more blocks as needed...
	};
}