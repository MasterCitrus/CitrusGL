#pragma once

#include <glm/vec3.hpp>

class Light
{
public:
	Light();
	virtual ~Light();


private:
	glm::vec3 position;
	glm::vec3 colour;
	float intensity;
};

class PointLight : public Light
{
public:

private:
};

class SpotLight : public Light
{
public:

private:
};

class DirectionLight : public Light
{
public:

private:
};