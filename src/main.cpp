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

#include "engine/Input.h"
#include "graphics/Camera.h"
#include "graphics/ShaderProgram.h"
#include "graphics/ChunkMesh.h"
#include "world/chunks/Chunk.h"
#include "world/chunks/ChunkManager.h"
#include "world/generation/Generator.h"

struct ChunkInfo {
    World::Chunks::Chunk chunk;
    Graphics::ChunkMesh mesh;
};

Graphics::Camera camera(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 5000.0f);

constexpr int renderDistance = 10;

// Window resize callback
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
	camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void WindowFocusCallback(GLFWwindow* window, int focused) {
    if (focused) {
        Engine::Input::onFocusGained.emit();
    } else {
        Engine::Input::onFocusLost.emit();
    }
}

// Update camera position and rotation from key and mouse inputs
void UpdateCamera(float deltaTime) {
    // Handle keyboard movement
    const glm::vec3 right = camera.transform[0];
    const glm::vec3 forward = camera.transform[2];
    const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 moveLocal(0.0f);
    if (Engine::Input::isKeyHeld(Engine::Input::KEY_A)) moveLocal.x -= 1.0f;
    if (Engine::Input::isKeyHeld(Engine::Input::KEY_D)) moveLocal.x += 1.0f;
    if (Engine::Input::isKeyHeld(Engine::Input::KEY_Q)) moveLocal.y -= 1.0f;
    if (Engine::Input::isKeyHeld(Engine::Input::KEY_E)) moveLocal.y += 1.0f;
    if (Engine::Input::isKeyHeld(Engine::Input::KEY_W)) moveLocal.z += 1.0f;
    if (Engine::Input::isKeyHeld(Engine::Input::KEY_S)) moveLocal.z -= 1.0f;

    if (glm::length(moveLocal) > 0.0f) {
        moveLocal = glm::normalize(moveLocal) * (deltaTime * 32.0f);
        camera.transform = glm::translate(camera.transform, moveLocal);
    }

	float yaw = atan2(forward.x, forward.z);
	float pitch = asin(-forward.y);

    // Update camera orientation
	const auto mouseDelta = Engine::Input::getMouseDelta();
    yaw += -mouseDelta.x * 0.0025f;
    pitch += mouseDelta.y * 0.0025f;

    // Prevent looking too far up or down
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate new direction vectors from yaw and pitch
    glm::vec3 newForward;
    newForward.x = sin(yaw) * cos(pitch);
    newForward.y = -sin(pitch);
    newForward.z = cos(yaw) * cos(pitch);
    newForward = glm::normalize(newForward);

    // Calculate right and up vectors
    glm::vec3 newRight = glm::normalize(glm::cross(newForward, worldUp));
    glm::vec3 newUp = glm::normalize(glm::cross(newRight, newForward));

    // Update the camera transform matrix (preserve position)
    glm::vec3 position = camera.transform[3];
    camera.transform[0] = glm::vec4(newRight, 0.0f);
    camera.transform[1] = glm::vec4(newUp, 0.0f);
    camera.transform[2] = glm::vec4(newForward, 0.0f);
    camera.transform[3] = glm::vec4(position, 1.0f);
}

// Key callback function to handle key inputs. Stores state of keys.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        Engine::Input::onKeyDown.emit(static_cast<Engine::Input::Key>(key));
    } else if (action == GLFW_RELEASE) {
        Engine::Input::onKeyUp.emit(static_cast<Engine::Input::Key>(key));
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
	//glfwSwapInterval(0);

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

    for (int x = -renderDistance; x <= renderDistance; x++)
    for (int z = -renderDistance; z <= renderDistance; z++) {
        World::Chunks::ChunkManager::CreateChunk(glm::ivec2(x, z));
    }

	Engine::Input::start();

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

        Engine::Input::update();

        UpdateCamera(deltaTime);

        shaderProgram.Activate();
        glUniform1i(glGetUniformLocation(shaderProgram.GetId(), "textureAtlas"), 0);

		const glm::mat4 view = camera.GetViewMatrix();
		const glm::mat4 projection = camera.GetProjectionMatrix();

        // Update camera and projection matrices once per frame
        GLuint viewLocation = glGetUniformLocation(shaderProgram.GetId(), "view");
        GLuint projectionLocation = glGetUniformLocation(shaderProgram.GetId(), "projection");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint chunkCoordLocation = glGetUniformLocation(shaderProgram.GetId(), "chunkCoord");
        for (const auto& [coord, mesh] : World::Chunks::ChunkManager::meshes) {
			mesh.Draw(chunkCoordLocation);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
