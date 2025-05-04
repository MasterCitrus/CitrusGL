#pragma once

#include "Animation.h"
#include "BoneInfo.h"
#include "Mesh.h"
#include "Texture.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Shader;

struct aiNode;
struct aiScene;
struct aiMaterial;

class Model
{
public:
	Model( const std::string& path, Shader& shader );
	~Model();

	void Draw();

	std::vector<Mesh>& GetMeshes() { return meshes; }

private:
	void LoadModel( const std::string& path, Shader& shader );

	void ProcessNode(aiNode* node, const aiScene* scene, Shader& shader);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, Shader& shader);

	void SetVertexBoneDataToDefault( Vertex& vertex );
	void SetVertexBoneData( Vertex& vertex, int boneID, float weight );
	void ExtractBoneWeightForVertices( std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene );

	std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture*> loadedTextures;
	std::map<std::string, BoneInfo> boneInfoMap;
	std::string path;
	int boneCount = 0;
};

