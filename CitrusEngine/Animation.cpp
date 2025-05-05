#include "Animation.h"
#include "Bone.h"
#include "Model.h"
#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

Animation::Animation(aiNode* node, aiAnimation* animation, Model* model)
{
	duration = (float)animation->mDuration;
	ticksPerSecond = (float)animation->mTicksPerSecond;

	name = animation->mName.C_Str();
	name = name.substr(name.find_last_of("/\\") + 1);
	ReadHierarchyData(rootNode, node);
	ReadMissingBones(animation, *model);
}

Animation::Animation(const std::string& filePath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);

	auto animation = scene->mAnimations[0];

	name = animation->mName.C_Str();
	name = name.substr(name.find_last_of("|") + 1);

	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();

	duration = (float)animation->mDuration;
	ticksPerSecond = (float)animation->mTicksPerSecond;
	ReadHierarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Animation::~Animation()
{
}

Bone* Animation::FindBone(const std::string& name)
{
	auto it = std::find_if(bones.begin(), bones.end(), [&](const Bone& bone)
		{
			return bone.GetBoneName() == name;
		});

	if( it == bones.end() ) return nullptr;
	else return &( *it );
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;

	auto& boneInfoMap = model.GetBoneInfoMap();
	int& boneCount = model.GetBoneCount();

	for( int i = 0; i < size; i++ )
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if( boneInfoMap.find(boneName) == boneInfoMap.end() )
		{
			boneInfoMap[boneName].ID = boneCount;
			boneCount++;
		}
		bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].ID, channel));
	}

	this->boneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(NodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = GetGLMMat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for( unsigned int i = 0; i < src->mNumChildren; i++ )
	{
		NodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}
