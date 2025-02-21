#pragma once

#include <glad/glad.h>

class EBO {
public:
	EBO(const GLuint* indices, GLsizeiptr size);
	~EBO();

	void Bind();
	void Unbind();
	void Delete();

	GLuint GetId() const { return id; }

private:
	GLuint id;
};