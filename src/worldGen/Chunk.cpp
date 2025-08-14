#include "worldGen/Chunk.h"

void WorldGen::Chunk::BuildMesh(std::vector<GLint>& vertices, std::vector<GLuint>& indices) const {
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

	vertices.push_back(WorldGen::GetVertexIndex(0, 0, 0));
	vertices.push_back(WorldGen::GetVertexIndex(1, 0, 0));
	vertices.push_back(WorldGen::GetVertexIndex(1, 0, 1));
	vertices.push_back(WorldGen::GetVertexIndex(0, 0, 1));

	/*vertices.push_back(WorldGen::GetVertexIndex(0, 0, 0));
	vertices.push_back(WorldGen::GetVertexIndex(1, 0, 0));
	vertices.push_back(WorldGen::GetVertexIndex(1, 0, 1));
	vertices.push_back(WorldGen::GetVertexIndex(0, 0, 1));*/

	/*vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(2);
	vertices.push_back(3);*/

	// Down left triangle
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// Up right triangle
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
}