#include "Model.h"
#include "Utils.h"

Model::Model( const std::string& path )
{
	LoadModel( path );
}

Model::~Model()
{

}

void Model::Draw( Shader& shader )
{
	for (auto& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void Model::LoadModel( const std::string& path )
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile( path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Assimp Error: " << import.GetErrorString() << '\n';
		return;
	}

	this->path = path.substr( 0, path.find_last_of( "/\\" ) );

	ProcessNode( scene->mRootNode, scene );
}

void Model::ProcessNode( aiNode* node, const aiScene* scene )
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back( ProcessMesh( mesh, scene ) );
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode( node->mChildren[i], scene );
	}
}

Mesh Model::ProcessMesh( aiMesh* mesh, const aiScene* scene )
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		SetVertexBoneDataToDefault( vertex );
		vertex.position = GetGLMVec( mesh->mVertices[i] );
		vertex.normal = GetGLMVec( mesh->mNormals[i] );

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord = GetGLMVec( mesh->mTextureCoords[0][i] );
		}
		else
		{
			vertex.texCoord = { 0.0f, 0.0f };
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = GetGLMVec( mesh->mTangents[i] );
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

	//Put materials here?

	ExtractBoneWeightForVertices( vertices, mesh, scene );

	return Mesh( vertices, indices, textures );
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

	for (int bIndex = 0; bIndex < mesh->mNumBones; ++bIndex)
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
