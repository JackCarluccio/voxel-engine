#include "graphics/VBO.h"

namespace Graphics {

	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO::VBO(GLsizeiptr size, const void* data) {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	// Binds the VBO
	void VBO::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	// Unbinds the VBO
	void VBO::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Deletes the VBO
	void VBO::Delete() {
		if (id != 0) {
			glDeleteBuffers(1, &id);
			id = 0;
		}
	}

	// Steal from another VBO by taking its ID
	void VBO::steal(VBO& other) noexcept {
		id = other.id;
		other.id = 0; // Set the other's id to 0 to avoid double deletion
	}

}
