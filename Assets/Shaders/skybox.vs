#version 460 core

layout(location = 0) in vec3 Pos;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 position = projection * view * vec4(Pos, 1.0);
    TexCoord = vec3(Pos.x, Pos.y, -Pos.z);
    gl_Position = vec4(position.x, position.y, position.w, position.w);
}