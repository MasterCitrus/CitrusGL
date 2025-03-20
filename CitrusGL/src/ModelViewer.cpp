#include <CitrusGL.h>
#include <imgui/imgui.h>

class ExampleLayer : public CitrusGL::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate()
	{

	}

	void OnEvent(CitrusGL::Event& e)
	{

	}

	void OnImGuiRender()
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}
};

class ModelViewer : public CitrusGL::Application
{
public:
	ModelViewer()
	{
		PushLayer(new ExampleLayer());
	}

	~ModelViewer()
	{

	}

};

CitrusGL::Application* CitrusGL::CreateApplication()
{
	return new ModelViewer();
}