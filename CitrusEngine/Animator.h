#pragma once

#include <glm/glm.hpp>

#include <vector>

class Animation;
struct NodeData;

const int MAX_BONES = 100;

class Animator
{
public:
	Animator(Animation* animation);

	void UpdateAnimation(float delta);
	void PlayAnimation(Animation* animation);
	void CalculateBoneTransform(const NodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices() const { return finalBoneMatrices; }
	bool& GetIsPlaying() { return isPlaying; }
	Animation* GetCurrentAnimation() const { return currentAnimation; }

private:
	std::vector<glm::mat4> finalBoneMatrices;
	Animation* currentAnimation;
	float currentTime;
	float deltaTime;
	bool isPlaying = true;
};

