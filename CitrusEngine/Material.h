#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

#include <map>

class Shader;

struct TextureParam
{
	int textureID;
	int textureUnit;
};

typedef std::map<std::string, float> FloatParams;
typedef std::map<std::string, int> IntParams;
typedef std::map<std::string, bool> BoolParams;
typedef std::map<std::string, glm::vec2> Vec2Params;
typedef std::map<std::string, glm::vec3> Vec3Params;
typedef std::map<std::string, glm::vec4> Vec4Params;
typedef std::map<std::string, TextureParam> TexParams;

class Material
{
public:
	Material( Shader& shader );
	~Material();

	void SetBool( const std::string& name, bool value );
	void SetInt( const std::string& name, int value );
	void SetFloat( const std::string& name, float value );
	void SetVec( const std::string& name, glm::vec2& value );
	void SetVec( const std::string& name, glm::vec3& value );
	void SetVec( const std::string& name, glm::vec4& value );

	void Apply();

	Shader& GetShader() const { return shader; }

private:
	FloatParams floatParams;
	IntParams intParams;
	BoolParams boolParams;
	Vec2Params vec2Params;
	Vec3Params vec3Params;
	Vec4Params vec4Params;
	TexParams textureParams;
	Shader& shader;
};

