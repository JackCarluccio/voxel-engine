#include "graphics/Renderer.h"

int frameCount = 0;
std::chrono::steady_clock::time_point lastFrameTime;

std::unique_ptr<Graphics::ShaderProgram> shaderProgram;

namespace Graphics::Renderer {

	Graphics::Camera camera(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 5000.0f);

	void start() {
		// Load OpenGL function pointers
		if (!gladLoadGL(glfwGetProcAddress)) {
			std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
			return;
		}

		// Make sure triangles don't render over each other
		glEnable(GL_DEPTH_TEST);

		// Only render triangles if they appear CCW
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		// Create the chunk mesh shader program
		shaderProgram = std::make_unique<Graphics::ShaderProgram>("chunk_mesh_vert.glsl", "chunk_mesh_frag.glsl");

		lastFrameTime = std::chrono::steady_clock::now();
	}

	void render() {
		// Calculate delta time
		frameCount++;
		auto currentTime = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
		lastFrameTime = currentTime;

		// Run all callbacks before rendering
		onPreRender.emit(deltaTime);

		// Clear the screen
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render chunks

		shaderProgram->Activate();
        glUniform1i(glGetUniformLocation(shaderProgram->GetId(), "textureAtlas"), 0);

		const glm::mat4 view = camera.GetViewMatrix();
		const glm::mat4 projection = camera.GetProjectionMatrix();

        // Update camera and projection matrices once per frame
        GLuint viewLocation = glGetUniformLocation(shaderProgram->GetId(), "view");
        GLuint projectionLocation = glGetUniformLocation(shaderProgram->GetId(), "projection");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint chunkCoordLocation = glGetUniformLocation(shaderProgram->GetId(), "chunkCoord");
        for (const auto& [coord, mesh] : World::Chunks::ChunkManager::meshes) {
			mesh.Draw(chunkCoordLocation);
        }

		// Run all callbacks after rendering
		onPostRender.emit(deltaTime);
	}

}
