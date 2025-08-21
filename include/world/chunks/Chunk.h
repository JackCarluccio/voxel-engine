#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

namespace World::Chunks {

	// Chunk measurement properties
	constexpr int width = 16;
	constexpr int height = 256;
	constexpr int area = width * width;
	constexpr int volume = area * height;
	constexpr int interiorVolume = (width - 2) * (width - 2) * (height - 2);
	constexpr int exteriorVolume = volume - interiorVolume;

	// Offsets to neighboring chunks
	constexpr glm::ivec2 chunkOffsets[4] = {
		glm::ivec2(0, -1), // Back
		glm::ivec2(0, 1), // Front
		glm::ivec2(-1, 0), // Right
		glm::ivec2(1, 0), // Left
	};

	// How much to offset the index when checking neighboring blocks
	inline constexpr int indexOffsets[6] = {
		-1, 1,
		-width, width,
		-area, area
	};

	// Convert 3D coordinates to a 1D index in the chunk's blocks vector
	constexpr int IndexFromXYZ(int x, int y, int z) noexcept {
		return z + x * width + y * area;
	}

	// Checks if the block is within the bounds of a chunk
	constexpr bool InBounds(int x, int y, int z) noexcept {
		return x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < width;
	}
	// Checks if the block is an interior block of a chunk
	constexpr bool IsInterior(int x, int y, int z) noexcept {
		return x > 0 && x < width - 1 && y > 0 && y < height - 1 && z > 0 && z < width - 1;
	}
	// Checks if the block is an exterior block of the chunk
	constexpr bool IsExterior(int x, int y, int z) noexcept {
		return !IsInterior(x, y, z) && InBounds(x, y, z);
	}
	// Checks if the block on this face of a block is in bounds
	constexpr bool IsBlockFaceInBounds(int x, int y, int z, int face) noexcept {
		return IsInterior(x, y, z) || (InBounds(x, y, z) && !(
			(z == 0 && face == 0) ||
			(z == width - 1 && face == 1) ||
			(x == 0 && face == 2) ||
			(x == width - 1 && face == 3) ||
			(y == 0 && face == 4) ||
			(y == height - 1 && face == 5)
		));
	}

	class Chunk {
	public:

		const glm::ivec2 position;
		std::vector<uint8_t> blocks;

		Chunk(const glm::ivec2& p) : position(p), blocks(volume) {};
		Chunk() : position(glm::ivec2(0, 0)), blocks(volume) { };

		// Disable copying.
		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;

		Chunk(Chunk&& other) noexcept :
			position(other.position),
			blocks(std::move(other.blocks))
		{
			other.blocks.clear(); // Clear the moved-from chunk's blocks
		}
		Chunk& operator=(Chunk&&) = delete;

		// GetBlock methods
		inline uint8_t GetBlock(int index) const noexcept {
			return blocks[index];
		};
		inline uint8_t GetBlock(int x, int y, int z) const noexcept {
			int index = IndexFromXYZ(x, y, z);
			return GetBlock(index);
		};

		// SetBlock methods
		inline void SetBlock(int index, uint8_t block) noexcept {
			blocks[index] = block;
		};
		inline void SetBlock(int x, int y, int z, uint8_t block) noexcept {
			int index = IndexFromXYZ(x, y, z);
			SetBlock(index, block);
		};
	};

}
