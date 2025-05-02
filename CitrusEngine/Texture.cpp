#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
	: textureID(0), width(0), height(0), path("none"), type(TextureType::NONE), pixels(nullptr)
{
}

Texture::Texture( const std::string& filePath, TextureType type )
	: textureID( 0 ), width( 0 ), height( 0 ), path( "none" ), type( type), pixels( nullptr )
{
	Load( filePath, type );
}

Texture::Texture( unsigned int width, unsigned int height, TextureType type, TextureFormat format, unsigned char* pixels )
	: textureID( 0 ), path( "none" ) , type(type)
{
	Create( width, height, type, pixels );
}

Texture::~Texture()
{
	if (textureID != 0)
	{
		glDeleteTextures( 1, &textureID );
	}
	if (pixels)
	{
		stbi_image_free( pixels );
	}
}

bool Texture::Load( const std::string& filePath, TextureType type )
{
	if (textureID != 0)
	{
		glDeleteTextures( 1, &textureID );
		textureID = 0;
		width = 0;
		height = 0;
		type = TextureType::NONE;
		if (pixels)
		{
			stbi_image_free( pixels );
			pixels = nullptr;
		}
	}

	int width, height, channels;
	pixels = stbi_load( filePath.c_str(), &width, &height, &channels, STBI_default );

	if (pixels)
	{
		glGenTextures( 1, &textureID );
		glBindTexture( GL_TEXTURE_2D, textureID );

		switch (channels)
		{
		case STBI_grey:
			format = RED;
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels );
			break;
		case STBI_grey_alpha:
			format = RG;
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels );
			break;
		case STBI_rgb:
			format = RGB;
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );
			break;
		case STBI_rgb_alpha:
			format = RGBA;
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
			break;
		default:
			return false;
		}

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, 0 );

		this->width = (unsigned int)width;
		this->height = (unsigned int)height;
		this->type = type;
		path = filePath;
		return true;
	}
}

bool Texture::Create( unsigned int width, unsigned int height, TextureType type, TextureFormat format, unsigned char* pixels )
{
	if (textureID != 0)
	{
		glDeleteTextures( 1, &textureID );
		textureID = 0;
		width = 0;
		height = 0;
		type = TextureType::NONE;
		if (pixels)
		{
			delete pixels;
			pixels = nullptr;
		}
	}

	this->width = width;
	this->height = height;
	this->type = type;
	this->format = format;
	this->pixels = pixels;

	glGenTextures( 1, &textureID );
	glBindTexture( GL_TEXTURE_2D, textureID );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTExParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	switch (format)
	{
	case RED:
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels );
		break;
	case RG:
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels );
		break;
	case RGB:
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );
		break;
	case RGBA:
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
		break;
	default:
		return false;
	}

	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture::Bind( unsigned int slot )
{
	glActiveTexture( GL_TEXTURE0 + slot );
	glBindTexture( GL_TEXTURE_2D, textureID );
}

void Texture::Unbind()
{
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}
