#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "graphics/ShaderProgram.h"
#include "graphics/Mesh.h"
#include "graphics/Vertex.h"
#include "worldGen/Chunk.h"
#include "worldGen/WorldGen.h"

// Vertices coordinates
const std::vector<GLfloat> vertices {
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
    0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

// Indices for vertices order
const std::vector<GLuint> indices {
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Lower right triangle
    5, 4, 1 // Upper triangle
};

float aspectRatio = 800.0f / 600.0f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

bool isWHeld = false;
bool isSHeld = false;
bool isAHeld = false;
bool isDHeld = false;
bool isQHeld = false;
bool isEHeld = false;


// Window resize callback
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Update camera position based on key inputs
void UpdateCamera(float delta) {
    glm::vec3 moveDirection(
        isDHeld - isAHeld,
        isEHeld - isQHeld,
        isSHeld - isWHeld
    );

	if (glm::length(moveDirection) > 0.0f) {
		moveDirection = glm::normalize(moveDirection);
		cameraPosition += moveDirection * delta;
        view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	}
}

// Key callback function to handle key inputs. Stores state of keys.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_W:
        if (action == GLFW_PRESS) {
            isWHeld = true;
        } else if (action == GLFW_RELEASE) {
            isWHeld = false;
        }
        break;
    case GLFW_KEY_S:
        if (action == GLFW_PRESS) {
            isSHeld = true;
        } else if (action == GLFW_RELEASE) {
            isSHeld = false;
        }
        break;
    case GLFW_KEY_A:
		if (action == GLFW_PRESS) {
			isAHeld = true;
		} else if (action == GLFW_RELEASE) {
			isAHeld = false;
		}
		break;
	case GLFW_KEY_D:
        if (action == GLFW_PRESS) {
            isDHeld = true;
		} else if (action == GLFW_RELEASE) {
            isDHeld = false;
        }
        break;
    case GLFW_KEY_Q:
        if (action == GLFW_PRESS) {
            isQHeld = true;
        } else if (action == GLFW_RELEASE) {
			isQHeld = false;
		}
		break;
    case GLFW_KEY_E:
		if (action == GLFW_PRESS) {
			isEHeld = true;
		} else if (action == GLFW_RELEASE) {
			isEHeld = false;
		}
		break;
    }
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

	glfwSetKeyCallback(window, KeyCallback);

    // Create and use shader program
	Graphics::ShaderProgram shaderProgram("default.vert", "default.frag");

    //Graphics::Mesh mesh(vertices, indices);

    std::vector<Graphics::Vertex> meshVertices;
	std::vector<GLuint> meshIndices;

	WorldGen::Chunk chunk = WorldGen::GenerateChunk(glm::ivec3(0, 0, 0));
	chunk.BuildMesh(meshVertices, meshIndices);

    Graphics::Mesh mesh(meshVertices, meshIndices);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        UpdateCamera(0.01f);

        shaderProgram.Activate();

        GLuint modelLocation = glGetUniformLocation(shaderProgram.GetId(), "model");
        GLuint viewLocation = glGetUniformLocation(shaderProgram.GetId(), "view");
        GLuint projectionLocation = glGetUniformLocation(shaderProgram.GetId(), "projection");

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		mesh.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
