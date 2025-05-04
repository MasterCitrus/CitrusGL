#pragma once
#include "Application.h"
#include "AssetManager.h"
#include "Model.h"
#include "Camera.h"
#include "Framebuffer.h"

struct MaterialProperty;

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
	AssetManager assetManager;
	Model* testModel;
	Shader shader;
	Framebuffer* framebuffer;
	Camera* camera;
	bool viewportFocused;
	bool viewportHovered;

	//ImGui Variables
	int selectedMesh = 0;
	int selectedMatProp = 0;
};

