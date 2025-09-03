#include "engine/Input.h"

Engine::Input::MousePosition mouseDelta = { 0.0f, 0.0f };
Engine::Input::MousePosition lastMousePosition = { 0.0f, 0.0f };

bool keyStates[359] = { false };

void resetAllKeys() {
	for (int i = 0; i < 359; i++) {
		keyStates[i] = false;
	}
}

namespace Engine::Input {

	// Check if a key is currently held down
	bool isKeyHeld(Key key) noexcept {
		return keyStates[key];
	}

	// Get the current mouse position
	MousePosition getMousePosition() {
		return lastMousePosition;
	}

	// Get the change in mouse position since the last frame
	MousePosition getMouseDelta() {
		return mouseDelta;
	}

	void start() {
		lastMousePosition = getMousePosition();

		onFocusLost.connect(resetAllKeys);
		onFocusGained.connect(resetAllKeys);

		onKeyDown.connect([](Key key) {
			keyStates[key] = true;
		});
		onKeyUp.connect([](Key key) {
			keyStates[key] = false;
		});
	}

	void update() {
		double x, y;
		glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);

		float xf = static_cast<float>(x);
		float yf = static_cast<float>(y);

		// Calculate mouse delta and store new mouse position
		mouseDelta.x = xf - lastMousePosition.x;
		mouseDelta.y = yf - lastMousePosition.y;
		lastMousePosition = { xf, yf };
	}

}
