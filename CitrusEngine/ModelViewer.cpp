#include "ModelViewer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

ModelViewer::ModelViewer()
{
}

ModelViewer::~ModelViewer()
{
}

bool ModelViewer::Initialise()
{
	return true;
}

void ModelViewer::Shutdown()
{
}

void ModelViewer::Update( float delta )
{
}

void ModelViewer::Draw()
{
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
