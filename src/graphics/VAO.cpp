#include "graphics/VAO.h"

// Constructor that generates a VAO ID
Graphics::VAO::VAO() {
	glGenVertexArrays(1, &id);
}

// Links a VBO attribute
void Graphics::VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Links a VBO integer attribute
void Graphics::VAO::LinkAttribI(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribIPointer(layout, numComponents, type, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Links a VBO attribute without binding/unbinding the VBO
void Graphics::VAO::LinkAttribNoBinding(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
}

// Links a VBO integer attribute without binding/unbinding the VBO
void Graphics::VAO::LinkAttribINoBinding(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	glVertexAttribIPointer(layout, numComponents, type, stride, offset);
	glEnableVertexAttribArray(layout);
}

// Binds the VAO
void Graphics::VAO::Bind() const {
	glBindVertexArray(id);
}

// Unbinds the VAO
void Graphics::VAO::Unbind() const {
	glBindVertexArray(0);
}

// Deletes the VAO
void Graphics::VAO::Delete() {
	if (id != 0) {
		glDeleteVertexArrays(1, &id);
		id = 0;
	}
}

// Steal from another VAO by taking its ID
void Graphics::VAO::steal(VAO& other) noexcept {
	id = other.id;
	other.id = 0;
}