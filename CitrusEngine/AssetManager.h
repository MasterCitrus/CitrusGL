#pragma once
#include "Shader.h"

#include <map>
#include <string>

class AssetManager
{
public:
	AssetManager() = default;

	void LoadShaders();
	Shader& CreateShader(const std::string& name, const std::string& path);
	Shader& GetShader(const std::string& name);

private:
	std::map<std::string, Shader> shaders;
};

