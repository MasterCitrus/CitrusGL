#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <vector>

enum ShaderType : unsigned int
{
	NONE = 0,
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	TESS_EVAL,
	TESS_CONTROL,

	ShaderType_COUNT
};

enum UniformType
{
	UNDEFINED = 0,
	FLOAT,
	FLOAT_VEC2,
	FLOAT_VEC3,
	FLOAT_VEC4,
	DOUBLE,
	DOUBLE_VEC2,
	DOUBLE_VEC3,
	DOUBLE_VEC4,
	INT,
	INT_VEC2,
	INT_VEC3,
	INT_VEC4,
	UNSIGNED_INT,
	UNSIGNED_INT_VEC2,
	UNSIGNED_INT_VEC3,
	UNSIGNED_INT_VEC4,
	BOOL,
	BOOL_VEC2,
	BOOL_VEC3,
	BOOL_VEC4,
	FLOAT_MAT2,
	FLOAT_MAT3,
	FLOAT_MAT4,
	FLOAT_MAT2X3,
	FLOAT_MAT2X4,
	FLOAT_MAT3X2,
	FLOAT_MAT3X4,
	FLOAT_MAT4X2,
	FLOAT_MAT4X3,
	SAMPLER_1D,
	SAMPLER_2D,
	SAMPLER_3D,
	SAMPLER_CUBE,
	SAMPLER_1D_SHADOW,
	SAMPLER_2D_SHADOW,
	SAMPLER_1D_ARRAY,
	SAMPLER_2D_AARAY,
	SAMPLER_1D_ARRAY_SHADOW,
	SAMPLER_2D_ARRAY_SHADOW,
	SAMPLER_2D_MULTISAMPLE,
	SAMPLER_2D_MULTISAMPLE_ARRAY,
	SAMPLER_CUBE_SHADOW,
	SAMPLER_BUFFER,
	SAMPLER_2D_RECT,
	SAMPLER_2D_RECT_SHADOW,
	INT_SAMPLER_1D,
	INT_SAMPLER_2D,
	INT_SAMPLER_3D,
	INT_SAMPLER_CUBE,
	INT_SAMPLER_1D_ARRAY,
	INT_SAMPLER_2D_AARAY,
	INT_SAMPLER_2D_MULTISAMPLE,
	INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
	INT_SAMPLER_BUFFER,
	INT_SAMPLER_2D_RECT,
	UNSIGNED_INT_SAMPLER_1D,
	UNSIGNED_INT_SAMPLER_2D,
	UNSIGNED_INT_SAMPLER_3D,
	UNSIGNED_INT_SAMPLER_CUBE,
	UNSIGNED_INT_SAMPLER_1D_ARRAY,
	UNSIGNED_INT_SAMPLER_2D_AARAY,
	UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,
	UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
	UNSIGNED_INT_SAMPLER_BUFFER,
	UNSIGNED_INT_SAMPLER_2D_RECT,
	IMAGE_1D,
	IMAGE_2D,
	IMAGE_3D,
	IMAGE_2D_RECT,
	IMAGE_CUBE,
	IMAGE_BUFFER,
	IMAGE_1D_ARRAY,
	IMAGE_2D_ARRAY,
	IMAGE_2D_MULTISAMPLE,
	IMAGE_2D_MULTISAMPLE_ARRAY,
	INT_IMAGE_1D,
	INT_IMAGE_2D,
	INT_IMAGE_3D,
	INT_IMAGE_2D_RECT,
	INT_IMAGE_CUBE,
	INT_IMAGE_BUFFER,
	INT_IMAGE_1D_ARRAY,
	INT_IMAGE_2D_ARRAY,
	INT_IMAGE_2D_MULTISAMPLE,
	INT_IMAGE_2D_MULTISAMPLE_ARRAY,
	UNSIGNED_INT_IMAGE_1D,
	UNSIGNED_INT_IMAGE_2D,
	UNSIGNED_INT_IMAGE_3D,
	UNSIGNED_INT_IMAGE_2D_RECT,
	UNSIGNED_INT_IMAGE_CUBE,
	UNSIGNED_INT_IMAGE_BUFFER,
	UNSIGNED_INT_IMAGE_1D_ARRAY,
	UNSIGNED_INT_IMAGE_2D_ARRAY,
	UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,
	UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
	UNSIGNED_INT_ATOMIC_COUNTER
};

struct UniformData
{
	std::string name;
	UniformType type;
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

	std::vector<UniformData> GetAllUniforms() const;

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
	std::string lastError;
	std::shared_ptr<ShaderCode> shaderCodes[ShaderType::ShaderType_COUNT];
	unsigned int programID;
};

