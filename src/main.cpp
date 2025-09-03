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
#include "graphics/Window.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "graphics/ShaderProgram.h"
#include "graphics/ChunkMesh.h"
#include "world/chunks/Chunk.h"
#include "world/chunks/ChunkManager.h"
#include "world/generation/Generator.h"

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

// Key callback function to handle key inputs
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        Engine::Input::onKeyDown.emit(static_cast<Engine::Input::Key>(key));
    } else if (action == GLFW_RELEASE) {
        Engine::Input::onKeyUp.emit(static_cast<Engine::Input::Key>(key));
    }
}

int main() {

    Graphics::Window window("Voxel Engine", false);
    Graphics::Renderer::start();

	window.setKeyCallback(KeyCallback);
	window.setWindowFocusCallback(WindowFocusCallback);
	window.setFramebufferSizeCallback(FramebufferSizeCallback);

	Engine::Input::start();
    Graphics::ChunkMesh::Initialize();

    // Create and use shader program
	Graphics::ShaderProgram shaderProgram("chunk_mesh_vert.glsl", "chunk_mesh_frag.glsl");

    for (int x = -renderDistance; x <= renderDistance; x++)
    for (int z = -renderDistance; z <= renderDistance; z++) {
        World::Chunks::ChunkManager::CreateChunk(glm::ivec2(x, z));
    }

    Graphics::Renderer::onPreRender.connect(UpdateCamera);

    // Main loop
    while (!window.shouldClose()) {
        Engine::Input::update();
		Graphics::Renderer::render();

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

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
