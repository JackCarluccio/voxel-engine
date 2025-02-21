#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shaderProgram.h"
#include "VAO.h"
#include "VBO.h"

static const GLfloat vertices[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5, 0.0f,
    -0.5f, -0.5, 0.0f,
};


// Window resize callback
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

	// Force OpenGL 4.6 Core Profile to avoid compatibility issues
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Voxel Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        return -1;
    }

	VAO vertexArrayObject;
	vertexArrayObject.Bind();

	VBO vertexBufferObject(vertices, sizeof(vertices));

	vertexArrayObject.LinkAttrib(vertexBufferObject, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), nullptr);

	vertexArrayObject.Unbind();
	vertexBufferObject.Unbind();

    // Create and use shader program
	ShaderProgram shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		vertexArrayObject.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
