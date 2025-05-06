#include "ModelViewer.h"
#include "Animation.h"
#include "Animator.h"
#include "Cubemap.h"
#include "Input.h"
#include "Key.h"
#include "Light.h"
#include "Material.h"
#include "Skybox.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>
#include <nfd.hpp>

#include <filesystem>

static int InputTextResizeCallback(ImGuiInputTextCallbackData* data)
{
	if( data->EventFlag == ImGuiInputTextFlags_CallbackResize )
	{
		std::string str = (const char*)data->UserData;
	}
	return 0;
}

ModelViewer::ModelViewer()
{
	sun = new DirectionLight({ 0.0f, -0.5f, -1.0f }, { 0.05f, 0.05f, 0.05f }, { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f });
}

ModelViewer::~ModelViewer()
{

}

bool ModelViewer::Initialise()
{
	camera = new Camera(45.0f, (float)GetWindow().GetWidth() / (float)GetWindow().GetHeight(), 0.1f, 1000.0f);
	camera->SetWindow(&GetWindow());
	blinnPhong = assetManager.CreateShader("Blinn-Phong", "Assets/Shaders/blinnphong.vs");
	skyboxShader = &assetManager.CreateShader("Skybox", "Assets/Shaders/skybox.vs");
	if (!blinnPhong.Link())
	{
		std::cout << "Shader link error: " << blinnPhong.GetLastError() << '\n';
		return false;
	}

	if( !skyboxShader->Link() )
	{
		std::cout << "Shader link error: " << skyboxShader->GetLastError() << '\n';
		return false;
	}

	skyboxes = CreateSkyboxes();

	testModel = new Model("Assets/Models/person.gltf", &blinnPhong);

	framebuffer = new Framebuffer(GetWindow().GetWidth(), GetWindow().GetHeight());

	prop = &testModel->GetMeshes()[selectedMesh]->GetMaterial()->GetMaterialProperties().begin()->second;

	PointLight* l1 = new PointLight({ -1.5, 2.0f, 0.0f }, { 0.05f, 0.05f, 0.05f }, { 1.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f });
	PointLight* l2 = new PointLight({ 0.0, 2.0f, 1.0f }, { 0.05f, 0.05f, 0.05f }, { 0.0f, 1.0f, 0.0f }, { 0.5f, 0.5f, 0.5f });
	PointLight* l3 = new PointLight({ 1.5, 2.0f, 0.0f }, { 0.05f, 0.05f, 0.05f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.5f });

	lights.push_back(l1);
	lights.push_back(l2);
	lights.push_back(l3);

	materials = CreateMaterialsFromMaterial(testModel->GetMeshes()[selectedMesh]->GetMaterial());
	for( int i = 0; i < testModel->GetMeshes().size(); i++ )
	{
		materials[( "Mesh" + std::to_string(i + 1) )] = testModel->GetMeshes()[i]->GetMaterial();
	}
	selectedMaterial = materials.find( "Mesh" + std::to_string(selectedMesh + 1) );
	selectedProp = testModel->GetMeshes()[selectedMesh]->GetMaterial()->GetMaterialProperties().begin();

	return true;
}

void ModelViewer::Shutdown()
{
	for( auto& skybox : skyboxes ) delete skybox;
	skyboxes.clear();

	for (auto& light : lights) delete light;
	lights.clear();

	for (auto& mat : materials) delete mat.second;
	materials.clear();

	std::cout << "Deleting camera\n";
	delete camera;
	std::cout << "Deleting model\n";
	delete testModel;
	//std::cout << "Deleting skybox shader\n";
	//delete skyboxShader;
	delete sun;
	delete framebuffer;
}

void ModelViewer::Update( float delta )
{
	camera->Update(delta);
	testModel->Update(delta);

	if( Input::IsKeyPressed(Key::LEFT_CONTROL || Key::RIGHT_CONTROL, &GetWindow()) )
	{
		if( Input::IsKeyPressed(Key::O, &GetWindow()) )
		{
			std::cout << "Open file\n";
		}
	}
}

void ModelViewer::Draw()
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	framebuffer->Bind();

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	blinnPhong.Bind();

	blinnPhong.SetVec("dirLight.direction", sun->GetPosition());
	blinnPhong.SetVec("dirLight.ambient", sun->GetAmbient());
	blinnPhong.SetVec("dirLight.diffuse", sun->GetDiffuse());
	blinnPhong.SetVec("dirLight.specular", sun->GetSpecular());
	blinnPhong.SetVec("viewPos", camera->GetPosition());

	for (int i = 0; i < lights.size(); i++)
	{
		blinnPhong.SetVec(("pointLights[" + std::to_string(i) + "].position"), lights[i]->GetPosition());
		blinnPhong.SetVec(("pointLights[" + std::to_string(i) + "].ambient"), lights[i]->GetAmbient());
		blinnPhong.SetVec(("pointLights[" + std::to_string(i) + "].diffuse"), lights[i]->GetDiffuse());
		blinnPhong.SetVec(("pointLights[" + std::to_string(i) + "].specular"), lights[i]->GetSpecular());
		blinnPhong.SetFloat(("pointLights[" + std::to_string(i) + "].linear"), lights[i]->GetLinear());
		blinnPhong.SetFloat(("pointLights[" + std::to_string(i) + "].quadratic"), lights[i]->GetQuadratic());
		blinnPhong.SetFloat(( "pointLights[" + std::to_string(i) + "].intensity" ), lights[i]->GetIntensity());
	}
	
	blinnPhong.SetMat("projection", camera->GetProjectionMatrix());
	blinnPhong.SetMat("view", camera->GetViewMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	blinnPhong.SetMat("model", model);
	if( testModel->GetBoneCount() > 0 )
	{
		blinnPhong.SetBool("isSkinned", true);
	}
	else
	{
		blinnPhong.SetBool("isSkinned", false);
	}

	testModel->Draw();

	skyboxes[selectedSkybox]->Draw();

	framebuffer->Unbind();
}

void ModelViewer::ImGuiDraw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoTabBar;

	ImGuiID dockspaceID = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockspaceFlags);

	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;

		ImGui::DockBuilderRemoveNode(dockspaceID);
		ImGui::DockBuilderAddNode(dockspaceID, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetWindowSize());

		ImGuiID mainDockspaceID = dockspaceID;
		ImGuiID right = ImGui::DockBuilderSplitNode(mainDockspaceID, ImGuiDir_Right, 0.80f, nullptr, &mainDockspaceID);

		ImGui::DockBuilderDockWindow("Viewport", mainDockspaceID);
		ImGui::DockBuilderDockWindow("InfoPanel", right);
		ImGui::DockBuilderFinish(dockspaceID);
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if ( ImGui::MenuItem("Load Model") )
			{
				NFD::Guard guard;
				NFD::UniquePath uPath;

				nfdfilteritem_t filter[1] = {{"3D Models", "fbx,gltf,obj"}};

				nfdresult_t result = NFD::OpenDialog(uPath, filter, 1);

				std::string path;

				if( result == NFD_OKAY )
				{
					path = uPath.get();
					delete testModel;
					testModel = nullptr;
					testModel = new Model(path, &blinnPhong);

					for( auto& mat : materials ) delete mat.second;
					materials.clear();

					for( int i = 0; i < testModel->GetMeshes().size(); i++ )
					{
						auto temp = CreateMaterialsFromMaterial(testModel->GetMeshes()[i]->GetMaterial());
						materials.insert(temp.begin(), temp.end());
					}
					for( int i = 0; i < testModel->GetMeshes().size(); i++ )
					{
						materials[( "Mesh" + std::to_string(i + 1) )] = testModel->GetMeshes()[i]->GetMaterial();
					}

					int selectedMesh = 0;
					int selectedMatProp = 0;
					int selectedAnimation = 0;

					selectedMaterial = materials.find("Mesh" + std::to_string(selectedMesh + 1));
					selectedProp = testModel->GetMeshes()[selectedMesh]->GetMaterial()->GetMaterialProperties().begin();
					prop = &selectedProp->second;
				}
				else
				{
					std::cout << NFD::GetError() << '\n';
				}
			}
			if (ImGui::MenuItem("Exit", "Alt + F4"))
			{
				Quit();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	bool viewportBool = true;
	ImGui::Begin("Viewport", &viewportBool, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImVec2 viewport = ImGui::GetContentRegionAvail();

	viewportFocused = ImGui::IsWindowFocused();
	viewportHovered = ImGui::IsWindowHovered();

	if (FSpec spec = framebuffer->GetSpec(); viewport.x > 0.0f && viewport.y > 0.0f && (spec.width != viewport.x || spec.height != viewport.y))
	{
		framebuffer->Resize((unsigned int)viewport.x, (unsigned int)viewport.y);
		camera->SetViewportSize(viewport.x, viewport.y);
	}

	ImGui::Image(framebuffer->GetColourTexture(), viewport, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	bool infoPanelBool = true;
	ImGui::Begin("InfoPanel", &infoPanelBool, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	if (ImGui::BeginTabBar("Options"))
	{
		if (ImGui::BeginTabItem("Model Settings"))
		{
			ImGui::SeparatorText("Meshes");
			if (ImGui::BeginTable("MeshTable", 2, ImGuiTableFlags_BordersOuter))
			{
				
				for (int i = 0; i < testModel->GetMeshes().size(); i++)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					if(ImGui::Selectable(std::to_string(i).c_str(), selectedMesh == i, ImGuiSelectableFlags_SpanAllColumns))
					{
						selectedMesh = i;
					}
					ImGui::TableNextColumn();
					ImGui::Text(testModel->GetMeshes()[i]->GetName().c_str());
				}
				ImGui::EndTable();
			}
			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Materials"))
			{
				ImGui::Text("Materials List");
				if( ImGui::BeginCombo("##MaterialsList", selectedMaterial->first.c_str()) )
				{
					for( auto it = materials.begin(); it != materials.end(); it++ )
					{
						//ImGui::Image(,{32, 32});
						if( ImGui::Selectable(it->first.c_str(), it->first == selectedMaterial->first) )
						{
							selectedMaterial = it;
							testModel->GetMeshes()[selectedMesh]->SetMaterial(it->second);
							selectedProp = it->second->GetMaterialProperties().begin();
							prop = &selectedProp->second;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Text("Properties List");
				if (ImGui::BeginCombo("##MatProps", selectedProp->first.c_str()))
				{
					for (auto it = testModel->GetMeshes()[selectedMesh]->GetMaterial()->GetMaterialProperties().begin(); it != testModel->GetMeshes()[selectedMesh]->GetMaterial()->GetMaterialProperties().end(); it++)
					{
						if (ImGui::Selectable(it->first.c_str(), it->first == selectedProp->first))
						{
							selectedProp = it;
							prop = &selectedProp->second;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SeparatorText("Material Property Settings");
				if (std::holds_alternative<int>((*prop).value))
				{
					ImGui::Text((*prop).name.c_str());
					switch( prop->type )
					{
					case MaterialPropertyDisplayType::INPUT:
						ImGui::InputInt("##Int", &std::get<int>(( *prop ).value));
						break;
					case MaterialPropertyDisplayType::DRAG:
						ImGui::DragInt("##Int", &std::get<int>(( *prop ).value), 0.01f , std::get<int>(prop->minValue), std::get<int>(prop->maxValue));
						break;
					case MaterialPropertyDisplayType::SLIDER:
						ImGui::SliderInt("##Int", &std::get<int>(( *prop ).value), std::get<int>(prop->minValue), std::get<int>(prop->maxValue));
						break;
					default:
						ImGui::InputInt("##Int", &std::get<int>(( *prop ).value));
						break;
					}
				}
				else if (std::holds_alternative<float>((*prop).value))
				{
					ImGui::Text((*prop).name.c_str());
					switch( prop->type )
					{
					case MaterialPropertyDisplayType::INPUT:
						ImGui::InputFloat("##Float", &std::get<float>(( *prop ).value));
						break;
					case MaterialPropertyDisplayType::DRAG:
						ImGui::DragFloat("##Float", &std::get<float>(( *prop ).value), 0.0f, std::get<float>(prop->minValue), std::get<float>(prop->maxValue));
						break;
					case MaterialPropertyDisplayType::SLIDER:
						ImGui::SliderFloat("##Float", &std::get<float>(( *prop ).value), std::get<float>(prop->minValue), std::get<float>(prop->maxValue), "%.2f");
						break;
					default:
						ImGui::InputFloat("##Int", &std::get<float>(( *prop ).value));
						break;
					}
				}
				else if (std::holds_alternative<bool>((*prop).value))
				{
					ImGui::Text((*prop).name.c_str());
					ImGui::Checkbox("##Bool", &std::get<bool>((*prop).value));
				}
				else if (std::holds_alternative<glm::vec2>((*prop).value))
				{
					ImGui::Text((*prop).name.c_str());
					switch( prop->type )
					{
					case MaterialPropertyDisplayType::INPUT:
						ImGui::InputFloat2("##Float", &std::get<glm::vec2>(( *prop ).value)[0]);
						break;
					case MaterialPropertyDisplayType::DRAG:
						ImGui::DragFloat2("##Float", &std::get<glm::vec2>(( *prop ).value)[0], 0.0f, std::get<float>(prop->minValue), std::get<float>(prop->maxValue));
						break;
					case MaterialPropertyDisplayType::SLIDER:
						ImGui::SliderFloat2("##Float", &std::get<glm::vec2>(( *prop ).value)[0], std::get<float>(prop->minValue), std::get<float>(prop->maxValue), "%.2f");
						break;
					default:
						ImGui::InputFloat2("##Int", &std::get<glm::vec2>(( *prop ).value)[0]);
						break;
					}
				}
				else if (std::holds_alternative<glm::vec3>((*prop).value))
				{
					ImGui::Text((*prop).name.c_str());
					switch( prop->type )
					{
					case MaterialPropertyDisplayType::INPUT:
						ImGui::InputFloat3("##glm::vec3", &std::get<glm::vec3>(( *prop ).value)[0]);
						break;
					case MaterialPropertyDisplayType::DRAG:
						ImGui::DragFloat3("##glm::vec3", &std::get<glm::vec3>(( *prop ).value)[0], 0.0f, std::get<float>(prop->minValue), std::get<float>(prop->maxValue));
						break;
					case MaterialPropertyDisplayType::SLIDER:
						ImGui::SliderFloat3("##glm::vec3", &std::get<glm::vec3>(( *prop ).value)[0], std::get<float>(prop->minValue), std::get<float>(prop->maxValue), "%.2f");
						break;
					case MaterialPropertyDisplayType::COLOUREDIT:
						ImGui::ColorEdit3("##glm::vec3", &std::get<glm::vec3>(( *prop ).value)[0]);
						break;
					case MaterialPropertyDisplayType::COLOURPICKER:
						ImGui::ColorPicker3("##glm::vec3", &std::get<glm::vec3>(( *prop ).value)[0]);
						break;
					default:
						ImGui::InputFloat3("##Int", &std::get<glm::vec3>(( *prop ).value)[0]);
						break;
					}
					
				}
				else if (std::holds_alternative<glm::vec4>((*prop).value))
				{
					ImGui::Text(( *prop ).name.c_str());
					switch( prop->type )
					{
					case MaterialPropertyDisplayType::INPUT:
						ImGui::InputFloat4("##glm::vec4", &std::get<glm::vec4>(( *prop ).value)[0]);
						break;
					case MaterialPropertyDisplayType::DRAG:
						ImGui::DragFloat4("##glm::vec4", &std::get<glm::vec4>(( *prop ).value)[0], 0.0f, std::get<float>(prop->minValue), std::get<float>(prop->maxValue));
						break;
					case MaterialPropertyDisplayType::SLIDER:
						ImGui::SliderFloat4("##glm::vec4", &std::get<glm::vec4>(( *prop ).value)[0], std::get<float>(prop->minValue), std::get<float>(prop->maxValue), "%.2f");
						break;
					case MaterialPropertyDisplayType::COLOUREDIT:
						ImGui::ColorEdit4("##glm::vec4", &std::get<glm::vec4>(( *prop ).value)[0]);
						break;
					case MaterialPropertyDisplayType::COLOURPICKER:
						ImGui::ColorPicker4("##glm::vec4", &std::get<glm::vec4>(( *prop ).value)[0]);
						break;
					default:
						ImGui::InputFloat4("##Int", &std::get<glm::vec4>(( *prop ).value)[0]);
						break;
					}
				}
				else if (std::holds_alternative<Texture*>((*prop).value))
				{
					ImGui::Text((*prop).name.c_str());
					ImGui::Image(std::get<Texture*>((*prop).value)->GetTextureID(), {256, 256});
				}
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.5f);
				ImGui::Spacing();
			}
			if (ImGui::CollapsingHeader("Animations"))
			{
				ImGui::Text("Animations");
				if( ImGui::BeginCombo("##AnimationList", testModel->GetAnimator() != nullptr ? testModel->GetAnimator()->GetCurrentAnimation()->GetName().c_str() : "") )
				{
					for( int i = 0; i < testModel->GetAnimations().size(); i++ )
					{
						auto anim = testModel->GetAnimations()[i];
						if( ImGui::Selectable(anim->GetName().c_str(), selectedAnimation == i) )
						{
							selectedAnimation = i;
							testModel->GetAnimator()->PlayAnimation(anim);
						}
					}
					ImGui::EndCombo();
				}
				if( testModel->GetAnimator() != nullptr )
				{
					if( ImGui::Button("Play Animation") )
					{
						testModel->GetAnimator()->GetIsPlaying() = true;
					}
					ImGui::SameLine();
					if( ImGui::Button("Pause Animation") )
					{
						testModel->GetAnimator()->GetIsPlaying() = false;
					}
					ImGui::SeparatorText("Animation Properties");
					ImGui::Text("Animation Name");
					ImGui::InputText("##AnimationName", &testModel->GetAnimator()->GetCurrentAnimation()->GetName(), 0, InputTextResizeCallback);
					ImGui::Text("Duration: %.2f", testModel->GetAnimator()->GetCurrentAnimation()->GetDuration() / testModel->GetAnimator()->GetCurrentAnimation()->GetTicksPerSecond());
				}
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Scene Settings"))
		{
			if(ImGui::CollapsingHeader("Lights"))
			{
				ImGui::SeparatorText("Sun Light");
				ImGui::Text("Direction");
				ImGui::DragFloat3("##SunDirection", &sun->GetPosition()[0], 0.01f, -1.0f, 1.0f, "%.2f");
				ImGui::Text("Ambient");
				ImGui::ColorEdit3("##SunAmbient", &sun->GetAmbient()[0]);
				ImGui::Text("Diffuse");
				ImGui::ColorEdit3("##SunDiffuse", &sun->GetDiffuse()[0]);
				ImGui::Text("Specular");
				ImGui::DragFloat3("##SunSpecular", &sun->GetSpecular()[0], 0.01f, 0.0f, 1.0f, "%.2f");
				ImGui::SeparatorText("Point Lights");
				if (ImGui::BeginCombo("##PointLightList", lights.size() > 0 ? ("Light " + std::to_string(selectedLight + 1)).c_str() : ""))
				{
					for (int i = 0; i < lights.size(); i++)
					{
						if (ImGui::Selectable(("Light " + std::to_string(i + 1)).c_str(), selectedLight == i))
						{
							selectedLight = i;
						}
					}
					ImGui::EndCombo();

				}

				if (lights.size() > 0)
				{
					ImGui::Text("Position");
					ImGui::DragFloat3("##PointLightDirection", &lights[selectedLight]->GetPosition()[0], 0.01f, 0.0f, 0.0f, "%.2f");
					ImGui::Text("Ambient");
					ImGui::ColorEdit3("##PointLightAmbient", &lights[selectedLight]->GetAmbient()[0]);
					ImGui::Text("Diffuse");
					ImGui::ColorEdit3("##PointLightDiffuse", &lights[selectedLight]->GetDiffuse()[0]);
					ImGui::Text("Specular");
					ImGui::DragFloat3("##PointLightSpecular", &lights[selectedLight]->GetSpecular()[0], 0.01f, 0.0f, 1.0f, "%.2f");
					ImGui::Text("Linear");
					ImGui::InputFloat("##PointLightLinear", &lights[selectedLight]->GetLinear(), 0.001f, 0.01f, "%.4f");
					ImGui::Text("Quadratic");
					ImGui::InputFloat("##PointLightQuadratic", &lights[selectedLight]->GetQuadratic(), 0.001f, 0.01f, "%.6f");
					ImGui::Text("Intensity");
					ImGui::SliderFloat("##PointLightIntensity", &lights[selectedLight]->GetIntensity(), 0.001f, 100.0f, "%.6f");
				}
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.5f);
				ImGui::Spacing();
			}
			if (ImGui::CollapsingHeader("Skybox"))
			{
				ImGui::Text("Skybox List");
				if( ImGui::BeginCombo("##Skyboxes", skyboxes.size() > 0 ? skyboxes[selectedSkybox]->GetName().c_str() : "") )
				{
					for( int i = 0; i < skyboxes.size(); i++ )
					{
						if( ImGui::Selectable(skyboxes[i]->GetName().c_str(), selectedSkybox == i) )
						{
							selectedSkybox = i;
						}
					}
					ImGui::EndCombo();
				}
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void ModelViewer::OnEvent( Event& e )
{
	Application::OnEvent( e );
	if (viewportHovered || viewportFocused)
	{
		camera->OnEvent(e);
	}

	EventDispatcher dispatcher( e );
	dispatcher.Dispatch<KeyPressedEvent>( BIND_EVENT_FN( ModelViewer::OnKeyPressed ) );
	dispatcher.Dispatch<MouseButtonPressedEvent>( BIND_EVENT_FN( ModelViewer::OnMouseButtonPressed ) );
}

bool ModelViewer::OnKeyPressed( KeyPressedEvent& e )
{
	if (e.GetKeyCode() == Key::R)
	{
		camera->SetFocus({ 0.0f, 0.0f, 0.0f });
	}
	return false;
}

bool ModelViewer::OnMouseButtonPressed( MouseButtonPressedEvent& e )
{
	return false;
}

std::map<std::string, Material*> ModelViewer::CreateMaterialsFromMaterial(Material* material) const
{
	std::map<std::string, Material*> temp;

	//Crystals/Gems/Minerals
	//Emerald
	Material* emeraldCrystal = new Material(material);
	emeraldCrystal->SetVec("material.ambient", { 0.0215f, 0.1745f, 0.0215f }, MaterialPropertyDisplayType::COLOUREDIT);
	emeraldCrystal->SetVec("material.diffuse", { 0.07568f, 0.61424f, 0.07568f }, MaterialPropertyDisplayType::COLOUREDIT);
	emeraldCrystal->SetVec("material.specular", { 0.633f, 0.727811f, 0.633f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	emeraldCrystal->SetFloat("material.shininess", 76.8f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Emerald"] = emeraldCrystal;
	//Jade
	Material* jadeCrystal = new Material(material);
	jadeCrystal->SetVec("material.ambient", { 0.135f, 0.2225f, 0.1575f }, MaterialPropertyDisplayType::COLOUREDIT);
	jadeCrystal->SetVec("material.diffuse", { 0.54f, 0.89f, 0.63f }, MaterialPropertyDisplayType::COLOUREDIT);
	jadeCrystal->SetVec("material.specular", { 0.316228f, 0.316228f, 0.316228f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	jadeCrystal->SetFloat("material.shininess", 12.8f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Jade"] = jadeCrystal;
	//Obisidian
	Material* obisidianCrystal = new Material(material);
	obisidianCrystal->SetVec("material.ambient", { 0.05375f, 0.05f, 0.06625f }, MaterialPropertyDisplayType::COLOUREDIT);
	obisidianCrystal->SetVec("material.diffuse", { 0.18275f, 0.17f, 0.22525f }, MaterialPropertyDisplayType::COLOUREDIT);
	obisidianCrystal->SetVec("material.specular", { 0.332741f, 0.328634f, 0.346435f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	obisidianCrystal->SetFloat("material.shininess", 38.4f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Obsidian"] = obisidianCrystal;
	//Pearl
	Material* pearlCrystal = new Material(material);
	pearlCrystal->SetVec("material.ambient", { 0.25f, 0.20725f, 0.20725f }, MaterialPropertyDisplayType::COLOUREDIT);
	pearlCrystal->SetVec("material.diffuse", { 1.0f, 0.829f, 0.829f }, MaterialPropertyDisplayType::COLOUREDIT);
	pearlCrystal->SetVec("material.specular", { 0.296648f, 0.296648f, 0.296648f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	pearlCrystal->SetFloat("material.shininess", 11.264f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Pearl"] = pearlCrystal;
	//Ruby
	Material* rubyCrystal = new Material(material);
	rubyCrystal->SetVec("material.ambient", { 0.1745f, 0.01175f, 0.01175f }, MaterialPropertyDisplayType::COLOUREDIT);
	rubyCrystal->SetVec("material.diffuse", { 0.61424f, 0.04136f, 0.04136f }, MaterialPropertyDisplayType::COLOUREDIT);
	rubyCrystal->SetVec("material.specular", { 0.727811f, 0.626959f, 0.626959f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	rubyCrystal->SetFloat("material.shininess", 76.8f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Ruby"] = rubyCrystal;
	//Turquoise
	Material* turquoiseCrystal = new Material(material);
	turquoiseCrystal->SetVec("material.ambient", { 0.1f, 0.18725f, 0.1745f }, MaterialPropertyDisplayType::COLOUREDIT);
	turquoiseCrystal->SetVec("material.diffuse", { 0.396f, 0.74151f, 0.69102f }, MaterialPropertyDisplayType::COLOUREDIT);
	turquoiseCrystal->SetVec("material.specular", { 0.297254f, 0.30829f, 0.306678f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	turquoiseCrystal->SetFloat("material.shininess", 12.8f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Turquoise"] = turquoiseCrystal;

	//Metals
	//Brass
	Material* brassMetal = new Material(material);
	brassMetal->SetVec("material.ambient", { 0.329412f, 0.223529f, 0.027451f }, MaterialPropertyDisplayType::COLOUREDIT);
	brassMetal->SetVec("material.diffuse", { 0.780392f, 0.568627f, 0.113725f }, MaterialPropertyDisplayType::COLOUREDIT);
	brassMetal->SetVec("material.specular", { 0.992157f, 0.941176f, 0.807843f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	brassMetal->SetFloat("material.shininess",27.89743616f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Brass"] = brassMetal;
	//Bronze
	Material* bronzeMetal = new Material(material);
	bronzeMetal->SetVec("material.ambient", { 0.2125f, 0.1275f, 0.054f }, MaterialPropertyDisplayType::COLOUREDIT);
	bronzeMetal->SetVec("material.diffuse", { 0.714f, 0.4284f, 0.18144f }, MaterialPropertyDisplayType::COLOUREDIT);
	bronzeMetal->SetVec("material.specular", { 0.393548f, 0.271906f, 0.166721f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	bronzeMetal->SetFloat("material.shininess", 25.6f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Bronze"] = bronzeMetal;
	//Chrome
	Material* chromeMetal = new Material(material);
	chromeMetal->SetVec("material.ambient", { 0.25f, 0.25f, 0.25f }, MaterialPropertyDisplayType::COLOUREDIT);
	chromeMetal->SetVec("material.diffuse", { 0.4f, 0.4f, 0.4f }, MaterialPropertyDisplayType::COLOUREDIT);
	chromeMetal->SetVec("material.specular", { 0.774597f, 0.774597f, 0.774597f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	chromeMetal->SetFloat("material.shininess", 76.8f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Chrome"] = chromeMetal;
	//Copper
	Material* copperMetal = new Material(material);
	copperMetal->SetVec("material.ambient", { 0.19125f, 0.0735f, 0.0225f }, MaterialPropertyDisplayType::COLOUREDIT);
	copperMetal->SetVec("material.diffuse", { 0.7038f, 0.27048f, 0.0828f }, MaterialPropertyDisplayType::COLOUREDIT);
	copperMetal->SetVec("material.specular", { 0.256777f, 0.137622f, 0.086014f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	copperMetal->SetFloat("material.shininess", 12.8f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Copper"] = copperMetal;
	//Gold
	Material* goldMetal = new Material(material);
	goldMetal->SetVec("material.ambient", { 0.24725f, 0.1995f, 0.0745f }, MaterialPropertyDisplayType::COLOUREDIT);
	goldMetal->SetVec("material.diffuse", { 0.75164f, 0.60648f, 0.22648f }, MaterialPropertyDisplayType::COLOUREDIT);
	goldMetal->SetVec("material.specular", { 0.628281f, 0.555802f, 0.366065f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	goldMetal->SetFloat("material.shininess", 51.2f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Gold"] = goldMetal;
	//Silver
	Material* silverMetal = new Material(material);
	silverMetal->SetVec("material.ambient", { 0.19225f, 0.19225f, 0.19225f }, MaterialPropertyDisplayType::COLOUREDIT);
	silverMetal->SetVec("material.diffuse", { 0.50754f, 0.50754f, 0.50754f }, MaterialPropertyDisplayType::COLOUREDIT);
	silverMetal->SetVec("material.specular", { 0.508273f, 0.508273f, 0.508273f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	silverMetal->SetFloat("material.shininess", 51.2f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Silver"] = silverMetal;

	//Plastics
	//Red Plastic
	Material* redPlastic = new Material(material);
	redPlastic->SetVec("material.ambient", { 0.0f, 0.0f, 0.0f }, MaterialPropertyDisplayType::COLOUREDIT);
	redPlastic->SetVec("material.diffuse", { 0.5f, 0.0f, 0.0f }, MaterialPropertyDisplayType::COLOUREDIT);
	redPlastic->SetVec("material.specular", { 0.7f, 0.6f, 0.6f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	redPlastic->SetFloat("material.shininess", 32.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Red Plastic"] = redPlastic;
	//Green Plastic
	Material* greenPlastic = new Material(material);
	greenPlastic->SetVec("material.ambient", { 0.0f, 0.0f, 0.0f }, MaterialPropertyDisplayType::COLOUREDIT);
	greenPlastic->SetVec("material.diffuse", { 0.1f, 0.35f, 0.1f }, MaterialPropertyDisplayType::COLOUREDIT);
	greenPlastic->SetVec("material.specular", { 0.45f, 0.55f, 0.45f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	greenPlastic->SetFloat("material.shininess", 32.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Green Plastic"] = greenPlastic;
	//Yellow Plastic
	Material* yellowPlastic = new Material(material);
	yellowPlastic->SetVec("material.ambient", { 0.0f, 0.0f, 0.0f }, MaterialPropertyDisplayType::COLOUREDIT);
	yellowPlastic->SetVec("material.diffuse", { 0.5f, 0.5f, 0.0f }, MaterialPropertyDisplayType::COLOUREDIT);
	yellowPlastic->SetVec("material.specular", { 0.6f, 0.6f, 0.5f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	yellowPlastic->SetFloat("material.shininess", 32.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Yellow Plastic"] = yellowPlastic;
	//Cyan Plastic
	Material* cyanPlastic = new Material(material);
	cyanPlastic->SetVec("material.ambient", { 0.0f, 0.1f, 0.06f }, MaterialPropertyDisplayType::COLOUREDIT);
	cyanPlastic->SetVec("material.diffuse", { 0.0f, 0.50980392f, 0.50980392f }, MaterialPropertyDisplayType::COLOUREDIT);
	cyanPlastic->SetVec("material.specular", { 0.50196078f, 0.50196078f, 0.50196078f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	cyanPlastic->SetFloat("material.shininess", 32.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Cyan Plastic"] = cyanPlastic;
	//White Plastic
	Material* whitePlastic = new Material(material);
	whitePlastic->SetVec("material.ambient", { 0.0f, 0.0f, 0.0f }, MaterialPropertyDisplayType::COLOUREDIT);
	whitePlastic->SetVec("material.diffuse", { 0.55f, 0.55f, 0.55f }, MaterialPropertyDisplayType::COLOUREDIT);
	whitePlastic->SetVec("material.specular", { 0.70f, 0.70f, 0.70f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	whitePlastic->SetFloat("material.shininess", 32.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["White Plastic"] = whitePlastic;
	//Black Plastic
	Material* blackPlastic = new Material(material);
	blackPlastic->SetVec("material.ambient", { 0.02f, 0.02f, 0.02f }, MaterialPropertyDisplayType::COLOUREDIT);
	blackPlastic->SetVec("material.diffuse", { 0.01f, 0.01f, 0.01f }, MaterialPropertyDisplayType::COLOUREDIT);
	blackPlastic->SetVec("material.specular", { 0.5f, 0.5f, 0.5f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	blackPlastic->SetFloat("material.shininess", 32.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Black Plastic"] = blackPlastic;

	//Rubbers
	//Red Rubber
	Material* redRubber = new Material(material);
	redRubber->SetVec("material.ambient", {0.05f, 0.0f, 0.0f}, MaterialPropertyDisplayType::COLOUREDIT);
	redRubber->SetVec("material.diffuse", {0.5f, 0.4f, 0.4f}, MaterialPropertyDisplayType::COLOUREDIT);
	redRubber->SetVec("material.specular", {0.7f, 0.04f, 0.04f}, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	redRubber->SetFloat("material.shininess", 10.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Red Rubber"] = redRubber;
	//Green Rubber
	Material* greenRubber = new Material(material);
	greenRubber->SetVec("material.ambient", {0.0f, 0.05f, 0.0f}, MaterialPropertyDisplayType::COLOUREDIT);
	greenRubber->SetVec("material.diffuse", {0.4f, 0.5f, 0.4f}, MaterialPropertyDisplayType::COLOUREDIT);
	greenRubber->SetVec("material.specular", {0.04f, 0.7f, 0.04f}, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	greenRubber->SetFloat("material.shininess", 10.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Green Rubber"] = greenRubber;
	//Yellow Rubber
	Material* yellowRubber = new Material(material);
	yellowRubber->SetVec("material.ambient", { 0.05f, 0.05f, 0.0f }, MaterialPropertyDisplayType::COLOUREDIT);
	yellowRubber->SetVec("material.diffuse", { 0.5f, 0.5f, 0.4f }, MaterialPropertyDisplayType::COLOUREDIT);
	yellowRubber->SetVec("material.specular", { 0.7f, 0.7f, 0.04f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	yellowRubber->SetFloat("material.shininess", 10.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Yellow Rubber"] = yellowRubber;
	//Cyan Rubber
	Material* cyanRubber = new Material(material);
	cyanRubber->SetVec("material.ambient", { 0.0f, 0.05f, 0.05f }, MaterialPropertyDisplayType::COLOUREDIT);
	cyanRubber->SetVec("material.diffuse", { 0.4f, 0.5f, 0.5f }, MaterialPropertyDisplayType::COLOUREDIT);
	cyanRubber->SetVec("material.specular", { 0.04f, 0.7f, 0.7f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	cyanRubber->SetFloat("material.shininess", 10.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Cyan Rubber"] = cyanRubber;
	//White Rubber
	Material* whiteRubber = new Material(material);
	whiteRubber->SetVec("material.ambient", { 0.05f, 0.05f, 0.05f }, MaterialPropertyDisplayType::COLOUREDIT);
	whiteRubber->SetVec("material.diffuse", { 0.5f, 0.5f, 0.5f }, MaterialPropertyDisplayType::COLOUREDIT);
	whiteRubber->SetVec("material.specular", { 0.70f, 0.70f, 0.70f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	whiteRubber->SetFloat("material.shininess", 10.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["White Rubber"] = whiteRubber;
	//Black Rubber
	Material* blackRubber = new Material(material);
	blackRubber->SetVec("material.ambient", { 0.02f, 0.02f, 0.02f }, MaterialPropertyDisplayType::COLOUREDIT);
	blackRubber->SetVec("material.diffuse", { 0.01f, 0.01f, 0.01f }, MaterialPropertyDisplayType::COLOUREDIT);
	blackRubber->SetVec("material.specular", { 0.4f, 0.4f, 0.4f }, MaterialPropertyDisplayType::DRAG, 0.0f, 1.0f);
	blackRubber->SetFloat("material.shininess", 10.0f, MaterialPropertyDisplayType::SLIDER, 0.0f, 128.0f);
	temp["Black Rubber"] = blackRubber;

	return temp;
}

std::vector<Skybox*> ModelViewer::CreateSkyboxes() const
{
	std::vector<Skybox*> temp;

	std::map<std::string, Cubemap*> cubemaps;

	std::filesystem::path skyboxFolder(std::filesystem::current_path().string() + "\\Assets\\Skyboxes");

	for( const auto& path : std::filesystem::directory_iterator(skyboxFolder) )
	{
		std::string name = path.path().string();
		name = name.substr(name.find_last_of("\\") + 1);
		Cubemap* cubemap = new Cubemap(path.path().string());

		cubemaps.insert({ name, cubemap });
	}

	for( auto it = cubemaps.begin(); it != cubemaps.end(); it++ )
	{
		Skybox* skybox = new Skybox(it->first, it->second, skyboxShader, camera);
		temp.push_back(skybox);
	}

	return temp;
}
