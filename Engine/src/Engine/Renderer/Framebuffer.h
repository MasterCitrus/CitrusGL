#pragma once
#include "Engine/Core.h";

namespace CitrusGL
{
	struct FramebufferSpec
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpec& spec);
		~Framebuffer();

		void Invalidate();

		void Bind();
		void Unbind();

		void Resize(uint32_t width, uint32_t height);

		uint32_t GetColourAttachmentID() const { return colourAttachment; }

		const FramebufferSpec& GetFramebufferSpec() const { return spec; }

	private:
		uint32_t framebufferID = 0;
		uint32_t colourAttachment = 0, depthAttachment = 0;
		FramebufferSpec spec;
	};
}