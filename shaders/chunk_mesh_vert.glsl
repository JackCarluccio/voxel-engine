#version 460 core

layout (location = 0) in uint aData;

out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform ivec2 chunkCoord;

const uint vertexIndexMask = 0x1FFFF;
const uint atlasIndexMask = 0xFF << 17;
const uint cornerMask = 0x3 << 25;

const vec2 cornerAtlasOffsets[4] = vec2[](
	vec2(0.0000, 0.0625),
	vec2(0.0625, 0.0625),
	vec2(0.0625, 0.0000),
	vec2(0.0000, 0.0000)
);

void main() {
	// Unpack vertexIndex, atlasIndex, and corner from aData
	int vertexIndex = int(aData & vertexIndexMask);
	int atlasIndex = int((aData & atlasIndexMask) >> 17);
	int corner = int((aData & cornerMask) >> 25);

	// Calculate local position from vertexIndex
	int z = vertexIndex % 17;
	int x = vertexIndex / 17 % 17;
	int y = vertexIndex / (17 * 17);

	gl_Position = projection * view * vec4(x + chunkCoord.x * 16, y, z + chunkCoord.y * 16, 1.0);

	// Calculate texture coordinates from atlas index and vertex corner
	texCoord = vec2(
		float(atlasIndex % 16) / 16.0 + cornerAtlasOffsets[corner].x,
		float(atlasIndex / 16) / 16.0 + cornerAtlasOffsets[corner].y
	);
}