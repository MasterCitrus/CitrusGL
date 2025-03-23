#include <CitrusGL.h>
#include <imgui/imgui.h>

#include "Engine/EntryPoint.h"

#include "EditorLayer.h"

namespace CitrusGL
{
	class CitrusEngine : public Application
	{
	public:
		CitrusEngine() : Application("Model Viewer")
		{
			PushLayer(new EditorLayer());
		}

		~CitrusEngine()
		{

		}

	};

	Application* CreateApplication()
	{
		return new CitrusEngine();
	}
}