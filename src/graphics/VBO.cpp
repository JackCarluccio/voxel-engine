#include "graphics/VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
Graphics::VBO::VBO(const std::vector<GLint>& vertices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLint), &vertices[0], GL_STATIC_DRAW);
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
void Graphics::VBO::Delete() {
	if (id != 0) {
		glDeleteBuffers(1, &id);
		id = 0;
	}
}

// Steal from another VBO by taking its ID
void Graphics::VBO::steal(VBO& other) noexcept {
	id = other.id;
	other.id = 0; // Set the other's id to 0 to avoid double deletion
}