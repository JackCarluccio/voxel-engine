#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>

#include "graphics/VAO.h"
#include "graphics/VBO.h"
#include "graphics/EBO.h"
#include "graphics/stb_image.h"

#include "worldGen/Chunk.h"

namespace Graphics {
	struct VertexData {
		GLint index;
		GLfloat u, v;
	};

	class ChunkMesh {
	public:
		const glm::ivec2 chunkCoord;

		static void Initialize();

		// Builds the ChunkMesh for the chunk
		static ChunkMesh BuildChunkMesh(const WorldGen::Chunk& chunk, const WorldGen::Chunk* const* neighbors);

		ChunkMesh(const glm::ivec2& chunkCoord, const std::vector<VertexData>& vertices, const std::vector<GLuint>& indices);
		~ChunkMesh();

		// Disable copy constructors. Since VAOs, VBOs, and EBOs cannot be copied, neither can a mesh.
		ChunkMesh(const ChunkMesh&) = delete;
		ChunkMesh& operator=(const ChunkMesh&) = delete;

		ChunkMesh(ChunkMesh&& other) noexcept :
			chunkCoord(other.chunkCoord),
			vao(std::move(other.vao)),
			vbo(std::move(other.vbo)),
			ebo(std::move(other.ebo)),
			indexCount(other.indexCount)
		{}

		// Cannot move assign a ChunkMesh, as it would invalidate indexCount
		ChunkMesh& operator=(ChunkMesh&&) = delete;

		void Draw() const;

	private:
		VAO vao;
		VBO vbo;
		EBO ebo;

		const int indexCount;

		inline static GLuint textureAtlasId;
	};
}