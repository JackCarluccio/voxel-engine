#include "graphics/Camera.h"

constexpr glm::vec4 defaultRight = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
constexpr glm::vec4 defaultUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
constexpr glm::vec4 defaultForward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
constexpr glm::vec4 defaultPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

namespace Graphics {

	Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip) :
		fov(fov),
		aspectRatio(aspectRatio),
		nearClip(nearClip),
		farClip(farClip),
		transform(defaultRight, defaultUp, defaultForward, defaultPosition),
		cachedProjection(1.0f)
	{
		UpdateProjectionMatrix();
	};

	glm::mat4 Camera::GetViewMatrix() const noexcept {
		const glm::vec3 position = glm::vec3(transform[3]);
		const glm::vec3 forward = glm::vec3(transform[2]);
		return glm::lookAt(
			position,
			position + forward,
			glm::vec3(0, 1, 0)
		);
	}

	glm::mat4 Camera::GetProjectionMatrix() const noexcept {
		return cachedProjection;
	}

	void Camera::SetFov(float fov_) noexcept {
		fov = fov_;
		UpdateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float aspectRatio_) noexcept {
		aspectRatio = aspectRatio_;
		UpdateProjectionMatrix();
	}

	void Camera::SetClipPlanes(float nearClip_, float farClip_) noexcept {
		nearClip = nearClip_;
		farClip = farClip_;
		UpdateProjectionMatrix();
	}

	void Camera::UpdateProjectionMatrix() noexcept {
		cachedProjection = glm::perspective(fov, aspectRatio, nearClip, farClip);
	}

}
