#pragma once

#include <glad/glad.h>
#include "graphics/VBO.h"

namespace Graphics {

	class VAO {
	public:
		VAO();
		~VAO() { Delete(); };

		// Disable copy constructors. Multiple VAOs should not point to the same buffer.
		VAO(const VAO&) = delete;
		VAO& operator=(const VAO&) = delete;

		VAO(VAO&& other) noexcept {
			steal(other);
		}
		VAO& operator=(VAO&& other) noexcept {
			if (this != &other) {
				destroy();
				steal(other);
			}

			return *this;
		}

		void Bind() const;
		void Unbind() const;
		void Delete();

		// Binds the VBO, links a VBO attribute, then unbinds the VBO
		void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		void LinkAttribI(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

		// Links the VBO attribute without binding/unbinding the VBO
		void LinkAttribNoBinding(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		void LinkAttribINoBinding(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

		inline GLuint GetId() const noexcept { return id; }

	private:
		GLuint id;

		void destroy() { Delete(); };
		void steal(VAO& other) noexcept;
	};

}
