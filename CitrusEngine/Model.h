#pragma once

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

class Animation;
class Animator;
class Shader;

struct aiMaterial;
struct aiNode;
struct aiScene;

class Model
{
public:
	Model( const std::string& path, Shader* shader );
	~Model();

	void Update(float delta);
	void Draw();

	std::vector<Mesh*>& GetMeshes() { return meshes; }
	std::vector<Animation*>& GetAnimations() { return animations; }
	std::map<std::string, BoneInfo>& GetBoneInfoMap() { return boneInfoMap; }
	int& GetBoneCount() { return boneCount; }
	Animator* GetAnimator() const { return animator; }

private:
	void LoadModel( const std::string& path, Shader* shader );
	void LoadAnimations(const aiScene* scene);

	void ProcessNode(aiNode* node, const aiScene* scene, Shader* shader);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, Shader* shader);
	Animation* ProcessAnimation(aiAnimation* animation, aiNode* node);

	void SetVertexBoneDataToDefault( Vertex& vertex );
	void SetVertexBoneData( Vertex& vertex, int boneID, float weight );
	void ExtractBoneWeightForVertices( std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene );

	std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);

private:
	std::vector<Mesh*> meshes;
	std::vector<Texture*> loadedTextures;
	std::vector<Animation*> animations;
	std::map<std::string, BoneInfo> boneInfoMap;
	std::string path;
	int boneCount = 0;
	Animator* animator;
	Shader* shader;
};

