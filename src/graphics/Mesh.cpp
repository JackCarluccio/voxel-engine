#include "graphics/Mesh.h"

Graphics::Mesh::Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices): 
	vertexArrayObject(), vertexBufferObject(vertices), elementBufferObject(indices), vertices(vertices), indices(indices)
{
	vertexArrayObject.Bind();
	vertexBufferObject.Bind();
	elementBufferObject.Bind();
	vertexArrayObject.LinkAttrib(vertexBufferObject, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), nullptr);

	vertexArrayObject.Unbind();
	vertexBufferObject.Unbind();
	elementBufferObject.Unbind();
}


Graphics::Mesh::~Mesh() {
	vertexBufferObject.Delete();
	elementBufferObject.Delete();
	vertexArrayObject.Delete();
}


void Graphics::Mesh::Draw() {
	vertexArrayObject.Bind();
	elementBufferObject.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}