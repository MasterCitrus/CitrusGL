#pragma once

#include "BoneInfo.h"

#include <glm/glm.hpp>

#include <map>
#include <string>
#include <vector>

class Bone;
class Model;

struct aiAnimation;
struct aiNode;
struct aiScene;

struct NodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<NodeData> children;
};

class Animation
{
public:
	Animation() = default;
	Animation(aiNode* node, aiAnimation* animation, Model* model);
	Animation(const std::string& filePath, Model* model);
	~Animation();

	Bone* FindBone(const std::string& name);

	inline std::string& GetName() { return name; }
	inline float GetTicksPerSecond() const { return ticksPerSecond; }
	inline float GetDuration() const { return duration; }
	inline const NodeData& GetRootNode() { return rootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return boneInfoMap; }

private:
	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHierarchyData(NodeData& dest, const aiNode* src);

private:
	std::map<std::string, BoneInfo> boneInfoMap;
	std::vector<Bone> bones;
	NodeData rootNode;
	std::string name;
	float duration;
	float ticksPerSecond;
};

