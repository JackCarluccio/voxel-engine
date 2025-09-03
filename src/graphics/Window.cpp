#include "graphics/Window.h"

namespace Graphics {

	Window::Window(const char* title, bool vsync) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
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
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Borderless window

        GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, title, nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        glfwSetWindowPos(window, 0, 0);
        glfwMakeContextCurrent(window);

        // Hide cursor and lock it to the window. Cursor inputs still work
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Enable/disable vsync
        if (!vsync)
            glfwSwapInterval(0);

        window_ = window;
	}

	// Destructor to clean up and terminate GLFW
	Window::~Window() {
        if (window_)
		    glfwDestroyWindow(window_);

		glfwTerminate();
	}

	// Poll for events
	void Window::pollEvents() const {
		glfwPollEvents();
	}

	// Swap the front and back buffers
	void Window::swapBuffers() const {
		glfwSwapBuffers(window_);
	}

	// Check if the window should close
	bool Window::shouldClose() const {
		return glfwWindowShouldClose(window_);
	}

	// Set the key callback
	void Window::setKeyCallback(GLFWkeyfun cbfun) {
		glfwSetKeyCallback(window_, cbfun);
	}

	// Set the window focus callback
	void Window::setWindowFocusCallback(GLFWwindowfocusfun cbfun) {
		glfwSetWindowFocusCallback(window_, cbfun);
	}

	// Set the framebuffer size callback
	void Window::setFramebufferSizeCallback(GLFWframebuffersizefun cbfun) {
		glfwSetFramebufferSizeCallback(window_, cbfun);
	}

}
