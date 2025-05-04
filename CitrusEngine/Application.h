#pragma once
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

#include <string>
#include <memory>

class Application
{
public:
	Application();
	virtual ~Application();

	void Run(const std::string& title, unsigned int width, unsigned int height, bool fullscreen);

	virtual bool Initialise() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float delta) = 0;
	virtual void Draw() = 0;
	virtual void ImGuiDraw() = 0;

	virtual void OnEvent(Event& e);

	void Quit() { running = false; }

	bool& GetFullscreen() { return fullscreen; }
	Window& GetWindow() { return *window; }
	unsigned int GetFPS() const { return fps; }

protected:
	virtual bool OnKeyPressed(KeyPressedEvent& e) { return false; }
	virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e) { return false; }

private:
	bool OnWindowClose( WindowCloseEvent& e );

private:
	Window* window;
	unsigned int fps;
	bool running = true;
	bool fullscreen;
	bool showDemoWindow = false;
};

