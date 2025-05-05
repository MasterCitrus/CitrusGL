#include "ModelViewer.h"
#include "Animation.h"
#include "Animator.h"
#include "Input.h"
#include "Key.h"
#include "Light.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>

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
	sun = new DirectionLight({ 0.0f, -0.5f, -1.0f }, { 0.05f, 0.05f, 0.05f }, { 0.4f, 0.4f, 0.4f }, { 0.5f, 0.05f, 0.5f });
}

ModelViewer::~ModelViewer()
{

}

bool ModelViewer::Initialise()
{
	camera = new Camera(45.0f, (float)GetWindow().GetWidth() / (float)GetWindow().GetHeight(), 0.1f, 1000.0f);
	camera->SetWindow(&GetWindow());
	staticMeshShader = assetManager.CreateShader("Temp", "Assets/Shaders/blinnphong.vs");
	skinnedMeshShader = assetManager.CreateShader("Skinned", "Assets/Shaders/skinnedblinnphong.vs");
	if (!staticMeshShader.Link())
	{
		std::cout << "Shader link error: " << staticMeshShader.GetLastError() << '\n';
		return false;
	}
	if( !skinnedMeshShader.Link() )
	{
		std::cout << "Shader link error: " << skinnedMeshShader.GetLastError() << '\n';
		return false;
	}

	testModel = new Model("Assets/Models/person.gltf", skinnedMeshShader);

	framebuffer = new Framebuffer(GetWindow().GetWidth(), GetWindow().GetHeight());

	glEnable(GL_DEPTH_TEST);

	return true;
}

void ModelViewer::Shutdown()
{
	delete camera;
	delete testModel;
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
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	framebuffer->Bind();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	staticMeshShader.Bind();

	staticMeshShader.SetVec("dirLight.direction", sun->GetPosition());
	staticMeshShader.SetVec("dirLight.ambient", sun->GetAmbient());
	staticMeshShader.SetVec("dirLight.diffuse", sun->GetDiffuse());
	staticMeshShader.SetVec("dirLight.specular", sun->GetSpecular());
	staticMeshShader.SetVec("viewPos", camera->GetPosition());
	

	staticMeshShader.SetMat("projection", camera->GetProjectionMatrix());
	staticMeshShader.SetMat("view", camera->GetViewMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	staticMeshShader.SetMat("model", model);

	skinnedMeshShader.Bind();

	skinnedMeshShader.SetVec("dirLight.direction", sun->GetPosition());
	skinnedMeshShader.SetVec("dirLight.ambient", sun->GetAmbient());
	skinnedMeshShader.SetVec("dirLight.diffuse", sun->GetDiffuse());
	skinnedMeshShader.SetVec("dirLight.specular", sun->GetSpecular());
	skinnedMeshShader.SetVec("viewPos", camera->GetPosition());

	skinnedMeshShader.SetMat("projection", camera->GetProjectionMatrix());
	skinnedMeshShader.SetMat("view", camera->GetViewMatrix());

	skinnedMeshShader.SetMat("model", model);

	testModel->Draw();

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
			if (ImGui::BeginMenu("Open"))
			{
				if( ImGui::MenuItem("Load Model") )
				{

				}
				if( ImGui::MenuItem("Load Skinned Model") )
				{

				}
				ImGui::EndMenu();
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
	static auto& prop = testModel->GetMeshes()[selectedMesh].GetMaterial()->GetMaterialProperties().begin()->second;
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
					ImGui::Text(testModel->GetMeshes()[i].GetName().c_str());
				}
				ImGui::EndTable();
			}
			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Materials"))
			{
				ImGui::Text("Properties List");
				static auto selectedProp = testModel->GetMeshes()[selectedMesh].GetMaterial()->GetMaterialProperties().begin();
				if (ImGui::BeginCombo("##MatProps", selectedProp->first.c_str()))
				{
					for (auto it = testModel->GetMeshes()[selectedMesh].GetMaterial()->GetMaterialProperties().begin(); it != testModel->GetMeshes()[selectedMesh].GetMaterial()->GetMaterialProperties().end(); it++)
					{
						if (ImGui::Selectable(it->first.c_str(), it->first == selectedProp->first))
						{
							selectedProp = it;
							prop = it->second;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::SeparatorText("Material Properties");
				if (std::holds_alternative<int>(prop.value))
				{
					ImGui::Text(prop.name.c_str());
					ImGui::InputInt("##Int", &std::get<int>(prop.value));
				}
				else if (std::holds_alternative<float>(prop.value))
				{
					ImGui::Text(prop.name.c_str());
					ImGui::InputFloat("##Float", &std::get<float>(prop.value));
				}
				else if (std::holds_alternative<bool>(prop.value))
				{
					ImGui::Text(prop.name.c_str());
					ImGui::Checkbox("##Bool", &std::get<bool>(prop.value));
				}
				else if (std::holds_alternative<glm::vec2>(prop.value))
				{
					ImGui::Text(prop.name.c_str());
					ImGui::InputFloat2("##Vec2", &std::get<glm::vec2>(prop.value)[0]);
				}
				else if (std::holds_alternative<glm::vec3>(prop.value))
				{
					if (prop.isColour)
					{
						ImGui::Text(prop.name.c_str());
						ImGui::ColorPicker3("##Vec3", &std::get<glm::vec3>(prop.value)[0]);
					}
					else
					{
						ImGui::Text(prop.name.c_str());
						ImGui::InputFloat3("##Vec3", &std::get<glm::vec3>(prop.value)[0]);
					}
				}
				else if (std::holds_alternative<glm::vec4>(prop.value))
				{
					if (prop.isColour)
					{
						ImGui::Text(prop.name.c_str());
						ImGui::ColorPicker4("##Vec4", &std::get<glm::vec4>(prop.value)[0]);
					}
					else
					{
						ImGui::Text(prop.name.c_str());
						ImGui::InputFloat4("##Vec4", &std::get<glm::vec4>(prop.value)[0]);
					}
				}
				else if (std::holds_alternative<Texture*>(prop.value))
				{
					ImGui::Text(prop.name.c_str());
					ImGui::Image(std::get<Texture*>(prop.value)->GetTextureID(), {256, 256});
				}
			}
			if (ImGui::CollapsingHeader("Animations"))
			{
				ImGui::Text("Animations");
				if( ImGui::BeginCombo("##AnimationList", testModel->GetAnimator()->GetCurrentAnimation()->GetName().c_str()) )
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
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Scene Settings"))
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
			if( ImGui::BeginCombo("##PointLightList", lights.size() > 0 ? ("Light " + std::to_string(selectedLight + 1 )).c_str() : "") )
			{
				for( int i = 0; i < lights.size(); i++ )
				{
					if( ImGui::Selectable(( "Light " + std::to_string(i + 1) ).c_str(), selectedLight == i) )
					{
						selectedLight = i;
					}
				}
				ImGui::EndCombo();

				if(lights.size() > 0 )
				{
					ImGui::Text("Position");
					ImGui::DragFloat3("##PointLightDirection", &lights[selectedLight]->GetPosition()[0], 0.01f, -1.0f, 1.0f, "%.2f");
					ImGui::Text("Ambient");
					ImGui::ColorEdit3("##PointLightAmbient", &lights[selectedLight]->GetAmbient()[0]);
					ImGui::Text("Diffuse");
					ImGui::ColorEdit3("##PointLightDiffuse", &lights[selectedLight]->GetDiffuse()[0]);
					ImGui::Text("Specular");
					ImGui::DragFloat3("##PointLightSpecular", &lights[selectedLight]->GetSpecular()[0], 0.01f, 0.0f, 1.0f, "%.2f");
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
