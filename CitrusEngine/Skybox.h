#pragma once

#include <string>

class Camera;
class Cubemap;
class Shader;

class Skybox
{
public:
	Skybox() = default;
	Skybox(const std::string& name, Cubemap* cubemap, Shader* shader, Camera* camera);
	~Skybox();

	void Draw();

	std::string GetName() const { return name; }

private:
	std::string name;
	Camera* camera;
	Cubemap* cubemap;
	Shader* shader;

	unsigned int vao, vbo;
};

