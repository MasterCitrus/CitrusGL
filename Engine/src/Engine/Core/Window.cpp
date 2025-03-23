#include "cglpch.h"
#include "Window.h"

#include "Engine/Core/Application.h"
#include "Engine/Renderer/OpenGLContext.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include <GLFW/glfw3.h>

namespace CitrusGL
{
	static bool GLFWInitialised = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		CGL_CORE_ERROR("GLFW ERROR ({0}): {1}", error, description);
	}

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		context->SwapBuffers();
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		data.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return data.VSync;
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new Window(props);
	}

	void Window::Init(const WindowProps& props)
	{
		data.Title = props.Title;
		data.Width = props.Width;
		data.Height = props.Height;

		CGL_CORE_INFO("Creating window {0} ({1}, {2})", props.Title.c_str(), props.Width, props.Height);

		if (!GLFWInitialised)
		{
			int success = glfwInit();
			CGL_CORE_ASSERT(success, "GLFW failed to initialise!");
			glfwSetErrorCallback(GLFWErrorCallback);
			GLFWInitialised = true;
		}

		window = glfwCreateWindow((int)props.Width, (int)props.Height, data.Title.c_str(), nullptr, nullptr);

		context = new OpenGLContext(window);
		context->Init();

		glfwSetWindowUserPointer(window, &data);
		SetVSync(true);

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
								  {
									  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
									  data.Width = width;
									  data.Height = height;

									  WindowResizeEvent event(width, height);
									  data.eventCallback(event);
								  });

		//glfwSetWindowRefreshCallback(window, [](GLFWwindow* window)
		//							 {
		//								 WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);



		//								 WindowRefreshEvent event;
		//								 data.eventCallback(event);
		//							 });

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
								   {
									   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

									   WindowCloseEvent event;
									   data.eventCallback(event);
								   });

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
						   {
							   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							   switch (action)
							   {
							   case GLFW_PRESS:
							   {
								   KeyPressedEvent event(key, 0);
								   data.eventCallback(event);
								   break;
							   }
							   case GLFW_RELEASE:
							   {
								   KeyReleasedEvent event(key);
								   data.eventCallback(event);
								   break;
							   }
							   case GLFW_REPEAT:
							   {
								   KeyPressedEvent event(key, 1);
								   data.eventCallback(event);
								   break;
							   }
							   }
						   });

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
							{
								WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								KeyTypedEvent event(keycode);
								data.eventCallback(event);
							});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
								   {
									   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

									   switch (action)
									   {
									   case GLFW_PRESS:
									   {
										   MouseButtonPressedEvent event(button);
										   data.eventCallback(event);
										   break;
									   }
									   case GLFW_RELEASE:
									   {
										   MouseButtonReleasedEvent event(button);
										   data.eventCallback(event);
										   break;
									   }
									   }
								   });

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xPos, double yPos)
							  {
								  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								  MouseScrolledEvent event((float)xPos, (float)yPos);
								  data.eventCallback(event);
							  });
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(window);
	}
}
