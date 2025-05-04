#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

constexpr int MAX_BONE_INFLUENCE = 4;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec4 tangent;
	glm::vec3 biTangent;
	int boneIDs[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};

