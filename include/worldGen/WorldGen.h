#pragma once

#include <glm/glm.hpp>
#include <algorithm>

#include "worldGen/Chunk.h"
#include "worldGen/Computed.h"
#include "worldGen/PerlinNoise.h"

namespace WorldGen {
	static constexpr int surfaceLevel = 0;

	Chunk GenerateChunk(const glm::ivec2& position);
}