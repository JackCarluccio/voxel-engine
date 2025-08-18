#include "graphics/ChunkMesh.h"


// ChunkMesh's vertex measurement properties
constexpr int vertexWidth = WorldGen::width + 1;
constexpr int vertexArea = vertexWidth * vertexWidth;
constexpr int GetVertexIndex(int x, int y, int z) noexcept {
	return z + x * vertexWidth + y * vertexArea;
}

struct FaceVertexInfo {
	const GLuint vertexIndexOffset;
	const GLfloat u, v;
};
constexpr FaceVertexInfo faceVertexInfo[6][4] = {
	{ // Back face
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.00f, 0.00f },
	},
	{ // Front face
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.00f, 0.00f },
	},
	{ // Right face ( cuz someone decided to flip x axis )
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.00f, 0.00f },
	},
	{ // Left face ( cuz someone decided to flip x axis )
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.00f, 0.00f },
	},
	{ // Bottom face
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.00f, 0.00f },
	},
	{ // Top face
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.00f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.25f, 0.25f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.25f, 0.00f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.00f, 0.00f },
	}
};

void BuildMeshInterior(std::vector<Graphics::VertexData>& vertices, std::vector<GLuint>& indices, const WorldGen::Chunk& chunk) {
	for (int y = 1; y < WorldGen::height - 1; y++)
	for (int x = 1; x < WorldGen::width - 1; x++)
	for (int z = 1; z < WorldGen::width - 1; z++) {
		int index = WorldGen::Chunk::IndexFromXYZ(x, y, z);
		if (chunk.GetBlock(index) == 0)
			continue;

		// Construct each face
		for (int i = 0; i < 6; i++) {
			// Only construct this face if not occluded
			int neighborIndex = index + WorldGen::neighborIndexOffsets[i];
			if (chunk.GetBlock(neighborIndex) != 0)
				continue;

			// Add each vertex
			int originVertexIndex = GetVertexIndex(x, y, z);
			for (int j = 0; j < 4; j++) {
				FaceVertexInfo info = faceVertexInfo[i][j];
				vertices.emplace_back(
					originVertexIndex + info.vertexIndexOffset,
					info.u, info.v
				);
			}

			// Construct the two triangles
			int indexOffset = vertices.size() - 4;
			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 1);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 3);
		}
	}
}

void BuildMeshCap(std::vector<Graphics::VertexData>& vertices, std::vector<GLuint>& indices, const WorldGen::Chunk& chunk, bool isTop) {
	int i, y;
	if (isTop) {
		i = 5; // Top face
		y = WorldGen::height - 1;
	} else {
		i = 4; // Bottom face
		y = 0;
	}

	for (int x = 0; x < WorldGen::width; x++)
	for (int z = 0; z < WorldGen::width; z++) {
		if (chunk.GetBlock(x, y, z) == 0)
			continue;
		// Construct the top face
		int originVertexIndex = GetVertexIndex(x, y, z);
		for (int j = 0; j < 4; j++) {
			FaceVertexInfo info = faceVertexInfo[i][j];
			vertices.emplace_back(
				originVertexIndex + info.vertexIndexOffset,
				info.u, info.v
			);
		}
		// Construct the two triangles
		int indexOffset = vertices.size() - 4;
		indices.push_back(indexOffset + 0);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 0);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 3);
	}
}

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

	ChunkMesh ChunkMesh::BuildChunkMesh(const WorldGen::Chunk& chunk) {
		std::vector<VertexData> vertices;
		std::vector<GLuint> indices;

		BuildMeshInterior(vertices, indices, chunk);
		BuildMeshCap(vertices, indices, chunk, true); // Top of the chunk
		BuildMeshCap(vertices, indices, chunk, false); // Bottom of the chunk

		return ChunkMesh(chunk.position, vertices, indices);
	}

	ChunkMesh::ChunkMesh(const glm::ivec2& chunkCoord, const std::vector<VertexData>& vertices, const std::vector<GLuint>& indices):
		chunkCoord(chunkCoord),
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
