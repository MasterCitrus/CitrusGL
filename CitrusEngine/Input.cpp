#include "Input.h"

#include "Application.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int keycode, Window* window)
{
	auto win = static_cast<GLFWwindow*>( window->GetNativeWindow() );
	auto state = glfwGetKey(win, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button, Window* window)
{
	auto win = static_cast<GLFWwindow*>( window->GetNativeWindow() );
	auto state = glfwGetMouseButton(win, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition(Window* window)
{
	auto win = static_cast<GLFWwindow*>( window->GetNativeWindow() );
	double xPos, yPos;
	glfwGetCursorPos(win, &xPos, &yPos);
	return { xPos, yPos };
}

float Input::GetMouseX(Window* window)
{
	auto [x, y] = GetMousePosition(window);
	return x;
}

float Input::GetMouseY(Window* window)
{
	auto [x, y] = GetMousePosition(window);
	return y;
}
