#include "graphics/ChunkMesh.h"

// ChunkMesh's vertex measurement properties
constexpr int vertexWidth = World::Chunks::width + 1;
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
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.0000f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.0625f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.0625f, 0.0000f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.0000f, 0.0000f },
	},
	{ // Front face
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.0000f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.0625f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.0625f, 0.0000f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.0000f, 0.0000f },
	},
	{ // Right face ( cuz someone decided to flip x axis )
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.0000f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.0625f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.0625f, 0.0000f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.0000f, 0.0000f },
	},
	{ // Left face ( cuz someone decided to flip x axis )
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.0000f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.0625f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.0625f, 0.0000f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.0000f, 0.0000f },
	},
	{ // Bottom face
		FaceVertexInfo{ GetVertexIndex(1, 0, 1), 0.0000f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 1), 0.0625f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 0, 0), 0.0625f, 0.0000f },
		FaceVertexInfo{ GetVertexIndex(1, 0, 0), 0.0000f, 0.0000f },
	},
	{ // Top face
		FaceVertexInfo{ GetVertexIndex(1, 1, 0), 0.0000f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 0), 0.0625f, 0.0625f },
		FaceVertexInfo{ GetVertexIndex(0, 1, 1), 0.0625f, 0.0000f },
		FaceVertexInfo{ GetVertexIndex(1, 1, 1), 0.0000f, 0.0000f },
	}
};

// Blueprint for how to construct the inner faces on exterior blocks
struct ExteriorBlockFaceBlueprint {
	int face;
	int thatIndex;
};
struct ExteriorBlockBlueprint {
	int thisIndex;
	int vertexIndexOrigin;
	int numFaces;
	ExteriorBlockFaceBlueprint faceBlueprints[5];
};

// Blueprint for how to construct the exterior-most faces
struct ExteriorBlueprint {
	int thisIndex, thatIndex;
	int vertexIndexOrigin;
};

ExteriorBlockBlueprint exteriorBlockBlueprints[World::Chunks::exteriorVolume];
ExteriorBlueprint exteriorBlueprints[4][World::Chunks::width * World::Chunks::height];

// Constructs the triangles for a face of a block
void AddFace(int blockId, std::vector<Graphics::VertexData>& vertices, std::vector<GLuint>& indices, int face, int vertexIndexOrigin) {
	// Add each vertex
	for (int j = 0; j < 4; j++) {
		FaceVertexInfo info = faceVertexInfo[face][j];
		vertices.emplace_back(
			vertexIndexOrigin + info.vertexIndexOffset,
			info.u + World::Blocks::blockData[blockId].u,
			info.v + World::Blocks::blockData[blockId].v
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

// Builds all faces for blocks in the interior of the chunk
void BuildMeshInteriorBlocks(std::vector<Graphics::VertexData>& vertices, std::vector<GLuint>& indices, const World::Chunks::Chunk& chunk) {
	for (int y = 1; y < World::Chunks::height - 1; y++)
	for (int x = 1; x < World::Chunks::width - 1; x++)
	for (int z = 1; z < World::Chunks::width - 1; z++) {
		// If this block is air, there's nothing to construct
		int index = World::Chunks::IndexFromXYZ(x, y, z);
		int blockId = chunk.GetBlock(index);
		if (blockId == World::Blocks::AIR)
			continue;

		// Construct each face
		int vertexIndexOrigin = GetVertexIndex(x, y, z);
		for (int face = 0; face < 6; face++) {
			// Only construct this face if not occluded
			int neighborIndex = index + World::Chunks::indexOffsets[face];
			if (chunk.GetBlock(neighborIndex) != World::Blocks::AIR)
				continue;

			AddFace(blockId, vertices, indices, face, vertexIndexOrigin);
		}
	}
}

// Builds all non-exterior faces for all blocks on the exterior of the chunk
void BuildMeshExteriorBlocks(std::vector<Graphics::VertexData>& vertices, std::vector<GLuint>& indices, const World::Chunks::Chunk& chunk) {
	for (const ExteriorBlockBlueprint& blockBlueprint : exteriorBlockBlueprints) {
		// If this block is air, there's nothing to construct
		int blockId = chunk.GetBlock(blockBlueprint.thisIndex);
		if (blockId == World::Blocks::AIR)
			continue;

		for (int i = 0; i < blockBlueprint.numFaces; i++) {
			const ExteriorBlockFaceBlueprint& faceBlueprint = blockBlueprint.faceBlueprints[i];
			// Only construct this face if not occluded
			if (chunk.GetBlock(faceBlueprint.thatIndex) != World::Blocks::AIR)
				continue;

			AddFace(blockId, vertices, indices, faceBlueprint.face, blockBlueprint.vertexIndexOrigin);
		}
	}
}

// Builds the top or bottom faces of the chunk
void BuildMeshCap(std::vector<Graphics::VertexData>& vertices, std::vector<GLuint>& indices, const World::Chunks::Chunk& chunk, bool isTop) {
	int face, y;
	if (isTop) {
		face = 5; // Top face
		y = World::Chunks::height - 1;
	}
	else {
		face = 4; // Bottom face
		y = 0;
	}

	for (int x = 0; x < World::Chunks::width; x++)
	for (int z = 0; z < World::Chunks::width; z++) {
		// If this block is air, there's nothing to construct
		int blockId = chunk.GetBlock(x, y, z);
		if (blockId == World::Blocks::AIR)
			continue;

		AddFace(blockId, vertices, indices, face, GetVertexIndex(x, y, z));
	}
}

// Builds the exterior faces between this chunk and an adjacent chunk
void BuildMeshExterior(
	std::vector<Graphics::VertexData>& vertices, std::vector<GLuint>& indices,
	const World::Chunks::Chunk& chunk, const World::Chunks::Chunk& exteriorChunk,
	int face
) {
	for (const ExteriorBlueprint& blueprint : exteriorBlueprints[face]) {
		// If this block is air, there's nothing to construct
		int blockId = chunk.GetBlock(blueprint.thisIndex);
		if (blockId == World::Blocks::AIR)
			continue;

		// Only construct this face if not occluded
		if (exteriorChunk.GetBlock(blueprint.thatIndex) != World::Blocks::AIR)
			continue;
		
		AddFace(blockId, vertices, indices, face, blueprint.vertexIndexOrigin);
	}
}


// Populates the exterior block blueprints
void PopulateExteriorBlockBlueprints() {
	int blueprintIndex = 0;
	for (int y = 0; y < World::Chunks::height; y++)
	for (int x = 0; x < World::Chunks::width; x++)
	for (int z = 0; z < World::Chunks::width; z++) {
		if (World::Chunks::IsInterior(x, y, z))
			continue;

		ExteriorBlockBlueprint blockBlueprint;
		blockBlueprint.thisIndex = World::Chunks::IndexFromXYZ(x, y, z);
		blockBlueprint.vertexIndexOrigin = GetVertexIndex(x, y, z);

		int faceIndex = 0;
		for (int face = 0; face < 6; face++) {
			if (!World::Chunks::IsBlockFaceInBounds(x, y, z, face))
				continue;

			int thatIndex = blockBlueprint.thisIndex + World::Chunks::indexOffsets[face];
			blockBlueprint.faceBlueprints[faceIndex++] = {
				face, thatIndex
			};
		}

		blockBlueprint.numFaces = faceIndex;
		exteriorBlockBlueprints[blueprintIndex++] = blockBlueprint;
	}
}

// Populates the exterior blueprints
void PopulateExteriorBlueprints() {
	// Populate front and back exterior blueprints
	int blueprintIndex = 0;
	for (int y = 0; y < World::Chunks::height; y++)
	for (int x = 0; x < World::Chunks::width; x++) {
		int thisIndexNegative = World::Chunks::IndexFromXYZ(x, y, 0);
		int thisIndexPositive = World::Chunks::IndexFromXYZ(x, y, World::Chunks::width - 1);

		exteriorBlueprints[0][blueprintIndex] = {
			thisIndexNegative,
			thisIndexPositive,
			GetVertexIndex(x, y, 0),
		};
		exteriorBlueprints[1][blueprintIndex] = {
			thisIndexPositive,
			thisIndexNegative,
			GetVertexIndex(x, y, World::Chunks::width - 1),
		};

		blueprintIndex++;
	}

	// Populate right and left exterior blueprints
	blueprintIndex = 0;
	for (int y = 0; y < World::Chunks::height; y++)
	for (int z = 0; z < World::Chunks::width; z++) {
		int thisIndexNegative = World::Chunks::IndexFromXYZ(0, y, z);
		int thisIndexPositive = World::Chunks::IndexFromXYZ(World::Chunks::width - 1, y, z);

		exteriorBlueprints[2][blueprintIndex] = {
			thisIndexNegative,
			thisIndexPositive,
			GetVertexIndex(0, y, z),
		};
		exteriorBlueprints[3][blueprintIndex] = {
			thisIndexPositive,
			thisIndexNegative,
			GetVertexIndex(World::Chunks::width - 1, y, z),
		};

		blueprintIndex++;
	}
}

void LoadTextureAtlas(const char* textureAtlas, GLuint* textureAtlasIdPtr) {
	int width, height, numChannels;
	unsigned char* data = stbi_load(textureAtlas, &width, &height, &numChannels, 0);
	if (!data) {
		std::cerr << "Failed to load texture atlas: " << textureAtlas << std::endl;
		return;
	}

	glGenTextures(1, textureAtlasIdPtr);
	glBindTexture(GL_TEXTURE_2D, *textureAtlasIdPtr);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

namespace Graphics {

	void ChunkMesh::Initialize() {
		LoadTextureAtlas("../../../assets/texture_atlas.png", &textureAtlasId);
		PopulateExteriorBlueprints();
		PopulateExteriorBlockBlueprints();
	}

	ChunkMesh ChunkMesh::BuildChunkMesh(const World::Chunks::Chunk& chunk, const World::Chunks::Chunk* const* neighbors) {
		std::vector<VertexData> vertices;
		std::vector<GLuint> indices;

		BuildMeshInteriorBlocks(vertices, indices, chunk);
		BuildMeshExteriorBlocks(vertices, indices, chunk);
		BuildMeshCap(vertices, indices, chunk, true); // Top of the chunk
		BuildMeshCap(vertices, indices, chunk, false); // Bottom of the chunk
		BuildMeshExterior(vertices, indices, chunk, *neighbors[0], 0); // Back face
		BuildMeshExterior(vertices, indices, chunk, *neighbors[1], 1); // Front face
		BuildMeshExterior(vertices, indices, chunk, *neighbors[2], 2); // Right face
		BuildMeshExterior(vertices, indices, chunk, *neighbors[3], 3); // Left face

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

