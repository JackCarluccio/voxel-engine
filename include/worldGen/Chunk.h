#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace WorldGen {
	class Chunk {
	public:
		static constexpr int width = 16;
		static constexpr int height = 256;
		static constexpr int area = width * width;
		static constexpr int volume = area * height;

		// Helper functions
		static constexpr int IndexFromXYZ(int x, int y, int z) {
			return z + x * width + y * area;
		}

		const glm::ivec3 position;
		std::vector<uint8_t> blocks;

		Chunk(const glm::ivec3& p) : position(p), blocks(volume) {};

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

		void BuildMesh(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) const;
	};
}