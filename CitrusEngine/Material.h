#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

#include <map>
#include <variant>

class Shader;

struct MaterialProperty
{
	std::string name;
	std::variant<bool, float, int, glm::vec2, glm::vec3, glm::vec4, Texture*> value;
	bool isColour;
};

class Material
{
public:
	Material( Shader& shader );
	~Material();

	void SetBool( const std::string& name, bool value );
	void SetInt( const std::string& name, int value );
	void SetFloat( const std::string& name, float value );
	void SetTexture(const std::string& name, Texture* value);
	void SetVec( const std::string& name, const glm::vec2& value );
	void SetVec( const std::string& name, const glm::vec3& value, bool isColour );
	void SetVec( const std::string& name, const glm::vec4& value, bool isColour );

	std::map<std::string, MaterialProperty>& GetMaterialProperties() { return materialProperties; }

	void Apply();

	Shader& GetShader() const { return shader; }

private:
	std::map<std::string, MaterialProperty> materialProperties;
	Shader& shader;
};

