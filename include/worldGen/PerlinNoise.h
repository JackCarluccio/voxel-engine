#pragma once

#include <cmath>

namespace WorldGen {
	extern constexpr float GetMaxNoise(int octaves, float persistence) noexcept;

	class PerlinNoise2d {
	public:
		const int octaves;
		const float lacunarity;
		const float persistence;

		PerlinNoise2d(int octaves, float lacunarity, float persistence);

		float Sample(float x, float y) const noexcept;

	private:
		const float maxNoise;
	};

	float Sample2d(float x, float y) noexcept;
}