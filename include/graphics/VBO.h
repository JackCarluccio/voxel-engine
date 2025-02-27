#pragma once

#include <vector>
#include <glad/glad.h>

namespace Graphics {

class VBO {
public:
	VBO(const std::vector<GLfloat>& vertices);
	~VBO();

	void Bind();
	void Unbind();
	void Delete();

	GLuint GetId() const { return id; }

private:
	GLuint id;
};

}