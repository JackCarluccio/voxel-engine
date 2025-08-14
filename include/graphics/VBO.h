#pragma once

#include <vector>
#include <glad/glad.h>

namespace Graphics {

class VBO {
public:
	VBO(const std::vector<GLfloat>& vertices);
	~VBO();

	void Bind() const;
	void Unbind() const;
	void Delete() const;

	inline GLuint GetId() const { return id; }

private:
	GLuint id;
};

}