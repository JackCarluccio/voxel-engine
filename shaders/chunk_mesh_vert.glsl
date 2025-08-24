#version 460 core

layout (location = 0) in uint aData;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const uint vertexIndexMask = 0x1FFFF;
const uint atlasIndexMask = 0xFF << 17;
const uint cornerMask = 0x3 << 25;

void main() {
	// Unpack vertexIndex, atlasIndex, and corner from aData
	int vertexIndex = int(aData & vertexIndexMask);
	int atlasIndex = int((aData & atlasIndexMask) >> 17);
	int corner = int((aData & cornerMask) >> 25);

	// Calculate local position from vertexIndex
	int z = vertexIndex % 17;
	int x = vertexIndex / 17 % 17;
	int y = vertexIndex / (17 * 17);

	gl_Position = projection * view * model * vec4(x, y, z, 1.0);

	float u = float(atlasIndex % 16) / 16.0;
	float v = float(atlasIndex / 16) / 16.0;
	if (corner == 0) {
		v += 0.0625;
	} else if (corner == 1) {
		u += 0.0625;
		v += 0.0625;
	} else if (corner == 2) {
		u += 0.0625;
	} else {

	}

	texCoord = vec2(u, v);
}