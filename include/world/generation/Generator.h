#pragma once

#include <glm/glm.hpp>
#include <algorithm>

#include "world/chunks/Chunk.h"
#include "world/blocks/Blocks.h"
#include "world/generation/PerlinNoise.h"

namespace World::Generation::Generator {

	constexpr int surfaceLevel = 0;

	Chunks::Chunk GenerateChunk(const glm::ivec2& position);

}
