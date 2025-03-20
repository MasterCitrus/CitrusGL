#pragma once


namespace CitrusGL
{
	class Input
	{
	public:
	public:
		bool IsKeyPressed(int keycode);

		bool IsMouseButtonPressed(int button);
		std::pair<float, float> GetMousePosition();
		float GetMouseX();
		float GetMouseY();
	};
}