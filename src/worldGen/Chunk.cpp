#include "worldGen/Chunk.h"

void WorldGen::Chunk::BuildMesh(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) const {
	vertices.clear();
	indices.clear();

	// Back left
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	// Back right
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	// Front left
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);

	// Front right
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);

	// Down left triangle
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// Up right triangle
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
}