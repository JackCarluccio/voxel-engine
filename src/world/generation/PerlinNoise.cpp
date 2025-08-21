/*
Simple 2d perlin noise implementation from https://www.youtube.com/@zipped1214
*/

#include "world/generation/PerlinNoise.h"

struct Vector2 {
	float x, y;
};

// Cubic interpolation
constexpr float Interpolate(float a, float b, float t) noexcept {
	return (b - a) * (3.0f - t * 2.0f) * t * t + a;
}

Vector2 RandomGradient(int x, int y) noexcept {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = x, b = y;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    return Vector2{
		cos(random),
		sin(random)
    };
}

float DotGridGradient(int cellX, int cellY, float x, float y) noexcept {
    Vector2 gradient = RandomGradient(cellX, cellY);

    float dx = x - static_cast<float>(cellX);
	float dy = y - static_cast<float>(cellY);

    return (dx * gradient.x + dy * gradient.y);
}

// Computes the maximum possible value for the given number of octaves and persistence
constexpr float GetMaxNoise(int octaves, float persistence) noexcept {
	float maxNoise = 1.0f;
	float amplitude = 1.0f;
	for (int i = 1; i < octaves; i++) {
		amplitude *= persistence;
		maxNoise += amplitude;
	}

	return maxNoise;
}

namespace World::Generation::PerlinNoise {

	// Samples a 2d perlin noise at the specified coordinates
	float Sample2d(float x, float y) noexcept {
		// Surrounding grid points
		int x0 = static_cast<int>(floor(x));
		int y0 = static_cast<int>(floor(y));
		int x1 = x0 + 1;
		int y1 = y0 + 1;

		// Interpolation weights
		float tx = x - x0;
		float ty = y - y0;

		// Interpolate between the gradients at the top of the cell
		float n0 = DotGridGradient(x0, y0, x, y);
		float n1 = DotGridGradient(x1, y0, x, y);
		float ix0 = Interpolate(n0, n1, tx);

		// Interpolate between the gradients at the bottom of the cell
		float n2 = DotGridGradient(x0, y1, x, y);
		float n3 = DotGridGradient(x1, y1, x, y);
		float ix1 = Interpolate(n2, n3, tx);

		// Interpolate between the top and bottom gradients
		return Interpolate(ix0, ix1, ty);
	}

	// Creates a PerlinNoise2d instance with the specified parameters
	PerlinNoise2d::PerlinNoise2d(int octaves, float lacunarity, float persistence) :
		octaves(octaves), lacunarity(lacunarity), persistence(persistence),
		maxNoise(GetMaxNoise(octaves, persistence))
	{}

	// Samples the perlin noise at the specified coordinates. Layers multiple octaves together
	float PerlinNoise2d::Sample(float x, float y) const noexcept {
		float total = 0.0f;
		float frequency = 1.0f;
		float amplitude = 1.0f;
		for (int i = 0; i < octaves; i++) {
			total += Sample2d(x * frequency, y * frequency) * amplitude;
			frequency *= lacunarity;
			amplitude *= persistence;
		}

		return total / maxNoise;
	}

}
