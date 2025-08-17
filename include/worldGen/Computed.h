#pragma once

namespace WorldGen {
	// Chunk measurement properties
	inline constexpr int width = 16;
	inline constexpr int height = 256;
	inline constexpr int area = width * width;
	inline constexpr int volume = area * height;

	// How much to offset the index to get to a neighbor block
	inline constexpr int neighborIndexOffsets[6] = {
		-1, 1,
		-width, width,
		-area, area
	};
}