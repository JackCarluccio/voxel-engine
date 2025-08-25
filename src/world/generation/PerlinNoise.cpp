#include "world/generation/PerlinNoise.h"

struct Vector2 {
	float x, y;
};

// Normalized gradient table for super fast gradients from hash
// Maintain power of 2 to bitwise AND for index, instead of modulus
alignas(64) static constexpr Vector2 kGradients8[8] = {
	{  1.0f,  0.0f}, { -1.0f,  0.0f}, { 0.0f,  1.0f}, { 0.0f, -1.0f},
	{  0.70710678f,  0.70710678f}, { -0.70710678f,  0.70710678f},
	{  0.70710678f, -0.70710678f}, { -0.70710678f, -0.70710678f},
};

// Murmur3-style fmix finalizer, good avalanche, cheap
static constexpr uint32_t Fmix32(uint32_t h) noexcept {
	h ^= h >> 16;
	h *= 0x85EBCA6Bu;
	h ^= h >> 13;
	h *= 0xC2B2AE35u;
	h ^= h >> 16;
	return h;
}

// Stateless 2D hash with an external seed
static constexpr uint32_t Hash2D(uint32_t x, uint32_t y, uint32_t seed = 0) noexcept {
	uint32_t h = x * 0x8DA6B343u;
	h ^= y * 0xD8163841u;
	h ^= seed * 0xA62FC1B5u;
	return Fmix32(h);
}

static constexpr Vector2 RandomGradient(int x, int y) noexcept {
	uint32_t hash = Hash2D(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
	return kGradients8[hash & 7u];
}

static constexpr float DotGridGradient(int cellX, int cellY, float x, float y) noexcept {
    Vector2 gradient = RandomGradient(cellX, cellY);

    float dx = x - static_cast<float>(cellX);
	float dy = y - static_cast<float>(cellY);

    return (dx * gradient.x + dy * gradient.y);
}

// Computes the maximum possible value for the given number of octaves and persistence
static constexpr float GetMaxNoise(int octaves, float persistence) noexcept {
	float maxNoise = 1.0f;
	float amplitude = 1.0f;
	for (int i = 1; i < octaves; i++) {
		amplitude *= persistence;
		maxNoise += amplitude;
	}

	return maxNoise;
}

// Cubic interpolation. No 5th order as we don't need continuous derivatives
constexpr float Smooth3(float t) noexcept {
	return t * t * (3.0f - 2.0f * t);
}

namespace World::Generation::PerlinNoise {

	// Samples a 2d perlin noise at the specified coordinates
	float Sample2d(float x, float y) noexcept {
		// Surrounding grid points
		int x0 = static_cast<int>(floorf(x));
		int y0 = static_cast<int>(floorf(y));
		int x1 = x0 + 1;
		int y1 = y0 + 1;

		// Interpolation weights
		float tx = x - static_cast<float>(x0);
		float ty = y - static_cast<float>(y0);

		// Grid gradients
		Vector2 g00 = RandomGradient(x0, y0);
		Vector2 g10 = RandomGradient(x1, y0);
		Vector2 g01 = RandomGradient(x0, y1);
		Vector2 g11 = RandomGradient(x1, y1);

		// Displacement vectors
		float dx0 = tx, dy0 = ty;
		float dx1 = tx - 1.0f, dy1 = ty - 1.0f;

		// Dot products
		float n00 = dx0 * g00.x + dy0 * g00.y;
		float n10 = dx1 * g10.x + dy0 * g10.y;
		float n01 = dx0 * g01.x + dy1 * g01.y;
		float n11 = dx1 * g11.x + dy1 * g11.y;

		// Cubic smooth weights
		float sx = Smooth3(tx);
		float sy = Smooth3(ty);

		// lerp x, then y (Horner’d FMAs if you enable FMA)
		float ix0 = n00 + (n10 - n00) * sx;
		float ix1 = n01 + (n11 - n01) * sx;
		return ix0 + (ix1 - ix0) * sy;
	}

	// Creates a PerlinNoise2d instance with the specified parameters
	PerlinNoise2d::PerlinNoise2d(float frequency, int octaves, float lacunarity, float persistence) :
		frequency(frequency), octaves(octaves), lacunarity(lacunarity), persistence(persistence),
		maxNoiseRecip(1 / GetMaxNoise(octaves, persistence))
	{}

	// Samples the perlin noise at the specified coordinates. Layers multiple octaves together
	float PerlinNoise2d::Sample(float x, float y) const noexcept {
		float total = 0.0f;
		float thisFrequency = frequency;
		float thisAmplitude = 1.0f;
		for (int i = 0; i < octaves; i++) {
			total += Sample2d(x * thisFrequency, y * thisFrequency) * thisAmplitude;
			thisFrequency *= lacunarity;
			thisAmplitude *= persistence;
		}

		return total * maxNoiseRecip;
	}

}
