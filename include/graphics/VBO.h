#pragma once

#include <vector>
#include <glad/glad.h>

namespace Graphics {

	class VBO {
	public:
		VBO(const std::vector<GLint>& vertices);
		~VBO() { Delete(); };

		// Disable copy constructors. Multiple VAOs should not point to the same buffer.
		VBO(const VBO&) = delete;
		VBO& operator=(const VBO&) = delete;

		VBO(VBO&& other) noexcept {
			steal(other);
		}
		VBO& operator=(VBO&& other) noexcept {
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
		void steal(VBO& other) noexcept;
	};

}