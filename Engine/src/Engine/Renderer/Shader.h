#pragma once


namespace CitrusGL
{
	class Shader
	{
	public:
		Shader(const char* vertex, const char* fragment,
			   const char* geometry = nullptr, const char* tessControl = nullptr,
			   const char* tessEval = nullptr, bool isFile = false);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetInt2(const std::string& name, int val1, int val2) const;
		void SetInt3(const std::string& name, int val1, int val2, int val3) const;
		void SetInt4(const std::string& name, int val1, int val2, int val3, int val4) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec2(const std::string& name, const glm::vec2& value) const;
		void SetVec2(const std::string& name, float x, float y) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec3(const std::string& name, float x, float y, float z) const;
		void SetVec4(const std::string& name, const glm::vec4& value) const;
		void SetVec4(const std::string& name, float x, float y, float z, float w) const;
		void SetMat2(const std::string& name, const glm::mat2& value) const;
		void SetMat3(const std::string& name, const glm::mat3& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	private:
		void Load(const char* vertex, const char* fragment,
			 const char* geometry, const char* tessControl,
			 const char* tessEval);
		void Create(const char* vertex, const char* fragment,
					const char* geometry, const char* tessControl,
					const char* tessEval);

	private:
		uint32_t shaderID;
	};
}