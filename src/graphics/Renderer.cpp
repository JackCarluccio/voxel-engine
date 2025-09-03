#include "graphics/Renderer.h"

int frameCount = 0;
std::chrono::steady_clock::time_point lastFrameTime;

namespace Graphics::Renderer {

	void start() {
		// Load OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
			return;
		}

		// Make sure triangles don't render over each other
		glEnable(GL_DEPTH_TEST);

		// Only render triangles if they appear CCW
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

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

		// Run all callbacks after rendering
		onPostRender.emit(deltaTime);
	}

}
