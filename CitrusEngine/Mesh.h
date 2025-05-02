#pragma once

#include "Material.h"
#include "Texture.h"
#include "Vertex.h"

#include <vector>

class Material;
class Shader;

class Mesh
{
public:
	Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures );

	void Draw(Shader& shader);

	std::vector<Vertex>& GetVertices() { return vertices; }
	Material* GetMaterial() { return material; }

private:
	void Initialise();

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Material* material;
	unsigned int VAO, VBO, IBO;
};

