#pragma once

class Window;

#include <utility>

class Input
{
public:
	Input() = default;

	static bool IsKeyPressed(int keycode, Window* window);
	static bool IsMouseButtonPressed(int button, Window* window);
	static std::pair<float, float> GetMousePosition(Window* window);
	static float GetMouseX(Window* window);
	static float GetMouseY(Window* window);
};