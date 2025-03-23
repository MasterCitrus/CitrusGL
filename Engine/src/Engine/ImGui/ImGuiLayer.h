#pragma once

#include "Engine/Core/Layer.h"

namespace CitrusGL
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { blockEvents = block; }
	private:
		float time = 0.0f;
		bool blockEvents = true;
	};
}