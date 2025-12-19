#pragma once

#include <chrono>
#include <memory>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "engine/Signal.h"
#include "graphics/Camera.h"
#include "graphics/ShaderProgram.h"
#include "world/chunks/ChunkManager.h"

namespace Graphics::Renderer {

	extern Graphics::Camera camera;

	inline Engine::Signal<float> onPreRender;
	inline Engine::Signal<float> onPostRender;
	
	void start();
	void render();

}
