#pragma once

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include "world/chunks/Chunk.h"
#include "world/blocks/Blocks.h"
#include "world/biomes/Biomes.h"
#include "world/generation/Splines.h"
#include "world/generation/PerlinNoise.h"

namespace World::Generation::Generator {

	constexpr int surfaceLevel = 0;

	Chunks::Chunk GenerateChunk(const glm::ivec2& coord);

}
