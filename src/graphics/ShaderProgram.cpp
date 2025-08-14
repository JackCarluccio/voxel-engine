#include "graphics/shaderProgram.h"

std::string ReadFile(const std::string& fileName) {
	std::ifstream file("../../../shaders/" + fileName);
	if (!file) {
		std::cerr << "Error opening shader source file: " << fileName << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}


Graphics::ShaderProgram::ShaderProgram(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) {
	std::string vertexShaderSourceStr = ReadFile(vertexShaderFileName);
	std::string fragmentShaderSourceStr = ReadFile(fragmentShaderFileName);

	const char* vertexShaderSource = vertexShaderSourceStr.c_str();
	const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

	// Create, attach, and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Check for any errors with the vertex shader
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
	}

	// Create, attach, and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Check for any errors with the fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
	}

	// Create the shader program, attach the shaders, and link the program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	// Cleanup the now useless shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void Graphics::ShaderProgram::Activate() const {
	glUseProgram(id);
}


void Graphics::ShaderProgram::Delete() const {
	glDeleteProgram(id);
}