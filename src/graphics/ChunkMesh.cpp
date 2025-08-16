#include "graphics/ChunkMesh.h"

Graphics::ChunkMesh::ChunkMesh(const std::vector<GLint>& vertices, const std::vector<GLuint>& indices):
	vertexArrayObject(),
	vertexBufferObject(vertices.size() * sizeof(GLint), vertices.data()),
	elementBufferObject(indices.size() * sizeof(GLuint), indices.data()),
	indexCount(indices.size())
{
	vertexArrayObject.Bind();
	vertexBufferObject.Bind();
	elementBufferObject.Bind();
	vertexArrayObject.LinkAttribI(vertexBufferObject, 0, 1, GL_INT, sizeof(GLint), (void*)0);

	vertexArrayObject.Unbind();
	vertexBufferObject.Unbind();
	elementBufferObject.Unbind();
}


Graphics::ChunkMesh::~ChunkMesh() {
	vertexBufferObject.Delete();
	elementBufferObject.Delete();
	vertexArrayObject.Delete();
}


void Graphics::ChunkMesh::Draw() const {
	vertexArrayObject.Bind();
	elementBufferObject.Bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}