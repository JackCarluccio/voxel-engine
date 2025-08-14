#include "graphics/VAO.h"

// Constructor that generates a VAO ID
Graphics::VAO::VAO() {
	glGenVertexArrays(1, &id);
}


Graphics::VAO::~VAO() {
	Delete();
}

// Links a VBO Attribute such as a position or color to the VAO
void Graphics::VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void Graphics::VAO::LinkAttribI(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribIPointer(layout, numComponents, type, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
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
void Graphics::VAO::Delete() const {
	glDeleteVertexArrays(1, &id);
}