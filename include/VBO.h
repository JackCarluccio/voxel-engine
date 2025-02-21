#pragma once

#include<glad/glad.h>

class VBO {
public:
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(const GLfloat* vertices, GLsizeiptr size);
	~VBO();

	void Bind();
	void Unbind();
	void Delete();

	GLuint GetId() const { return id; }

private:
	GLuint id;
};