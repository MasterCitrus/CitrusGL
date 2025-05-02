#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

#include <assimp/vector3.h>
#include <assimp/vector2.h>
#include <assimp/matrix4x4.h>
#include "assimp/quaternion.h"

//ASSIMP CONVERSIONS

//Vectors
static glm::vec2 GetGLMVec( const aiVector2D& vec );
static glm::vec3 GetGLMVec( const aiVector3D& vec );
//Matrices
static glm::mat4 GetGLMMat( const aiMatrix4x4& from );
//Quaternion
static glm::quat GetGLMQuat( const aiQuaternion& quat );

//OPENGL UTILITIES

std::string GetGLType( unsigned int type );