#pragma once

#include <glm/glm.hpp>

namespace WorldGen {
	// Chunk measurement properties
	inline constexpr int width = 16;
	inline constexpr int height = 256;
	inline constexpr int area = width * width;
	inline constexpr int volume = area * height;

	inline constexpr int interiorVolume = (width - 2) * (width - 2) * (height - 2);
	inline constexpr int exteriorVolume = volume - interiorVolume;

	// How much to offset the index to get to a neighbor block
	inline constexpr int neighborIndexOffsets[6] = {
		-1, 1,
		-width, width,
		-area, area
	};

	inline constexpr glm::ivec2 chunkOffsets[4] = {
		glm::ivec2(0, -1), // Back
		glm::ivec2(0, 1), // Front
		glm::ivec2(-1, 0), // Right
		glm::ivec2(1, 0), // Left
	};
}