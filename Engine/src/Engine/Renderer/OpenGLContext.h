#pragma once

struct GLFWwindow;

namespace CitrusGL
{
	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* windowHandle;
	};
}