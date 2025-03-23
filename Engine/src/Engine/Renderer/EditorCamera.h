#pragma once

#include "Camera.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"

namespace CitrusGL
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep delta);
		void OnEvent(Event& e);

		inline float GetDistance() const { return distance; }
		inline void SetDistance(float distance) { this->distance = distance; }

		inline void SetViewportSize(float width, float height)
		{
			viewportWidth = width;
			viewportHeight = height;
			UpdateProjection();
		}

		const glm::mat4& GetViewMatrix() const { return viewMatrix; }
		glm::mat4 GetViewProjection() const { return projection * viewMatrix; }

		glm::vec3 GetUpVector() const;
		glm::vec3 GetRightVector() const;
		glm::vec3 GetForwardVector() const;
		const glm::vec3& GetPosition() const { return position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		float fov = 45.0f;
		float aspectRatio = 16.0f / 9.0f;
		float nearClip = 0.1f;
		float farClip = 1000.0f;

		glm::mat4 viewMatrix;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 focalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 initialMousePos = { 0.0f, 0.0f };

		float distance = 10.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float viewportWidth = 1280;
		float viewportHeight = 720;

	};
}