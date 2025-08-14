#pragma once

#include <vector>
#include <glad/glad.h>

namespace Graphics {

class EBO {
public:
	EBO(const std::vector<GLuint>& indices);
	~EBO();

	void Bind() const;
	void Unbind() const;
	void Delete() const;

	GLuint GetId() const { return id; }

private:
	GLuint id;
};

}