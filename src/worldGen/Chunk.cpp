#include "worldGen/Chunk.h"

// ChunkMesh's vertex measurement properties
constexpr int vertexWidth = WorldGen::width + 1;
constexpr int vertexArea = vertexWidth * vertexWidth;
constexpr int GetVertexIndex(int x, int y, int z) noexcept {
	return z + x * vertexWidth + y * vertexArea;
}

struct FaceVertexInfo {
	GLuint vertexIndexOffset;
	GLfloat u, v;
};
constexpr FaceVertexInfo faceVertexInfo[6][4] = {
	{ // Back face
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.00f, 0.00f },
	},
	{ // Front face
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.00f, 0.00f },
	},
	{ // Right face ( cuz someone decided to flip x axis )
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.00f, 0.00f },
	},
	{ // Left face ( cuz someone decided to flip x axis )
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.00f, 0.00f },
	},
	{ // Bottom face
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.00f, 0.00f },
	},
	{ // Top face
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.00f, 0.00f },
	}
};

Graphics::ChunkMesh WorldGen::Chunk::BuildMesh() const {
	std::vector<Graphics::VertexData> vertices;
	std::vector<GLuint> indices;

	for (int y = 1; y < height - 1; y++)
	for (int x = 1; x < width - 1; x++)
	for (int z = 1; z < width -1 ; z++) {
		int index = IndexFromXYZ(x, y, z);
		if (GetBlock(index) == 0)
			continue;

		// Construct each face
		for (int i = 0; i < 6; i++) {
			// Only construct this face if not occluded
			int neighborIndex = index + neighborIndexOffsets[i];
			if (GetBlock(neighborIndex) != 0)
				continue;
			
			// Add each vertex
			int originVertexIndex = GetVertexIndex(x, y, z);
			for (int j = 0; j < 4; j++) {
				FaceVertexInfo info = faceVertexInfo[i][j];
				vertices.emplace_back(
					originVertexIndex + info.vertexIndexOffset,
					info.u, info.v
				);
			}

			// Construct the two triangles
			int indexOffset = vertices.size() - 4;
			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 1);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 3);			
		}
	}

	return Graphics::ChunkMesh(vertices, indices);
}