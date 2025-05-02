#include "Shader.h"
#include "Utils.h"

#include <glad/glad.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

ShaderCode::ShaderCode()
	: shaderID(0), type(ShaderType::UNKNOWN), lastError(std::string())
{
}

ShaderCode::ShaderCode( ShaderType type, const std::string& filePath )
{
	LoadShader( type, filePath );
}

ShaderCode::~ShaderCode()
{
	glDeleteShader( shaderID );
}

bool ShaderCode::LoadShader( ShaderType type, const std::string& filePath )
{
	assert( type > 0 && type < ShaderType::ShaderType_COUNT );

	this->type = type;

	switch (type)
	{
	case VERTEX:
		shaderID = glCreateShader( GL_VERTEX_SHADER );
		break;
	case FRAGMENT:
		shaderID = glCreateShader( GL_FRAGMENT_SHADER );
		break;
	case GEOMETRY:
		shaderID = glCreateShader( GL_GEOMETRY_SHADER );
		break;
	case TESS_EVAL:
		shaderID = glCreateShader( GL_TESS_EVALUATION_SHADER );
		break;
	case TESS_CONTROL:
		shaderID = glCreateShader( GL_TESS_CONTROL_SHADER );
		break;
	default:
		return false;
	}

	std::fstream shaderFile;
	shaderFile.exceptions( std::ifstream::badbit | std::ifstream::failbit );
	
	shaderFile.open( filePath );

	if (!shaderFile.is_open())
	{
		std::cout << "File could not be opened\n";
		return false;
	}

	std::stringstream shader;
	std::string shaderSrc;
	try
	{
		shader << shaderFile.rdbuf();
		shaderFile.close();

		shaderSrc = shader.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "File failed to read\n";
		return false;
	}

	const char* temp = shaderSrc.c_str();
	int success;

	glShaderSource( shaderID, 1, &temp, NULL );
	glCompileShader( shaderID );

	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog( shaderID, 512, NULL, infoLog );
		std::cout << "Shader Compilation Failed\n" << infoLog << '\n';
		return false;
	}

	return true;
}

bool ShaderCode::CreateShader( ShaderType type, const std::string& src )
{
	assert( type > 0 && type < ShaderType::ShaderType_COUNT );

	this->type = type;

	switch (type)
	{
	case VERTEX:
		shaderID = glCreateShader( GL_VERTEX_SHADER );
	case FRAGMENT:
		shaderID = glCreateShader( GL_FRAGMENT_SHADER );
	case GEOMETRY:
		shaderID = glCreateShader( GL_GEOMETRY_SHADER );
	case TESS_EVAL:
		shaderID = glCreateShader( GL_TESS_EVALUATION_SHADER );
	case TESS_CONTROL:
		shaderID = glCreateShader( GL_TESS_CONTROL_SHADER );
	default:
		return false;
	}

	const char* temp = src.c_str();
	int success;

	glShaderSource( shaderID, 1, &temp, NULL );
	glCompileShader( shaderID );

	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog( shaderID, 512, NULL, infoLog );
		std::cout << "Shader Compilation Failed\n" << infoLog << '\n';
		return false;
	}

	return true;
}

Shader::Shader()
	: programID(0), lastError("")
{
	shaderCodes[0] = shaderCodes[1] = shaderCodes[2] = shaderCodes[3] = shaderCodes[4] = 0;
}

Shader::~Shader()
{
}

bool Shader::Link()
{
	programID = glCreateProgram();

	for (auto& s : shaderCodes)
	{
		if (s != nullptr)
		{
			glAttachShader( programID, s->GetShaderID() );
		}
	}

	glLinkProgram( programID );

	int success = GL_TRUE;
	glGetProgramiv( programID, GL_LINK_STATUS, &success );
	if (success == GL_FALSE)
	{
		int infoLength = 0;
		glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLength );

		char* temp = new char[infoLength + 1];
		glGetProgramInfoLog( programID, infoLength, 0, temp );
		lastError = temp;
		delete[] temp;
		return false;
	}

	return true;
}

void Shader::Bind() const
{
	assert( programID > 0 && "Invalid Shader Program" );
	glUseProgram( programID );
}

void Shader::AttachShaderCode( std::shared_ptr<ShaderCode> shaderCode )
{
	assert( shaderCode != nullptr );

	shaderCodes[shaderCode->GetShaderType()] = shaderCode;
}

int Shader::GetUniform( const std::string& name ) const
{
	return glGetUniformLocation( programID, name.c_str() );
}

std::vector<UniformData> Shader::GetAllUniforms() const
{
	std::vector<UniformData> uniforms;

	int count = 0;

	glGetProgramiv( programID, GL_ACTIVE_UNIFORMS, &count );

	const int bufsize = 256;
	char name[bufsize];
	int length;
	int size;
	unsigned int type;
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(programID, i, bufsize, &length, &size, &type, name);
		UniformData data;
		data.name = name;
		data.type = GetUniformType(type);

		uniforms.push_back(data);
	}

	return uniforms;
}

//Uniforms from name
void Shader::SetBool( const std::string& name, bool value )
{
	glUniform1i( glGetUniformLocation( programID, name.c_str()), (int)value );
}

void Shader::SetInt( const std::string& name, int value )
{
	glUniform1i( glGetUniformLocation( programID, name.c_str() ), value );
}

void Shader::SetInt( const std::string& name, int v1, int v2 )
{
	glUniform2i( glGetUniformLocation( programID, name.c_str() ), v1, v2 );
}

void Shader::SetInt( const std::string& name, int v1, int v2, int v3 )
{
	glUniform3i( glGetUniformLocation( programID, name.c_str() ), v1, v2, v3 );
}

void Shader::SetInt( const std::string& name, int v1, int v2, int v3, int v4 )
{
	glUniform4i( glGetUniformLocation( programID, name.c_str() ), v1, v2, v3, v4 );
}

void Shader::SetFloat( const std::string& name, float value )
{
	glUniform1f( glGetUniformLocation( programID, name.c_str() ), value );
}

void Shader::SetFloat( const std::string& name, float v1, float v2 )
{
	glUniform2f( glGetUniformLocation( programID, name.c_str() ), v1, v2 );
}

void Shader::SetFloat( const std::string& name, float v1, float v2, float v3 )
{
	glUniform3f( glGetUniformLocation( programID, name.c_str() ), v1, v2, v3 );
}

void Shader::SetFloat( const std::string& name, float v1, float v2, float v3, float v4 )
{
	glUniform4f( glGetUniformLocation( programID, name.c_str() ), v1, v2, v3, v4 );
}

void Shader::SetVec( const std::string& name, const glm::vec2& value )
{
	glUniform2fv( glGetUniformLocation( programID, name.c_str() ), 1, &value[0] );
}

void Shader::SetVec( const std::string& name, const glm::vec3& value )
{
	glUniform3fv( glGetUniformLocation( programID, name.c_str() ), 1, &value[0] );
}

void Shader::SetVec( const std::string& name, const glm::vec4& value )
{
	glUniform4fv( glGetUniformLocation( programID, name.c_str() ), 1, &value[0] );
}

void Shader::SetMat( const std::string& name, const glm::mat2& value )
{
	glUniformMatrix2fv( glGetUniformLocation( programID, name.c_str() ), 1, GL_FALSE, &value[0][0] );
}

void Shader::SetMat( const std::string& name, const glm::mat3& value )
{
	glUniformMatrix3fv( glGetUniformLocation( programID, name.c_str() ), 1, GL_FALSE, &value[0][0] );
}

void Shader::SetMat( const std::string& name, const glm::mat4& value )
{
	glUniformMatrix4fv( glGetUniformLocation( programID, name.c_str() ), 1, GL_FALSE, &value[0][0] );
}

//Uniforms from ID
void Shader::SetBool( int ID, bool value )
{
	glUniform1i( ID, (int)value );
}

void Shader::SetInt( int ID, int value )
{
	glUniform1i( ID, value );
}

void Shader::SetInt( int ID, int v1, int v2 )
{
	glUniform2i( ID, v1, v2 );
}

void Shader::SetInt( int ID, int v1, int v2, int v3 )
{
	glUniform3i( ID, v1, v2, v3 );
}

void Shader::SetInt( int ID, int v1, int v2, int v3, int v4 )
{
	glUniform4i( ID, v1, v2, v3, v4 );
}

void Shader::SetFloat( int ID, float value )
{
	glUniform1f( ID, value );
}

void Shader::SetFloat( int ID, float v1, float v2 )
{
	glUniform2f( ID, v1, v2 );
}

void Shader::SetFloat( int ID, float v1, float v2, float v3 )
{
	glUniform3f( ID, v1, v2, v3 );
}

void Shader::SetFloat( int ID, float v1, float v2, float v3, float v4 )
{
	glUniform4f( ID, v1, v2, v3, v4 );
}

void Shader::SetVec( int ID, const glm::vec2& value )
{
	glUniform2fv( ID, 1, &value[0] );
}

void Shader::SetVec( int ID, const glm::vec3& value )
{
	glUniform3fv( ID, 1, &value[0] );
}

void Shader::SetVec( int ID, const glm::vec4& value )
{
	glUniform4fv( ID, 1, &value[0] );
}

void Shader::SetMat( int ID, const glm::mat2& value )
{
	glUniformMatrix2fv( ID, 1, GL_FALSE, &value[0][0] );
}

void Shader::SetMat( int ID, const glm::mat3& value )
{
	glUniformMatrix3fv( ID, 1, GL_FALSE, &value[0][0] );
}

void Shader::SetMat( int ID, const glm::mat4& value )
{
	glUniformMatrix4fv( ID, 1, GL_FALSE, &value[0][0] );
}