#version 460 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoord;
in vec3 FragTangent;
in vec3 FragBitangent;

out vec4 FragColour;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D diffuseTex;
    sampler2D specularTex;
    sampler2D normalTex;
};

uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColour;

void main() 
{
    vec3 norm = normalize(FragNormal);
    vec3 btan = normalize(FragBitangent);
    vec3 ntan = normalize(FragTangent);

    mat3 TBN = mat3(ntan, btan, norm);

    vec3 texDiffuse = texture(material.diffuseTex, FragTexCoord).rgb;
    vec3 texSpecular = texture(material.specularTex, FragTexCoord).rgb;
    vec3 texNormal = texture(material.normalTex, FragTexCoord).rgb;

    norm = TBN * (texNormal * 2 - 1);

    vec3 ambient = material.ambient * lightColour * texDiffuse;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff * lightColour * texDiffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * lightColour * texSpecular;

    vec3 result = ambient + diffuse + specular;
    FragColour = vec4(result, 1.0);
}