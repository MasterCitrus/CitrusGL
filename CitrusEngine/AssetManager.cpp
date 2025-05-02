#include "AssetManager.h"

#include <filesystem>
#include <stdexcept>

void AssetManager::LoadShaders()
{
}

Shader& AssetManager::CreateShader(const std::string& name, const std::string& path)
{
    if (shaders.find(name) != shaders.end())
    {
        return shaders[name];
    }

    std::string shaderName;
    std::string directory;
    std::size_t begin = path.find_last_of("/\\");
    std::size_t end = path.find_last_of('.');
    shaderName = path.substr(begin + 1, end - 1 - begin);
    directory = path.substr(0, begin);

    Shader shader;
    std::string filename;
    for (const auto& file : std::filesystem::directory_iterator(directory))
    {
        std::string temp = file.path().string();
        std::string extension = temp.substr(end);
        filename = temp.substr(begin + 1, end - 1 - begin);
        if (filename == shaderName)
        {
            if (extension == ".vs")
            {
                std::shared_ptr<ShaderCode> code = make_shared<ShaderCode>(ShaderType::VERTEX, temp);
                shader.AttachShaderCode(code);
            }
            else if (extension == ".fs")
            {
                std::shared_ptr<ShaderCode> code = make_shared<ShaderCode>(ShaderType::FRAGMENT, temp);
                shader.AttachShaderCode(code);
            }
        }
    }

    shaders[shaderName] = shader;

    return shaders[shaderName];
}

Shader& AssetManager::GetShader(const std::string& name)
{
    if (shaders.find(name) == shaders.end())
    {
        throw std::runtime_error("Shader not found: " + name);
    }

    return shaders[name];
}
