#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	virtual ~Light();

	glm::vec3& GetPosition() { return position; }
	glm::vec3& GetAmbient() { return ambient; }
	glm::vec3& GetDiffuse() { return diffuse; }
	glm::vec3& GetSpecular() { return specular; }

private:
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~PointLight();

	float& GetLinear() { return linear; }
	float& GetQuadratic() { return quadratic; }
	float& GetIntensity() { return intensity; }

private:
	float linear = 0.09f;
	float quadratic = 0.032f;
	float intensity = 1.0f;
};

class SpotLight : public Light
{
public:
	SpotLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~SpotLight();

	float& GetLinear() { return linear; }
	float& GetQuadratic() { return quadratic; }
	float& GetCutOff() { return cutOff; }
	float& GetOuterCutOff() { return outerCutOff; }

private:
	float linear;
	float quadratic;
	//Store as radians
	float cutOff;
	float outerCutOff;
};

class DirectionLight : public Light
{
public:
	DirectionLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~DirectionLight();
};