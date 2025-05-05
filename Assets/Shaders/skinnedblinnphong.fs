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

	bool useDiffuseTex;
	bool useSpecularTex;
	bool useNormalTex;

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

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

//Light Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 GetNormal();
vec3 GetDiffuseColour();
vec3 GetSpecularColour();

void main() 
{
    vec3 normal = GetNormal();

	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
	}
	
	result += CalcSpotLight(spotLight, normal, FragPos, viewDir);
    FragColour = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	
	vec3 ambient = light.ambient * material.ambient * GetDiffuseColour();
	vec3 diffuse = light.diffuse *  diff * GetDiffuseColour();
	vec3 specular = light.specular * spec * GetSpecularColour();
	
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic + (distance * distance));
	
	vec3 ambient = light.ambient * material.ambient * GetDiffuseColour();
	vec3 diffuse = light.diffuse *  diff * GetDiffuseColour();
	vec3 specular = light.specular * spec * GetSpecularColour();
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic + (distance * distance));
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec3 ambient = light.ambient * material.ambient * GetDiffuseColour();
	vec3 diffuse = light.diffuse *  diff * GetDiffuseColour();
	vec3 specular = light.specular * spec * GetSpecularColour();
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (ambient + diffuse + specular);
}

vec3 GetNormal()
{
	if(material.useNormalTex)
	{
		vec3 norm = normalize(FragNormal);
    	vec3 btan = normalize(FragBitangent);
    	vec3 ntan = normalize(FragTangent);

		vec3 normal = texture(material.normalTex, FragTexCoord).rgb;

    	mat3 TBN = mat3(ntan, btan, norm);

		return TBN * (normal * 2 - 1);
	}
	else
	{
		return normalize(FragNormal);
	}
}

vec3 GetDiffuseColour()
{
	return material.useDiffuseTex ? texture(material.diffuseTex, FragTexCoord).rgb : material.diffuse;
}

vec3 GetSpecularColour()
{
	return material.useSpecularTex ? texture(material.specularTex, FragTexCoord).rgb : material.specular;
}