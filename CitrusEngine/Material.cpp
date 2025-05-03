#include "Material.h"

#include "Shader.h"

#include <glad/glad.h>

Material::Material( Shader& shader )
	: shader(shader)
{
	
}

Material::~Material()
{

}

void Material::SetBool( const std::string& name, bool value )
{
	materialProperties[name].value = value;
}

void Material::SetInt( const std::string& name, int value )
{
	materialProperties[name].value = value;
}

void Material::SetFloat( const std::string& name, float value )
{
	materialProperties[name].value = value;
}

void Material::SetVec( const std::string& name, glm::vec2& value )
{
	materialProperties[name].value = value;
}

void Material::SetVec( const std::string& name, glm::vec3& value )
{
	materialProperties[name].value = value;
}

void Material::SetVec( const std::string& name, glm::vec4& value )
{
	materialProperties[name].value = value;
}

void Material::Apply()
{
	shader.Bind();

	for (const auto& [name, prop] : materialProperties)
	{
		if (std::holds_alternative<int>(prop.value))
		{
			shader.SetInt(prop.name, std::get<int>(prop.value));
		}
		else if (std::holds_alternative<float>(prop.value))
		{
			shader.SetFloat(prop.name, std::get<float>(prop.value));
		}
		else if (std::holds_alternative<bool>(prop.value))
		{
			shader.SetBool(prop.name, std::get<bool>(prop.value));
		}
		else if (std::holds_alternative<glm::vec2>(prop.value))
		{
			shader.SetVec(prop.name, std::get<glm::vec2>(prop.value));
		}
		else if (std::holds_alternative<glm::vec3>(prop.value))
		{
			shader.SetVec(prop.name, std::get<glm::vec3>(prop.value));
		}
		else if (std::holds_alternative<glm::vec4>(prop.value))
		{
			shader.SetVec(prop.name, std::get<glm::vec4>(prop.value));
		}
		else if (std::holds_alternative<Texture>(prop.value))
		{
			auto thing = std::get<Texture>(prop.value);
			int textureUnit = 0;
			switch (thing.GetTextureType())
			{
			case TextureType::DIFFUSE:
				textureUnit = 0;
				break;
			case TextureType::SPECULAR:
				textureUnit = 1;
				break;
			case TextureType::NORMAL:
				textureUnit = 0;
				break;
			}
			shader.SetInt(prop.name, textureUnit);
		}
	}
}
