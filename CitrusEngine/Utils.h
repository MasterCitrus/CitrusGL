#pragma once

#include "Texture.h"
#include "UniformData.h"

#include <string>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <assimp/types.h>
#include <assimp/color4.h>
#include <assimp/matrix4x4.h>
#include "assimp/quaternion.h"
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <assimp/material.h>

//ASSIMP CONVERSIONS

//Colours
 glm::vec3 GetGLMVec(const aiColor3D& colour);
 glm::vec4 GetGLMVec(const aiColor4D& colour);
//Vectors
 glm::vec2 GetGLMVec( const aiVector2D& vec );
 glm::vec3 GetGLMVec( const aiVector3D& vec );
//Matrices
 glm::mat4 GetGLMMat( const aiMatrix4x4& from );
//Quaternion
 glm::quat GetGLMQuat( const aiQuaternion& quat );

 //Texture Type
 TextureType GetTextureType(aiTextureType type);

//OPENGL UTILITIES

std::string GetGLType( unsigned int type );
UniformType GetUniformType(unsigned int type);