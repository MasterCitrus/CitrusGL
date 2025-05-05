#version 460 core

in vec4 FragPos;
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

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

const int MAX_LIGHTS = 4;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColour;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragpos, vec3 viewDir);

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
	
	vec3 viewDir = normalize(viewPos - FragPos.xyz);

	vec3 result = CalDirLight(dirLight, norm, viewDir);
	
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos.xyz, viewDir);
	}
	
	result += CalcSpotLight(spotLight, norm, FragPos.xyz, viewDir);

    FragColour = vec4(result, 1.0);
}

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction.xyz);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseTex, FragTexCoord));
	vec3 diffuse = light.diffuse * material.diffuse * diff * vec3(texture(material.diffuseTex, FragTexCoord));
	vec3 specular = light.specular * material.specular * spec * vec3(texture(material.specularTex, FragTexCoord));
	
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos.xyz);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	
	float distance = length(light.position - FragPos.xyz);
	float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic + (distance * distance));
	
	vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseTex, FragTexCoord));
	vec3 diffuse = light.diffuse * material.diffuse * diff * vec3(texture(material.diffuseTex, FragTexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specularTex, FragTexCoord));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos.xyz);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	
	float distance = length(light.position - FragPos.xyz);
	float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic + (distance * distance));
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseTex, FragTexCoord));
	vec3 diffuse = light.diffuse * material.diffuse * diff * vec3(texture(material.diffuseTex, FragTexCoord));
	vec3 specular = light.specular * material.specular* spec * vec3(texture(material.specularTex, FragTexCoord));
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (ambient + diffuse + specular);
}