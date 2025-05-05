#pragma once
#include "Application.h"
#include "AssetManager.h"
#include "Model.h"
#include "Camera.h"
#include "Framebuffer.h"

class DirectionLight;
class PointLight;

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
	std::vector<Material*> CreateMaterials() const;

private:
	std::vector<PointLight*> lights;
	std::vector<Material*> materials;
	DirectionLight* sun;
	AssetManager assetManager;
	Model* testModel;
	Shader staticMeshShader;
	Shader skinnedMeshShader;
	Framebuffer* framebuffer;
	Camera* camera;
	MaterialProperty* prop;
	bool viewportFocused;
	bool viewportHovered;

	//ImGui Variables
	int selectedMesh = 0;
	int selectedMatProp = 0;
	int selectedAnimation = 0;
	int selectedLight = 0;
};

