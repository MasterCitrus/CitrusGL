#pragma once
#include "Application.h"
#include "AssetManager.h"
#include "Model.h"
#include "Camera.h"
#include "Framebuffer.h"

class DirectionLight;
class PointLight;
class Skybox;

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
	std::map<std::string, Material*> CreateMaterialsFromMaterial(Material* material) const;
	std::vector<Skybox*> CreateSkyboxes() const;

private:
	std::vector<PointLight*> lights;
	std::map<std::string, Material*> materials;
	DirectionLight* sun;
	AssetManager assetManager;
	Model* testModel;
	Shader blinnPhong;
	Shader* skyboxShader;
	Framebuffer* framebuffer;
	Camera* camera;
	MaterialProperty* prop;
	bool viewportFocused;
	bool viewportHovered;

	std::vector<Skybox*> skyboxes;

	std::map<std::string, Material*>::iterator selectedMaterial;
	std::map<std::string, MaterialProperty>::iterator selectedProp;

	//ImGui Variables
	int selectedMesh = 0;
	int selectedMatProp = 0;
	int selectedAnimation = 0;
	int selectedLight = 0;
	int selectedSkybox = 0;
};

