#pragma once

#include <glad/glad.h>
#include "graphics/VBO.h"

namespace Graphics {

class VAO {
public:
	VAO();
	~VAO();

	void Bind();
	void Unbind();
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Delete();

	GLuint GetId() const { return id; }

private:
	GLuint id;
};

}
