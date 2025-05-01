#include "Application.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <chrono>

Application::Application()
{

}

Application::~Application()
{

}

void Application::Run(const std::string& title, unsigned int width, unsigned int height, bool fullscreen)
{
	WindowProperties props( title, width, height, fullscreen );
	window = new Window( props );
	window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	if (window && Initialise())
	{
		auto window = (GLFWwindow*)GetWindow().GetNativeWindow();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL( window, true );
		ImGui_ImplOpenGL3_Init( "#version 460" );

		std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
		std::chrono::time_point<std::chrono::high_resolution_clock> currTime;

		double deltaTime = 0;
		unsigned int frames = 0;
		double fpsInterval = 0;

		while (running)
		{
			currTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime);

			deltaTime = duration.count();

			if (glfwGetWindowAttrib( window, GLFW_ICONIFIED ) != 0) continue;

			frames++;
			fpsInterval += deltaTime;
			if (fpsInterval >= 1.0f)
			{
				fps = frames;
				frames = 0;
				fpsInterval -= 1.0f;
			}

			Update( (float)deltaTime );

			Draw();

			ImGuiDraw();

			if (showDemoWindow)
			{
				ImGui::ShowDemoWindow();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = window;
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent( backup_current_context );
			}

			this->window->Update();
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	Shutdown();
}

void Application::OnEvent( Event& e )
{
	EventDispatcher dispatcher( e );
	dispatcher.Dispatch<WindowCloseEvent>( BIND_EVENT_FN( Application::OnWindowClose ) );
}

bool Application::OnWindowClose( WindowCloseEvent& e )
{
	running = false;
	return true;
}
