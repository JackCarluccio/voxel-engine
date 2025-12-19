#pragma once

#include <cmath>
#include <cstdint>

namespace World::Generation::PerlinNoise {
	
	float Sample2d(float x, float y) noexcept;

	class PerlinNoise2d {
	public:
		const int octaves;
		const float lacunarity;
		const float persistence;

		PerlinNoise2d(float frequency, int octaves, float lacunarity, float persistence);

		float Sample(float x, float y) const noexcept;

	private:
		const float frequency;
		const float maxNoiseRecip;
	};

}