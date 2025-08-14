#pragma once

#include <glm/glm.hpp>
#include <worldGen/Chunk.h>

namespace WorldGen {
	static constexpr int surfaceLevel = 16;

	Chunk GenerateChunk(const glm::ivec3& position);
}