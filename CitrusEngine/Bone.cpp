#include "Bone.h"
#include "Utils.h"

#include <assimp/scene.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
	: name(name), ID(ID), localTransform(1.0f)
{
	numPositions = channel->mNumPositionKeys;
	for( int i = 0; i < numPositions; ++i )
	{
		KeyPosition data;
		data.position = GetGLMVec(channel->mPositionKeys[i].mValue);
		data.timeStamp = (float)channel->mPositionKeys[i].mTime;
		positions.push_back(data);
	}

	numRotations = channel->mNumRotationKeys;
	for( int i = 0; i < numRotations; ++i )
	{
		KeyRotation data;
		data.orientation = GetGLMQuat(channel->mRotationKeys[i].mValue);
		data.timeStamp = (float)channel->mRotationKeys[i].mTime;
		rotations.push_back(data);
	}

	numScales = channel->mNumScalingKeys;
	for( int i = 0; i < numScales; ++i )
	{
		KeyScale data;
		data.scale = GetGLMVec(channel->mScalingKeys[i].mValue);
		data.timeStamp = (float)channel->mScalingKeys[i].mTime;
		scales.push_back(data);
	}
}

void Bone::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScale(animationTime);
	localTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(float animationTime)
{
	for( int i = 0; i < numPositions - 1; ++i )
	{
		if( animationTime < positions[i + 1].timeStamp )
		{
			return i;
		}
	}
}

int Bone::GetRotationIndex(float animationTime)
{
	for( int i = 0; i < numRotations - 1; ++i )
	{
		if( animationTime < rotations[i + 1].timeStamp )
		{
			return i;
		}
	}
}

int Bone::GetScaleIndex(float animationTime)
{
	for( int i = 0; i < numScales - 1; ++i )
	{
		if( animationTime < scales[i + 1].timeStamp )
		{
			return i;
		}
	}
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if( numPositions == 1 )
	{
		return glm::translate(glm::mat4(1.0f), positions[0].position);
	}

	int p0 = GetPositionIndex(animationTime);
	int p1 = p0 + 1;
	float scaleFactor = GetScaleFactor(positions[p0].timeStamp, positions[p1].timeStamp, animationTime);

	glm::vec3 finalPosition = glm::mix(positions[p0].position, positions[p1].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if( numRotations == 1 )
	{
		glm::quat rotation = glm::normalize(rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0 = GetRotationIndex(animationTime);
	int p1 = p0 + 1;
	float scaleFactor = GetScaleFactor(rotations[p0].timeStamp, rotations[p1].timeStamp, animationTime);

	glm::quat finalRotation = glm::slerp(rotations[p0].orientation, rotations[p1].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScale(float animationTime)
{
	if( numScales == 1 )
	{
		return glm::scale(glm::mat4(1.0f), scales[0].scale);
	}

	int p0 = GetScaleIndex(animationTime);
	int p1 = p0 + 1;
	float scaleFactor = GetScaleFactor(scales[p0].timeStamp, scales[p1].timeStamp, animationTime);

	glm::vec3 finalScale = glm::mix(scales[p0].scale, scales[p1].scale, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}
