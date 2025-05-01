#pragma once
#include "Application.h"

class ModelViewer : public Application
{
public:
	ModelViewer();
	virtual ~ModelViewer();

	bool Initialise() override;
	void Shutdown() override;
	void Update( float delta ) override;
	void Draw() override;
	void ImGuiDraw() override;

	void OnEvent( Event& e ) override;

protected:
	bool OnKeyPressed( KeyPressedEvent& e ) override;
	bool OnMouseButtonPressed( MouseButtonPressedEvent& e ) override;

private:
	bool viewportFocused;
	bool viewportHovered;
};

