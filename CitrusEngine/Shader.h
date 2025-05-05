#pragma once

#include "UniformData.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <vector>

enum ShaderType : unsigned int
{
	UNKNOWN = 0,
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	TESS_EVAL,
	TESS_CONTROL,

	ShaderType_COUNT
};

class ShaderCode
{
public:
	ShaderCode();
	ShaderCode(ShaderType type, const std::string& filePath);
	~ShaderCode();

	bool LoadShader( ShaderType type, const std::string& filePath );
	bool CreateShader( ShaderType type, const std::string& src );

	ShaderType GetShaderType() const { return type; }
	unsigned int GetShaderID() const { return shaderID; }
	std::string GetLastError() const { return lastError; }

private:
	std::string lastError;
	ShaderType type;
	unsigned int shaderID;
};

class Shader
{
public:
	Shader();
	~Shader();

	bool Link();
	void Bind() const;

	void AttachShaderCode( std::shared_ptr<ShaderCode> shaderCode );

	unsigned int GetProgramID() const { return programID; }
	std::string GetLastError() const { return lastError; }

	int GetUniform( const std::string& name ) const;

	//Setting Uniforms
	//With name
	void SetBool( const std::string& name, bool value );
	void SetInt( const std::string& name, int value );
	void SetInt( const std::string& name, int v1, int v2 );
	void SetInt( const std::string& name, int v1, int v2, int v3 );
	void SetInt( const std::string& name, int v1, int v2, int v3, int v4 );
	void SetFloat( const std::string& name, float value );
	void SetFloat( const std::string& name, float v1, float v2 );
	void SetFloat( const std::string& name, float v1, float v2, float v3 );
	void SetFloat( const std::string& name, float v1, float v2, float v3, float v4 );
	void SetVec( const std::string& name, const glm::vec2& value );
	void SetVec( const std::string& name, const glm::vec3& value );
	void SetVec( const std::string& name, const glm::vec4& value );
	void SetMat( const std::string& name, const glm::mat2& value );
	void SetMat( const std::string& name, const glm::mat3& value );
	void SetMat( const std::string& name, const glm::mat4& value );

	//With ID
	void SetBool( int ID, bool value );
	void SetInt( int ID, int value );
	void SetInt( int ID, int v1, int v2 );
	void SetInt( int ID, int v1, int v2, int v3 );
	void SetInt( int ID, int v1, int v2, int v3, int v4 );
	void SetFloat( int ID, float value );
	void SetFloat( int ID, float v1, float v2 );
	void SetFloat( int ID, float v1, float v2, float v3 );
	void SetFloat( int ID, float v1, float v2, float v3, float v4 );
	void SetVec( int ID, const glm::vec2& value );
	void SetVec( int ID, const glm::vec3& value );
	void SetVec( int ID, const glm::vec4& value );
	void SetMat( int ID, const glm::mat2& value );
	void SetMat( int ID, const glm::mat3& value );
	void SetMat( int ID, const glm::mat4& value );

private:
	std::vector<UniformData> GetAllUniforms() const;

private:
	std::vector<UniformData> uniforms;
	std::string lastError;
	std::shared_ptr<ShaderCode> shaderCodes[ShaderType::ShaderType_COUNT];
	unsigned int programID;
};

