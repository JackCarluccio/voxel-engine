#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &id);
}


VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() {
	glBindVertexArray(id);
}

// Unbinds the VAO
void VAO::Unbind() {
	glBindVertexArray(0);
}