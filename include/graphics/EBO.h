#pragma once

#include <vector>
#include <glad/glad.h>

namespace Graphics {

	class EBO {
	public:
		EBO(GLsizeiptr size, const void* data);
		~EBO() { Delete(); };

		// Disable copy constructors. Multiple VAOs should not point to the same buffer.
		EBO(const EBO&) = delete;
		EBO& operator=(const EBO&) = delete;

		EBO(EBO&& other) noexcept {
			steal(other);
		}
		EBO& operator=(EBO&& other) noexcept {
			if (this != &other) {
				destroy();
				steal(other);
			}

			return *this;
		}

		void Bind() const;
		void Unbind() const;
		void Delete();

		inline GLuint GetId() const noexcept { return id; }

	private:
		GLuint id;

		void destroy() { Delete(); };
		void steal(EBO& other) noexcept;
	};

}