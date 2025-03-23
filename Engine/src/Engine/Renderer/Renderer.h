#pragma once


namespace CitrusGL
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();

		static void Submit();

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};
	};
}