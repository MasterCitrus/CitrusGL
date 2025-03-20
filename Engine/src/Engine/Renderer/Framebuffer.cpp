#include "cglpch.h"
#include "Framebuffer.h"

#include <glad/glad.h>

namespace CitrusGL
{
	Framebuffer::Framebuffer(const FramebufferSpec& spec) : spec(spec)
	{
		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &framebufferID);
		glDeleteTextures(1, &colourAttachment);
		glDeleteTextures(1, &depthAttachment);
	}

	void Framebuffer::Invalidate()
	{
		if (framebufferID)
		{
			glDeleteFramebuffers(1, &framebufferID);
			glDeleteTextures(1, &colourAttachment);
			glDeleteTextures(1, &depthAttachment);
		}

		glCreateFramebuffers(1, &framebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

		glCreateTextures(GL_TEXTURE_2D, 1, &colourAttachment);
		glBindTexture(1, colourAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(1, depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec.width, spec.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		CGL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glViewport(0, 0, spec.width, spec.height);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		spec.width = width;
		spec.height = height;

		Invalidate();
	}
}