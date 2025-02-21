#pragma once

#include <vector>
#include <glad/glad.h>
#include "shaderProgram.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Mesh {
public:
	Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
	~Mesh();

	void Draw(ShaderProgram&);

private:
	VAO vertexArrayObject;
	VBO vertexBufferObject;
	EBO elementBufferObject;

	const std::vector<GLfloat>& vertices;
	const std::vector<GLuint>& indices;
};