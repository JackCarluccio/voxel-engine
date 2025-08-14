#pragma once

#include <vector>
#include <glad/glad.h>
#include "graphics/VAO.h"
#include "graphics/VBO.h"
#include "graphics/EBO.h"

namespace Graphics {
	class Mesh {
	public:
		Mesh(const std::vector<GLint>& vertices, const std::vector<GLuint>& indices);
		~Mesh();

		void Draw();

	private:
		VAO vertexArrayObject;
		VBO vertexBufferObject;
		EBO elementBufferObject;

		const std::vector<GLint>& vertices;
		const std::vector<GLuint>& indices;
	};
}