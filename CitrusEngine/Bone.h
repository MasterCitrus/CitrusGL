#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <vector>

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

struct aiNodeAnim;

class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);

	void Update(float animationTime);

	glm::mat4 GetLocalTransform() const { return localTransform; }
	std::string GetBoneName() const { return name; }
	int GetBoneID() const { return ID; }

	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animatinTime);
	int GetScaleIndex(float animationTime);

private:
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScale(float animationTime);

private:
	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;
	glm::mat4 localTransform;
	std::string name;
	int numPositions;
	int numRotations;
	int numScales;
	int ID;
};

