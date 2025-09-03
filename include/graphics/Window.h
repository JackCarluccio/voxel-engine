#pragma once

#include <iostream>

#include <GLFW/glfw3.h>

namespace Graphics {
	class Window {
	public:
		Window(const char* title, bool vsync);
		~Window();

		void pollEvents() const;
		void swapBuffers() const;
		bool shouldClose() const;

		void setKeyCallback(GLFWkeyfun cbfun);
		void setWindowFocusCallback(GLFWwindowfocusfun cbfun);
		void setFramebufferSizeCallback(GLFWframebuffersizefun cbfun);

		inline GLFWwindow* getGLFWwindow() const { return window_; }

	private:
		GLFWwindow* window_;
	};
}