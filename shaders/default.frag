#version 460 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D textureAtlas;

void main() {
	FragColor = texture(textureAtlas, texCoord) * vec4(1, 1, 1, 1);
}