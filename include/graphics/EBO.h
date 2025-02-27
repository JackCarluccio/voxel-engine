#pragma once

#include <vector>
#include <glad/glad.h>

namespace Graphics {

class EBO {
public:
	EBO(const std::vector<GLuint>& indices);
	~EBO();

	void Bind();
	void Unbind();
	void Delete();

	GLuint GetId() const { return id; }

private:
	GLuint id;
};

}