#include "Model.h"
#include "Animation.h"
#include "Animator.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"

Model::Model( const std::string& path, Shader& shader )
	: shader(shader)
{
	LoadModel( path, shader );

	if( animations.size() > 0 )
	{
		animator = new Animator(*animations.begin());
	}
}

Model::~Model()
{

}

void Model::Update(float delta)
{
	if( animator )
	{
		animator->UpdateAnimation(delta);
	}
}

void Model::Draw()
{
	for (auto& mesh : meshes)
	{
		mesh.Draw();
	}

	if( animations.size() > 0 )
	{
		auto transforms = animator->GetFinalBoneMatrices();
		for( int i = 0; i < transforms.size(); ++i )
		{
			shader.SetMat(( "finalBoneMatrices[" + std::to_string(i) + "]" ).c_str(), transforms[i]);
		}
	}
}

void Model::LoadModel( const std::string& path, Shader& shader )
{
	Assimp::Importer import;
	import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene* scene = import.ReadFile( path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Assimp Error: " << import.GetErrorString() << '\n';
		return;
	}

	this->path = path.substr( 0, path.find_last_of( "/\\" ) );

	ProcessNode( scene->mRootNode, scene, shader );

	if( scene->HasAnimations() )
	{
		LoadAnimations(scene);
	}
}

void Model::LoadAnimations(const aiScene* scene)
{
	aiNode* root = scene->mRootNode;

	for( unsigned int i = 0; i < scene->mNumAnimations; i++ )
	{
		auto anim = scene->mAnimations[i];
		animations.push_back(ProcessAnimation(anim, root));
	}
}

void Model::ProcessNode( aiNode* node, const aiScene* scene, Shader& shader )
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back( ProcessMesh( mesh, scene, shader ) );
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode( node->mChildren[i], scene, shader );
	}
}

Mesh Model::ProcessMesh( aiMesh* mesh, const aiScene* scene, Shader& shader )
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		SetVertexBoneDataToDefault( vertex );
		vertex.position = GetGLMVec( mesh->mVertices[i] );
		vertex.normal = GetGLMVec( mesh->mNormals[i] );

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord = GetGLMVec( mesh->mTextureCoords[0][i] );
			vertex.texCoord.y = 1.0f - vertex.texCoord.y;
		}
		else
		{
			vertex.texCoord = { 0.0f, 0.0f };
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = { GetGLMVec(mesh->mTangents[i]), 1.0 };
			vertex.biTangent = GetGLMVec( mesh->mBitangents[i] );
		}

		vertices.push_back( vertex );
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back( face.mIndices[j] );
		}
	}

	Material* material = new Material(shader);
	if (scene->HasMaterials())
	{
		aiColor3D diffuse;
		aiColor3D specular;
		aiColor3D ambient;
		float shininess = 0.0f;

		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		//Material Properties
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		mat->Get(AI_MATKEY_SHININESS, shininess);

		material->SetVec("material.ambient", GetGLMVec(ambient), true);
		material->SetVec("material.diffuse", GetGLMVec(diffuse), true);
		material->SetVec("material.specular", GetGLMVec(specular), false);
		material->SetFloat("material.shininess", shininess);

		//Textures
		std::vector<Texture*> diffuseMaps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE);
		if( diffuseMaps.empty() )
		{
			unsigned char pixels[] = {255, 255, 255, 255};
			unsigned int width = 1;
			unsigned int height = 1;
			Texture* tex = new Texture(width, height, TextureType::DIFFUSE, TextureFormat::RGBA, pixels);
			diffuseMaps.push_back(tex);
		}
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture*> specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR);
		if( specularMaps.empty() )
		{
			unsigned char pixels[] = { 0, 0, 0, 0 };
			unsigned int width = 1;
			unsigned int height = 1;
			Texture* tex = new Texture(width, height, TextureType::SPECULAR, TextureFormat::RGB, pixels);
			specularMaps.push_back(tex);
		}
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture*> normalMaps = LoadMaterialTextures(mat, aiTextureType_HEIGHT);
		if( normalMaps.empty() )
		{
			unsigned char pixels[] = { 128, 128, 255 };
			unsigned int width = 1;
			unsigned int height = 1;
			Texture* tex = new Texture(width, height, TextureType::NORMAL, TextureFormat::RGBA, pixels);
			normalMaps.push_back(tex);
		}
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		for (auto texture : textures)
		{
			switch (texture->GetTextureType())
			{
			case TextureType::DIFFUSE:
				material->SetTexture("material.diffuseTex", texture);
				break;
			case TextureType::SPECULAR:
				material->SetTexture("material.specularTex", texture);
				break;
			case TextureType::NORMAL:
				material->SetTexture("material.normalTex", texture);
				break;
			}
		}
	}


	ExtractBoneWeightForVertices( vertices, mesh, scene );

	return Mesh( mesh->mName.C_Str(), vertices, indices, material );
}

Animation* Model::ProcessAnimation(aiAnimation* animation, aiNode* node)
{
	return new Animation(node, animation, this);
}

void Model::SetVertexBoneDataToDefault( Vertex& vertex )
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.boneIDs[i] = -1;
		vertex.weights[i] = 0.0f;
	}
}

void Model::SetVertexBoneData( Vertex& vertex, int boneID, float weight )
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if (vertex.boneIDs[i] < 0)
		{
			vertex.weights[i] = weight;
			vertex.boneIDs[i] = boneID;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices( std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene )
{
	auto& boneMap = boneInfoMap;
	int& boneCount = this->boneCount;

	for (unsigned int bIndex = 0; bIndex < mesh->mNumBones; ++bIndex)
	{
		aiBone* bone = mesh->mBones[bIndex];

		int boneID = -1;
		std::string boneName = bone->mName.C_Str();

		if (boneMap.find( boneName ) == boneMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.ID = boneCount;
			newBoneInfo.offset = GetGLMMat( bone->mOffsetMatrix );
			boneMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneMap[boneName].ID;
		}

		assert( boneID != -1 );

		auto weights = bone->mWeights;
		int numWeights = bone->mNumWeights;

		for (int wIndex = 0; wIndex < numWeights; ++wIndex)
		{
			int vertexID = weights[wIndex].mVertexId;
			float weight = weights[wIndex].mWeight;
			assert( vertexID <= vertices.size() );
			SetVertexBoneData( vertices[vertexID], boneID, weight );
		}
	}
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j]->GetFilePath().data(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			std::string filepath = path + '\\' + str.C_Str();
			Texture* tex = new Texture(filepath, GetTextureType(type));

			textures.push_back(tex);
			loadedTextures.push_back(tex);
		}
	}

	return textures;
}
