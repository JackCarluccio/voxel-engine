#include "graphics/EBO.h"

// Constructor that generates a Elements Buffer Object and links it to indices
Graphics::EBO::EBO(GLsizeiptr size, const void* data) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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
void Graphics::EBO::Delete() {
	if (id != 0) {
		glDeleteBuffers(1, &id);
		id = 0;
	}
}

// Steal from another EBO by taking its ID
void Graphics::EBO::steal(EBO& other) noexcept {
	id = other.id;
	other.id = 0; // Set the other's id to 0 to avoid double deletion
}