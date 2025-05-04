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
	MaterialProperty prop;
	prop.name = name;
	prop.value = value;
	materialProperties[name] = prop;
}

void Material::SetInt( const std::string& name, int value )
{
	MaterialProperty prop;
	prop.name = name;
	prop.value = value;
	materialProperties[name] = prop;
}

void Material::SetFloat( const std::string& name, float value )
{
	MaterialProperty prop;
	prop.name = name;
	prop.value = value;
	materialProperties[name] = prop;
}

void Material::SetTexture(const std::string& name, Texture* value)
{
	MaterialProperty prop;
	prop.name = name;
	prop.value = value;
	materialProperties[name] = prop;
}

void Material::SetVec( const std::string& name, const glm::vec2& value )
{
	MaterialProperty prop;
	prop.name = name;
	prop.value = value;
	materialProperties[name] = prop;
}

void Material::SetVec( const std::string& name, const glm::vec3& value, bool isColour )
{
	MaterialProperty prop;
	prop.name = name;
	prop.value = value;
	prop.isColour = isColour;
	materialProperties[name] = prop;
}

void Material::SetVec( const std::string& name, const glm::vec4& value, bool isColour )
{
	MaterialProperty prop;
	prop.name = name;
	prop.value = value;
	prop.isColour = isColour;
	materialProperties[name] = prop;
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
		else if (std::holds_alternative<Texture*>(prop.value))
		{
			auto tex = std::get<Texture*>(prop.value);
			int textureUnit = 0;
			switch (tex->GetTextureType())
			{
			case TextureType::DIFFUSE:
				textureUnit = 0;
				break;
			case TextureType::SPECULAR:
				textureUnit = 1;
				break;
			case TextureType::NORMAL:
				textureUnit = 2;
				break;
			}
			shader.SetInt(prop.name, textureUnit);
			tex->Bind(textureUnit);
		}
	}
}
