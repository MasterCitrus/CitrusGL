#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec4 Tangent;
layout(location = 4) in vec3 Bitangent;
layout(location = 5) in ivec4 BoneIDs;
layout(location = 6) in vec4 Weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool isSkinned;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragTexCoord;
out vec3 FragTangent;
out vec3 FragBitangent;

void main()
{
	vec4 totalPosition = vec4(0.0f);
	vec3 totalNormal = vec3(0.0f);
	if(isSkinned)
	{
		for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if(BoneIDs[i] == -1) continue;
			if(BoneIDs[i] >= MAX_BONES)
			{
				totalPosition = vec4(Position, 1.0);
				totalNormal = Normal;
				break;
			}
			
			vec4 localPosition = finalBoneMatrices[BoneIDs[i]] * vec4(Position, 1.0);
			totalPosition += localPosition * Weights[i];
			vec3 localNormal = mat3(finalBoneMatrices[BoneIDs[i]]) * Normal;
			totalNormal += localNormal * Weights[i];
		}
	}
	else
	{
		totalPosition = vec4(Position, 1.0);
		totalNormal = Normal;
	}

    FragPos = vec3(model * totalPosition);
    FragNormal = mat3(model) * totalNormal;
    FragTexCoord = TexCoord;
    FragTangent = vec3(model * vec4(Tangent.xyz, 0.0)).xyz;
    FragBitangent = cross(FragNormal, FragTangent) * Tangent.w;
    gl_Position = projection * view * model * vec4(FragPos, 1.0);
}