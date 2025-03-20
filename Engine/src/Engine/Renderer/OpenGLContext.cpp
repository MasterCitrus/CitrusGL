#include "cglpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace CitrusGL
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : windowHandle(windowHandle)
	{
		CGL_CORE_ASSERT(windowHandle, "Window is null reference.");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CGL_CORE_ASSERT(status, "Failed to initialise glad!");

		const unsigned char* vendor = glGetString(GL_VENDOR);
		const unsigned char* renderer = glGetString(GL_RENDERER);
		const unsigned char* version = glGetString(GL_VERSION);

		CGL_CORE_INFO("OpenGL Info:");
		CGL_CORE_INFO("  Vender: {0}", (const char*)vendor);
		CGL_CORE_INFO("  Renderer: {0}", (const char*)renderer);
		CGL_CORE_INFO("  Version: {0}", (const char*)version);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(windowHandle);
	}
}