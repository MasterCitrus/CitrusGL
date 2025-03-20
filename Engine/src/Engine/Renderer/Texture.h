#pragma once

#include <string>

#include <glad/glad.h>

namespace CitrusGL
{
	class Texture
	{
	public:
		enum Format : uint32_t
		{
			RED = 1,
			RG,
			RGB,
			RGBA
		};

		Texture();
		Texture(const std::string& path);
		Texture(uint32_t width, uint32_t height, Format format, unsigned char* pixels = nullptr);
		~Texture();

		bool Load(const std::string& path);
		void Create(uint32_t width, uint32_t height, Format format, unsigned char* pixels = nullptr);

		uint32_t GetWidth() const { return width; }
		uint32_t GetHeight() const { return height; }
		uint32_t GetTextureID() const { return textureID; }

		void Bind(uint32_t slot = 0) const;

	private:
		std::string path;
		uint32_t width, height;
		uint32_t textureID;
		uint32_t format;
		unsigned char* pixels;
	};
}