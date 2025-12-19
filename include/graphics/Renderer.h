#pragma once

#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "engine/Signal.h"

namespace Graphics::Renderer {

	inline Engine::Signal<float> onPreRender;
	inline Engine::Signal<float> onPostRender;
	
	void start();
	void render();

}
