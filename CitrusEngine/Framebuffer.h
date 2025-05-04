#pragma once

struct FSpec
{
	FSpec() = default;
	FSpec(unsigned int width, unsigned int height)
		: width(width), height(height)
	{
	}

	unsigned int width = 0;
	unsigned int height = 0;
};

class Framebuffer
{
public:
	Framebuffer(unsigned int width, unsigned int height);
	~Framebuffer();

	void Resize(unsigned int width, unsigned int height);

	void Bind() const;
	void Unbind() const;

	unsigned int GetColourTexture() const { return colourTexture; }
	unsigned int GetDepthTexture() const { return depthTexture; }
	unsigned int GetFramebufferID() const { return framebufferID; }
	FSpec& GetSpec() { return spec; }

private:
	unsigned int framebufferID;
	unsigned int renderbufferID;
	unsigned int colourTexture;
	unsigned int depthTexture;
	FSpec spec;
};

