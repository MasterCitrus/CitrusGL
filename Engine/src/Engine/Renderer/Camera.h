#pragma once

#include <glm/glm.hpp>

namespace CitrusGL
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return projection; }

	protected:
		glm::mat4 projection = glm::mat4(1.0f);
	};
}