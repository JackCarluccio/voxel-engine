#pragma once

#include <vector>
#include <glad/glad.h>
#include "graphics/VAO.h"
#include "graphics/VBO.h"
#include "graphics/EBO.h"

namespace Graphics {
	class ChunkMesh {
	public:
		ChunkMesh(const std::vector<GLint>& vertices, const std::vector<GLuint>& indices);
		~ChunkMesh();

		// Disable copy constructors. Since VAOs, VBOs, and EBOs cannot be copied, neither can a mesh.
		ChunkMesh(const ChunkMesh&) = delete;
		ChunkMesh& operator=(const ChunkMesh&) = delete;

		ChunkMesh(ChunkMesh&& other) noexcept :
			vertexArrayObject(std::move(other.vertexArrayObject)),
			vertexBufferObject(std::move(other.vertexBufferObject)),
			elementBufferObject(std::move(other.elementBufferObject)),
			indexCount(other.indexCount)
		{}

		// Cannot move assign a ChunkMesh, as it would invalidate indexCount
		ChunkMesh& operator=(ChunkMesh&&) = delete;

		void Draw() const;

	private:
		VAO vertexArrayObject;
		VBO vertexBufferObject;
		EBO elementBufferObject;

		const int indexCount;
	};
}