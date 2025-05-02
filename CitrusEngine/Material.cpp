#include "Material.h"

#include "Shader.h"

#include <glad/glad.h>

Material::Material( Shader* shader )
	: shader(shader)
{

}

Material::~Material()
{

}

void Material::SetBool( const std::string& name, bool value )
{
	boolParams[name] = value;
}

void Material::SetInt( const std::string& name, int value )
{
	intParams[name] = value;
}

void Material::SetFloat( const std::string& name, float value )
{
	floatParams[name] = value;
}

void Material::SetVec( const std::string& name, glm::vec2& value )
{
	vec2Params[name] = value;
}

void Material::SetVec( const std::string& name, glm::vec3& value )
{
	vec3Params[name] = value;
}

void Material::SetVec( const std::string& name, glm::vec4& value )
{
	vec4Params[name] = value;
}

void Material::Apply()
{
	shader->Bind();

	for (const auto& [name, value] : floatParams)
	{
		shader->SetFloat( name, value );
	}

	for (const auto& [name, value] : intParams)
	{
		shader->SetInt( name, value );
	}

	for (const auto& [name, value] : boolParams)
	{
		shader->SetBool( name, value );
	}

	for (const auto& [name, value] : vec2Params)
	{
		shader->SetVec( name, value );
	}

	for (const auto& [name, value] : vec3Params)
	{
		shader->SetVec( name, value );
	}

	for (auto const& [name, value] : vec4Params)
	{
		shader->SetVec( name, value );
	}

	for (auto const& [name, value] : textureParams)
	{
		glActiveTexture( GL_TEXTURE0 + value.textureUnit );
		glBindTexture( GL_TEXTURE_2D, value.textureID );
		shader->SetInt( name, value.textureUnit );
	}
}
