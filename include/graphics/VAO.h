#pragma once

#include <glad/glad.h>
#include "graphics/VBO.h"

namespace Graphics {

class VAO {
public:
	VAO();
	~VAO();

	void Bind() const;
	void Unbind() const;
	void Delete() const;

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void LinkAttribI(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	GLuint GetId() const { return id; }

private:
	GLuint id;
};

}
