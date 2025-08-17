#include "graphics/ChunkMesh.h"

namespace Graphics {
	void ChunkMesh::LoadTextureAtlas(const char* textureAtlas) {
		int width, height, numChannels;
		unsigned char* data = stbi_load(textureAtlas, &width, &height, &numChannels, 0);
		if (!data) {
			std::cerr << "Failed to load texture atlas: " << textureAtlas << std::endl;
			return;
		}

		glGenTextures(1, &textureAtlasId);
		glBindTexture(GL_TEXTURE_2D, textureAtlasId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	ChunkMesh::ChunkMesh(const std::vector<VertexData>& vertices, const std::vector<GLuint>& indices):
		vao(),
		vbo(vertices.size() * sizeof(VertexData), vertices.data()),
		ebo(indices.size() * sizeof(GLuint), indices.data()),
		indexCount(indices.size())
	{
		vao.Bind();
		vbo.Bind();
		ebo.Bind();
		vao.LinkAttribI(vbo, 0, 1, GL_INT, sizeof(GLint) + 2 * sizeof(GLfloat), (void*)0);
		vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(GLint) + 2 * sizeof(GLfloat), (void*)(1 * sizeof(GLint)));
		vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();
	}


	ChunkMesh::~ChunkMesh() {
		vbo.Delete();
		ebo.Delete();
		vao.Delete();
	}


	void ChunkMesh::Draw() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAtlasId);

		vao.Bind();
		vbo.Bind();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

}
