#include "ModelViewer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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
	testModel = new Model("Assets/Models/Person.gltf");

	glEnable(GL_DEPTH_TEST);

	return true;
}

void ModelViewer::Shutdown()
{

}

void ModelViewer::Update( float delta )
{
	camera->Update(delta);
}

void ModelViewer::Draw()
{
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Bind();

	glm::vec3 lightPos = { 0.0f, 0.0f, 3.0f };
	glm::vec3 viewPos = { 0.0f, 0.0f, 3.0f };
	glm::vec3 lightColour = { 1.0f, 1.0f, 1.0f };

	shader.SetVec("lightPos", lightPos);
	shader.SetVec("viewPos", viewPos);
	shader.SetVec("lightColour", lightColour);

	shader.SetVec("material.ambient", { 0.2f, 0.2f, 0.2f });
	shader.SetVec("material.diffuse", { 0.8f, 0.2f, 0.8f });
	shader.SetVec("material.specular", { 0.0f, 0.0f, 0.0f });
	shader.SetFloat("material.shininess", 10);

	shader.SetMat("projection", camera->GetProjectionMatrix());
	shader.SetMat("view", camera->GetViewMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	shader.SetMat("model", model);

	testModel->Draw(shader);
}

void ModelViewer::ImGuiDraw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


}

void ModelViewer::OnEvent( Event& e )
{
	Application::OnEvent( e );
	camera->OnEvent(e);

	EventDispatcher dispatcher( e );
	dispatcher.Dispatch<KeyPressedEvent>( BIND_EVENT_FN( ModelViewer::OnKeyPressed ) );
	dispatcher.Dispatch<MouseButtonPressedEvent>( BIND_EVENT_FN( ModelViewer::OnMouseButtonPressed ) );
}

bool ModelViewer::OnKeyPressed( KeyPressedEvent& e )
{
	return false;
}

bool ModelViewer::OnMouseButtonPressed( MouseButtonPressedEvent& e )
{
	return false;
}
