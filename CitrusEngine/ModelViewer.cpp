#include "ModelViewer.h"
#include "Key.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>

ModelViewer::ModelViewer()
{

}

ModelViewer::~ModelViewer()
{

}

bool ModelViewer::Initialise()
{
	camera = new Camera(45.0f, (float)GetWindow().GetWidth() / (float)GetWindow().GetHeight(), 0.1f, 1000.0f);
	camera->SetWindow(&GetWindow());
	shader = assetManager.CreateShader("Temp", "Assets/Shaders/blinnphong.vs");
	if (!shader.Link())
	{
		std::cout << "Shader link error: " << shader.GetLastError() << '\n';
		return false;
	}
	testModel = new Model("Assets/Models/soulspear.obj", shader);

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
}

void ModelViewer::Draw()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	framebuffer->Bind();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Bind();

	glm::vec3 lightPos = { 0.0f, 0.0f, 3.0f };
	glm::vec3 viewPos = { 0.0f, 0.0f, 3.0f };
	glm::vec3 lightColour = { 1.0f, 1.0f, 1.0f };

	shader.SetVec("lightPos", lightPos);
	shader.SetVec("viewPos", viewPos);
	shader.SetVec("lightColour", lightColour);

	shader.SetMat("projection", camera->GetProjectionMatrix());
	shader.SetMat("view", camera->GetViewMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	shader.SetMat("model", model);

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
			if (ImGui::MenuItem("Open", "Ctrl + O"))
			{

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
						if (ImGui::Selectable(it->first.c_str(), selectedProp->first == it->first))
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
						ImGui::InputFloat3("##Vec3", &std::get<glm::vec4>(prop.value)[0]);
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
						ImGui::InputFloat4("##Vec4", &std::get<glm::vec3>(prop.value)[0]);
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

			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Scene Settings"))
		{
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
