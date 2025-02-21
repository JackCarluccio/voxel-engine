#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string ReadFile(const char* filename);

class ShaderProgram {
public:
	ShaderProgram(const std::string& vertexSourceFileName, const std::string& fragmentSourceFileName);

	void Activate();
	void Delete();

	GLuint GetId() const { return id; }

private:
	GLuint id;
};