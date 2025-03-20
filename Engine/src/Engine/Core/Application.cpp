#include "cglpch.h"
#include "Application.h"

#include "Engine/Core/Log.h"

#include <glad/glad.h>

namespace CitrusGL
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::instance = nullptr;

	Application::Application()
	{
		CGL_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (running)
		{
			glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : layerStack) layer->OnUpdate();

			imGuiLayer->Begin();
			for (Layer* layer : layerStack) layer->OnImGuiRender();
			imGuiLayer->End();

			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = layerStack.end(); it != layerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.handled) break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		layerStack.PushLayer(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}
}