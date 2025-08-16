#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <chrono>
#include <iostream>

#include "graphics/ShaderProgram.h"
#include "graphics/ChunkMesh.h"
#include "worldGen/Chunk.h"
#include "worldGen/WorldGen.h"

struct ChunkInfo {
    WorldGen::Chunk chunk;
    Graphics::ChunkMesh mesh;
};

float aspectRatio = 800.0f / 600.0f;

glm::vec3 cameraPosition = glm::vec3(8.0f, 16.0f, -40.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 5000.0f);

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
void UpdateCamera(float deltaTime) {
    glm::vec3 moveDirection(
        isAHeld - isDHeld,
        isEHeld - isQHeld,
        isWHeld - isSHeld
    );

	if (glm::length(moveDirection) > 0.0f) {
		moveDirection = glm::normalize(moveDirection);
		cameraPosition += moveDirection * deltaTime * 8.0f;
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

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Voxel Engine", nullptr, nullptr);
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

    // Only render triangles if they appear CCW
    glEnable(GL_CULL_FACE);
	glCullFace(GL_CCW);

    // Make sure triangles don't render over each other
    glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, KeyCallback);

    // Create and use shader program
	Graphics::ShaderProgram shaderProgram("default.vert", "default.frag");

    std::vector<ChunkInfo> chunks;
    for (int x = -10; x <= 10; x++)
    for (int z = -1; z <= 10; z++) {
        glm::ivec2 chunkCoordinate(x, z);
		WorldGen::Chunk chunk = WorldGen::GenerateChunk(chunkCoordinate);
		Graphics::ChunkMesh mesh = chunk.BuildMesh();
		chunks.emplace_back(std::move(chunk), std::move(mesh));
    }

    int frameCount = 0;
	std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        frameCount++;
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
		lastFrameTime = currentTime;

		if (frameCount % 60 == 0) {
			std::cout << "FPS: " << (1.0f / deltaTime) << std::endl;
		}

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UpdateCamera(deltaTime);

        shaderProgram.Activate();

        // Update camera and projection matrices once per frame
        GLuint viewLocation = glGetUniformLocation(shaderProgram.GetId(), "view");
        GLuint projectionLocation = glGetUniformLocation(shaderProgram.GetId(), "projection");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint modelLocation = glGetUniformLocation(shaderProgram.GetId(), "model");

        for (const ChunkInfo& chunk : chunks) {
			glm::ivec2 chunkPosition = chunk.chunk.position;
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunkPosition.x * WorldGen::width, 0, chunkPosition.y * WorldGen::width));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			chunk.mesh.Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
