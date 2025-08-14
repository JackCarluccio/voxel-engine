#pragma once

namespace WorldGen {
	inline constexpr int width = 16;
	inline constexpr int height = 256;
	inline constexpr int area = width * width;
	inline constexpr int volume = area * height;

	// How much to offset the index to get to a neighbor block
	inline constexpr int neighborIndexOffsets[6] = {
		-1, 1,
		-width, width,
		-area, area
	};

	inline constexpr int vertexWidth = width + 1;
	inline constexpr int vertexArea = vertexWidth * vertexWidth;

	inline constexpr int GetVertexIndex(int x, int y, int z) noexcept {
		return z + x * vertexWidth + y * vertexArea;
	}

	inline constexpr int faceVertexIndexOffsets[6][4] = {
		{ // Back face
			GetVertexIndex(1, 0, 0),
			GetVertexIndex(0, 0, 0),
			GetVertexIndex(0, 1, 0),
			GetVertexIndex(1, 1, 0),
		},
		{ // Front face
			GetVertexIndex(0, 0, 1),
			GetVertexIndex(1, 0, 1),
			GetVertexIndex(1, 1, 1),
			GetVertexIndex(0, 1, 1),
		},
		{ // Right face ( cuz someone decided to flip x axis )
			GetVertexIndex(0, 0, 0),
			GetVertexIndex(0, 0, 1),
			GetVertexIndex(0, 1, 1),
			GetVertexIndex(0, 1, 0),
		},
		{ // Left face ( cuz someone decided to flip x axis )
			GetVertexIndex(1, 0, 1),
			GetVertexIndex(1, 0, 0),
			GetVertexIndex(1, 1, 0),
			GetVertexIndex(1, 1, 1),
		},
		{ // Bottom face
			GetVertexIndex(1, 0, 1),
			GetVertexIndex(0, 0, 1),
			GetVertexIndex(0, 0, 0),
			GetVertexIndex(1, 0, 0),
		},
		{ // Top face
			GetVertexIndex(1, 1, 0),
			GetVertexIndex(0, 1, 0),
			GetVertexIndex(0, 1, 1),
			GetVertexIndex(1, 1, 1),
		}
	};
}