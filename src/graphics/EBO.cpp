#include "graphics/EBO.h"

// Constructor that generates a Elements Buffer Object and links it to indices
Graphics::EBO::EBO(const std::vector<GLuint>& indices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}


Graphics::EBO::~EBO() {
	Delete();
}

// Binds the EBO
void Graphics::EBO::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

// Unbinds the EBO
void Graphics::EBO::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void Graphics::EBO::Delete() const {
	glDeleteBuffers(1, &id);
}