#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "graphics/ChunkMesh.h"
#include "worldGen/Computed.h"

namespace WorldGen {
	class Chunk {
	public:
		// Helper functions
		static constexpr int IndexFromXYZ(int x, int y, int z) {
			return z + x * width + y * area;
		}

		const glm::ivec3 position;
		std::vector<uint8_t> blocks;

		Chunk(const glm::ivec3& p) : position(p), blocks(volume) {};

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

		Graphics::ChunkMesh BuildMesh() const;
	};
}