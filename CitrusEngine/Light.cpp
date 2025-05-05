#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: position(position), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

Light::~Light()
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: Light(position, ambient, diffuse, specular)
{
}

PointLight::~PointLight()
{
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: Light(position, ambient, diffuse, specular)
{
}

SpotLight::~SpotLight()
{
}

DirectionLight::DirectionLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: Light(direction, ambient, diffuse, specular)
{
}

DirectionLight::~DirectionLight()
{
}
