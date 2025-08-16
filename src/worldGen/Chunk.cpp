#include "worldGen/Chunk.h"

Graphics::ChunkMesh WorldGen::Chunk::BuildMesh() const {
	std::vector<GLint> vertices;
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
				vertices.push_back(originVertexIndex + faceVertexIndexOffsets[i][j]);
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