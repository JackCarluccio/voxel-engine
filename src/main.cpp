#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <chrono>
#include <iostream>
#include <unordered_map>

#include "graphics/ShaderProgram.h"
#include "graphics/ChunkMesh.h"
#include "worldGen/Chunk.h"
#include "worldGen/WorldGen.h"
#include "worldGen/ChunkManager.h"

struct ChunkInfo {
    WorldGen::Chunk chunk;
    Graphics::ChunkMesh mesh;
};

float aspectRatio = 1920.0f / 1080.0f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 128.0f, 0.0f);
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

	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 5000.0f);
}

bool firstMouse = true;
void WindowFocusCallback(GLFWwindow* window, int focused) {
    if (focused) {
        // Window gained focus
        firstMouse = true;  // Reset mouse tracking when returning to window
    } else {
        // Window lost focus
        // Release all held keys to prevent getting stuck
        isWHeld = isSHeld = isAHeld = isDHeld = isQHeld = isEHeld = false;
    }
}

// Update camera position and rotation from key and mouse inputs
double lastMouseX = 0.0f, lastMouseY = 0.0f;
float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
void UpdateCamera(float deltaTime) {
    // Handle keyboard movement
    glm::vec3 right = glm::normalize(glm::cross(cameraDirection, cameraUp));
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 moveDirection(0.0f);
    if (isAHeld) moveDirection -= right;
    if (isDHeld) moveDirection += right;
    if (isQHeld) moveDirection -= worldUp;
    if (isEHeld) moveDirection += worldUp;
    if (isWHeld) moveDirection += cameraDirection;
    if (isSHeld) moveDirection -= cameraDirection;

    if (glm::length(moveDirection) > 0.0f) {
        moveDirection = glm::normalize(moveDirection);
        cameraPosition += moveDirection * deltaTime * 8.0f;
    }

    // Handle mouse rotation
    double mouseX, mouseY;
    glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);

    if (firstMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
    }

    // Calculate mouse movement sensitivity
    float sensitivity = 0.1f;
    float xOffset = static_cast<float>(mouseX - lastMouseX) * sensitivity;
    float yOffset = static_cast<float>(lastMouseY - mouseY) * sensitivity; // Reversed for y-coordinates

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Update camera orientation
    cameraYaw += xOffset;
    cameraPitch += yOffset;

    // Prevent looking too far up or down
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    // Calculate new direction vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    direction.y = sin(glm::radians(cameraPitch));
    direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraDirection = glm::normalize(direction);

    // Rebuild view matrix
    view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, worldUp);
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

    // Set relevant window hints before creating the window.
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    // Create a borderless window, not fullscreen. Fullscreen causes many issues
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Voxel Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(window, 0, 0);
    glfwMakeContextCurrent(window);

	// Reset cursor position and key states when the window changes focus
    glfwSetWindowFocusCallback(window, WindowFocusCallback);
	// Update OpenGL's viewport and adjust the projection matrix when the window is resized
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Hide cursor and lock it to the window. Cursor inputs still work
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Enables/Disables VSync. Without VSync, screen tearing can occur, and resource usage is higher
	glfwSwapInterval(1);

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

    Graphics::ChunkMesh::Initialize();

	glfwSetKeyCallback(window, KeyCallback);

    // Create and use shader program
	Graphics::ShaderProgram shaderProgram("chunk_mesh_vert.glsl", "chunk_mesh_frag.glsl");

    for (int x = -10; x <= 10; x++)
    for (int z = -10; z <= 10; z++) {
        WorldGen::ChunkManager::CreateChunk(glm::ivec2(x, z));
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
        firstMouse = false;

        shaderProgram.Activate();
        glUniform1i(glGetUniformLocation(shaderProgram.GetId(), "textureAtlas"), 0);

        // Update camera and projection matrices once per frame
        GLuint viewLocation = glGetUniformLocation(shaderProgram.GetId(), "view");
        GLuint projectionLocation = glGetUniformLocation(shaderProgram.GetId(), "projection");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint modelLocation = glGetUniformLocation(shaderProgram.GetId(), "model");

        for (const auto& [coord, mesh]: WorldGen::ChunkManager::meshes) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(mesh.chunkCoord.x * WorldGen::width, 0, mesh.chunkCoord.y * WorldGen::width));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			mesh.Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
