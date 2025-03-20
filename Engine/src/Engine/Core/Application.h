#pragma once

#include "../Core.h"

#include "Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Engine/ImGui/ImGuiLayer.h"

namespace CitrusGL
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *window; }

		inline static Application& Get() { return *instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		ImGuiLayer* imGuiLayer;
		bool running = true;
		LayerStack layerStack;

	private:
		static Application* instance;
	};

	Application* CreateApplication();
}