#pragma once

#include "Engine/Core.h"
#include "Engine/Events/ApplicationEvent.h"

struct GLFWwindow;


namespace CitrusGL
{
	class OpenGLContext;

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string title = "CitrusGL", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		virtual ~Window();

		virtual void OnUpdate();

		virtual unsigned int GetWidth() const { return data.Width; }
		virtual unsigned int GetHeight() const { return data.Height; }

		virtual void SetEventCallback(const EventCallbackFn& callback) { data.eventCallback = callback; }
		virtual void SetVSync(bool enabled);
		virtual bool IsVSync() const;

		virtual void* GetNativeWindow() const { return window; }

		static Window* Create(const WindowProps& props = WindowProps());

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* window;
		OpenGLContext* context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn eventCallback;
		};

		WindowData data;
	};
}