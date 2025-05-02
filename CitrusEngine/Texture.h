#pragma once

#include <string>

enum TextureFormat : unsigned int
{
	RED = 0,
	RG,
	RGB,
	RGBA
};

enum TextureType : unsigned int
{
	NONE = 0,
	DIFFUSE,
	SPECULAR,
	NORMAL,
	EMISSION,
	HEIGHT,
	ALBEDO,
	ROUGHNESS,
	METALLIC,
	OCCULUSION,
	TEXTURETYPE_Count
};

class Texture
{
public:
	Texture();
	Texture( const std::string& filePath, TextureType type );
	Texture( unsigned int width, unsigned int height, TextureType type, TextureFormat format, unsigned char* pixels = nullptr );
	~Texture();

	bool Load( const std::string& filePath, TextureType type );
	bool Create( unsigned int width, unsigned int height, TextureType type, TextureFormat format, unsigned char* pixels = nullptr );

	void Bind(unsigned int slot);
	void Unbind();

	std::string GetFilePath() const { return path; }
	TextureType GetTextureType() const { return type; }
	TextureFormat GetTextureFormat() const { return format; }
	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }
	unsigned char* GetPixels() const { return pixels; }

private:
	std::string path;
	TextureType type;
	TextureFormat format;
	unsigned int textureID;
	unsigned int width;
	unsigned int height;
	unsigned char* pixels;
};

