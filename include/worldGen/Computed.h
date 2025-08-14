#pragma once

namespace WorldGen {
	inline constexpr int width = 16;
	inline constexpr int height = 256;
	inline constexpr int area = width * width;
	inline constexpr int volume = area * height;

	inline constexpr int vertexWidth = width + 1;
	inline constexpr int vertexArea = vertexWidth * vertexWidth;

	inline constexpr int GetVertexIndex(int x, int y, int z) noexcept {
		return z + x * vertexWidth + y * vertexArea;
	}
}