#version 460 core

layout (location = 0) in int aVertexIndex;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	int z = aVertexIndex % 17;
	int x = aVertexIndex / 17 % 17;
	int y = aVertexIndex / (17 * 17);

	gl_Position = projection * view * model * vec4(x, y, z, 1.0);
	color = vec3(x % 2, y % 2, z % 2);
}