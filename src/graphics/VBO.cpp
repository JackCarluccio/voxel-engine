#include "graphics/VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
Graphics::VBO::VBO(const std::vector<GLfloat>& vertices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
}


Graphics::VBO::~VBO() {
	Delete();
}

// Binds the VBO
void Graphics::VBO::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

// Unbinds the VBO
void Graphics::VBO::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void Graphics::VBO::Delete() const {
	glDeleteBuffers(1, &id);
}