#version 460 core

out vec4 FragColour;

in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{
    FragColour = texture(skybox, TexCoord);
}