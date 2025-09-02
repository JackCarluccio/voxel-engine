#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Graphics {

	class Camera {
	public:
		glm::mat4 transform;

		Camera(float fov, float aspectRatio, float nearClip, float farClip);

		glm::mat4 GetViewMatrix() const noexcept;
		glm::mat4 GetProjectionMatrix() const noexcept;

		void SetFov(float fov) noexcept;
		void SetAspectRatio(float aspectRatio) noexcept;
		void SetClipPlanes(float nearClip, float farClip) noexcept;

	private:
		float fov;
		float aspectRatio;
		float nearClip;
		float farClip;

		glm::mat4 cachedProjection;

		void UpdateProjectionMatrix() noexcept;
	};

}
