#pragma once

extern CitrusGL::Application* CitrusGL::CreateApplication();

int main(int argc, char** argv)
{
	CitrusGL::Log::Init();
	CGL_CORE_WARN("Log Initialised");

	auto app = CitrusGL::CreateApplication();
	app->Run();
	delete app;
}