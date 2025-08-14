#include "worldGen/Chunk.h"

void WorldGen::Chunk::BuildMesh(std::vector<Graphics::Vertex>& vertices, std::vector<GLuint>& indices) const {
	vertices.clear();
	indices.clear();

	//for (int y = 0; y < height; y++)
	//	for (int x = 0; x < width; x++)
	//		for (int z = 0; z < width; z++) {
	//			if (GetBlock(x, y, z) != 0) {
	//				// Add vertices for the cube
	//				AddCubeVertices(vertices, x, y, z);
	//			}
	//		}
	//	}
	//}

	vertices.push_back(Graphics::Vertex{ 0.0f, 0.0f, 0.0f }); // Back left vertex
	vertices.push_back(Graphics::Vertex{ 1.0f, 0.0f, 0.0f }); // Back right vertex
	vertices.push_back(Graphics::Vertex{ 0.0f, 0.0f, 1.0f }); // Front left vertex
	vertices.push_back(Graphics::Vertex{ 1.0f, 0.0f, 1.0f }); // Front right vertex

	// Down left triangle
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// Up right triangle
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
}