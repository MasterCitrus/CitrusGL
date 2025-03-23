#pragma once

#include <CitrusGL.h>

#include "Engine/Renderer/EditorCamera.h"

namespace CitrusGL
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep delta) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		Ref<Framebuffer> framebuffer;
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;

		EditorCamera camera;

		bool viewportFocused = false, viewportHovered = false;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
	};
}