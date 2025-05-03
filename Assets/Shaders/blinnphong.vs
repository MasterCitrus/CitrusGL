#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;
layout(location = 5) in ivec4 BoneIDs;
layout(location = 6) in vec4 Weigths;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragTexCoord;

void main()
{
    FragPos = vec3(model * vec4(Position, 1.0));
    FragNormal = mat3(model) * Normal;
    FragTexCoord = TexCoord;
    gl_Position = projection * view * model * vec4(FragPos, 1.0);
}