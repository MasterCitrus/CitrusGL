#include "cglpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace CitrusGL
{
	Shader::Shader(const char* vertex, const char* fragment,
				   const char* geometry, const char* tessControl,
				   const char* tessEval, bool isFile)
	{
		if (!isFile) Create(vertex, fragment, geometry, tessControl, tessEval);
		else Load(vertex, fragment, geometry, tessControl, tessEval);
	}

	Shader::~Shader()
	{
		glDeleteProgram(shaderID);
	}

	void Shader::Bind() const
	{
		glUseProgram(shaderID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetBool(const std::string& name, bool value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetInt2(const std::string& name, int val1, int val2) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform2i(location, val1, val2);
	}

	void Shader::SetInt3(const std::string& name, int val1, int val2, int val3) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform3i(location, val1, val2, val3);
	}

	void Shader::SetInt4(const std::string& name, int val1, int val2, int val3, int val4) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform4i(location, val1, val2, val3, val4);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform2fv(location, 1, &value[0]);
	}

	void Shader::SetVec2(const std::string& name, float x, float y) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform2f(location, x, y);
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform3fv(location, 1, &value[0]);
	}

	void Shader::SetVec3(const std::string& name, float x, float y, float z) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform3f(location, x, y, z);
	}

	void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform4fv(location, 1, &value[0]);
	}

	void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniform4f(location, x, y, z, w);
	}

	void Shader::SetMat2(const std::string& name, const glm::mat2& value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		GLint location = glGetUniformLocation(shaderID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}

	void Shader::Load(const char* vertex, const char* fragment, const char* geometry, const char* tessControl, const char* tessEval)
	{

	}

	void Shader::Create(const char* vertex, const char* fragment, const char* geometry, const char* tessControl, const char* tessEval)
	{
		/// Vertex Shader /////////////////////
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertex;
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			CGL_CORE_ERROR("{0}", infoLog.data());
			CGL_CORE_ASSERT(false, "Vertex shader failed compilation!");
			return;
		}

		/// Fragment Shader //////////////

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragment;
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);

			CGL_CORE_ERROR("{0}", infoLog.data());
			CGL_CORE_ASSERT(false, "Fragment shader failed compilation!");
			return;
		}

		/// Geometry Shader //////////////
		
		GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

		if (geometry)
		{
			source = geometry;
			glShaderSource(geometryShader, 1, &source, 0);

			glCompileShader(geometryShader);

			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(geometryShader);

				CGL_CORE_ERROR("{0}", infoLog.data());
				CGL_CORE_ASSERT(false, "Fragment shader failed compilation!");
				return;
			}
		}

		/// Tesselation Control Shader //////////////
		
		GLuint tessConShader = glCreateShader(GL_TESS_CONTROL_SHADER);

		if (tessControl)
		{
			source = tessControl;
			glShaderSource(tessConShader, 1, &source, 0);

			glCompileShader(tessConShader);

			glGetShaderiv(tessConShader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(tessConShader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(tessConShader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(tessConShader);

				CGL_CORE_ERROR("{0}", infoLog.data());
				CGL_CORE_ASSERT(false, "Fragment shader failed compilation!");
				return;
			}
		}

		/// Tesselation Evaluation Shader //////////////
		
		GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);

		if (tessEval)
		{
			source = tessEval;
			glShaderSource(tessEvalShader, 1, &source, 0);

			glCompileShader(tessEvalShader);

			glGetShaderiv(tessEvalShader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(tessEvalShader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(tessEvalShader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(tessEvalShader);

				CGL_CORE_ERROR("{0}", infoLog.data());
				CGL_CORE_ASSERT(false, "Fragment shader failed compilation!");
				return;
			}
		}

		shaderID = glCreateProgram();

		glAttachShader(shaderID, vertexShader);
		glAttachShader(shaderID, fragmentShader);
		if (geometry) glAttachShader(shaderID, geometryShader);
		if (tessControl) glAttachShader(shaderID, tessConShader);
		if (tessEval) glAttachShader(shaderID, tessEvalShader);

		glLinkProgram(shaderID);

		GLint linked = 0;
		glGetProgramiv(shaderID, GL_LINK_STATUS, (int*)&linked);
		if (linked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(shaderID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			if (geometry) glDeleteShader(geometryShader);
			if (tessControl) glDeleteShader(tessConShader);
			if (tessEval) glDeleteShader(tessEvalShader);

			CGL_CORE_ERROR("{0}", infoLog.data());
			CGL_CORE_ASSERT(false, "Shader link failed!");
			return;
		}

		glDetachShader(shaderID, vertexShader);
		glDetachShader(shaderID, fragmentShader);
	}
}