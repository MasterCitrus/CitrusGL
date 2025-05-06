#pragma once

#include <glm/glm.hpp>

#include <map>
#include <string>
#include <variant>

class Shader;
class Texture;

enum class MaterialPropertyDisplayType : unsigned int
{
	UNDEFINED = 0,
	INPUT,
	SLIDER,
	DRAG,
	COLOURPICKER,
	COLOUREDIT
};

struct MaterialProperty
{
	std::string name;
	std::variant<bool, float, int, glm::vec2, glm::vec3, glm::vec4, Texture*> value;
	std::variant<int, float> maxValue = 0;
	std::variant<int, float> minValue = 0;
	MaterialPropertyDisplayType type = MaterialPropertyDisplayType::UNDEFINED;
};

class Material
{
public:
	Material(Shader* shader);
	Material(const Shader* shader);
	Material(Material* material);
	~Material();
	
	void SetBool( const std::string& name, bool value );
	void SetInt( const std::string& name, int value, MaterialPropertyDisplayType type, int minValue = 0, int maxValue = 0 );
	void SetFloat( const std::string& name, float value, MaterialPropertyDisplayType type, float minValue = 0.0f, float maxValue = 0.0f );
	void SetTexture(const std::string& name, Texture* value);
	void SetVec( const std::string& name, const glm::vec2& value, MaterialPropertyDisplayType type, float minValue = 0.0f, float maxValue = 0.0f);
	void SetVec( const std::string& name, const glm::vec3& value, MaterialPropertyDisplayType type, float minValue = 0.0f, float maxValue = 0.0f);
	void SetVec( const std::string& name, const glm::vec4& value, MaterialPropertyDisplayType type, float minValue = 0.0f, float maxValue = 0.0f);

	MaterialProperty& GetProperty(const std::string& name);
	std::map<std::string, MaterialProperty>& GetMaterialProperties() { return materialProperties; }

	void Apply();

	Shader* GetShader() const { return shader; }

private:
	std::map<std::string, MaterialProperty> materialProperties;
	Shader* shader;
};

