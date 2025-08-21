#include "world/blocks/Blocks.h"

namespace World::Blocks {

	BlockData blockData[256] = {
		{ 0.0000f, 0.0000f }, // Air
		{ 0.0000f, 0.0000f }, // Stone
		{ 0.0625f, 0.0000f }, // Grass
		{ 0.0625f * 2, 0.0000f }, // Dirt
		// Add more blocks as needed...
	};

}