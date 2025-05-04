#pragma once

#include "Material.h"
#include "Texture.h"
#include "Vertex.h"

#include <string>
#include <vector>

class Material;
class Shader;

class Mesh
{
public:
	Mesh( const std::string& name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material );

	void Draw();

	void SetMaterial(Material* material) { this->material = material; }

	std::vector<Vertex>& GetVertices() { return vertices; }
	Material* GetMaterial() { return material; }
	std::string GetName() const { return name; }

private:
	void Initialise();

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::string name;
	Material* material;
	unsigned int VAO, VBO, IBO;
};

