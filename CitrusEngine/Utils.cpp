#include "Utils.h"

#include <glad/glad.h>

glm::vec3 GetGLMVec(const aiColor3D& colour)
{
	return glm::vec3(colour.r, colour.g, colour.b);
}

glm::vec4 GetGLMVec(const aiColor4D& colour)
{
	return glm::vec4(colour.r, colour.g, colour.b, colour.a);
}

glm::vec2 GetGLMVec( const aiVector2D& vec )
{
	return glm::vec2( vec.x, vec.y );
}

glm::vec3 GetGLMVec( const aiVector3D& vec )
{
	return glm::vec3( vec.x, vec.y, vec.z );
}

glm::mat4 GetGLMMat( const aiMatrix4x4& from )
{
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

glm::quat GetGLMQuat( const aiQuaternion& quat )
{
	return glm::quat( quat.w, quat.x, quat.y, quat.z );
}

std::string GetGLType( unsigned int type )
{
	switch (type)
	{
	case GL_FLOAT:
		return "Float";
	case GL_FLOAT_VEC2:
		return "Vec2";
	case GL_FLOAT_VEC3:
		return "Vec3";
	case GL_FLOAT_VEC4:
		return "Vec4";
	case GL_DOUBLE:
		return "Double";
	case GL_DOUBLE_VEC2:
		return "DVec2";
	case GL_DOUBLE_VEC3:
		return "DVec3";
	case GL_DOUBLE_VEC4:
		return "DVec4";
	case GL_INT:
		return "Int";
	case GL_INT_VEC2:
		return "IVec2";
	case GL_INT_VEC3:
		return "IVec3";
	case GL_INT_VEC4:
		return "IVec4";
	case GL_UNSIGNED_INT:
		return "Unsigned Int";
	case GL_UNSIGNED_INT_VEC2:
		return "UVec2";
	case GL_UNSIGNED_INT_VEC3:
		return "UVec3";
	case GL_UNSIGNED_INT_VEC4:
		return "UVec4";
	case GL_BOOL:
		return "Boolean";
	case GL_BOOL_VEC2:
		return "BVec2";
	case GL_BOOL_VEC3:
		return "BVec3";
	case GL_BOOL_VEC4:
		return "BVec4";
	case GL_FLOAT_MAT2:
		return "Matrix2";
	case GL_FLOAT_MAT3:
		return "Matrix3";
	case GL_FLOAT_MAT4:
		return "Matrix4";
	case GL_FLOAT_MAT2x3:
		return "Matrix2x3";
	case GL_FLOAT_MAT2x4:
		return "Matrix2x4";
	case GL_FLOAT_MAT3x2:
		return "Matrix3x2";
	case GL_FLOAT_MAT3x4:
		return "Matrix3x4";
	case GL_FLOAT_MAT4x2:
		return "Matrix4x2";
	case GL_FLOAT_MAT4x3:
		return "Matrix4x3";
	case GL_DOUBLE_MAT2:
		return "DMatrix2";
	case GL_DOUBLE_MAT3:
		return "DMatrix3";
	case GL_DOUBLE_MAT4:
		return "DMatrix4";
	case GL_DOUBLE_MAT2x3:
		return "DMatrix2x3";
	case GL_DOUBLE_MAT2x4:
		return "DMatrix2x4";
	case GL_DOUBLE_MAT3x2:
		return "DMatrix3x2";
	case GL_DOUBLE_MAT3x4:
		return "DMatrix3x4";
	case GL_DOUBLE_MAT4x2:
		return "DMatrix4x2";
	case GL_DOUBLE_MAT4x3:
		return "DMatrix4x3";
	case GL_SAMPLER_1D:
		return "Sampler1D";
	case GL_SAMPLER_2D:
		return "Sampler2D";
	case GL_SAMPLER_3D:
		return "Sampler3D";
	case GL_SAMPLER_CUBE:
		return "SamplerCube";
	case GL_SAMPLER_1D_SHADOW:
		return "Sampler1D Shadow";
	case GL_SAMPLER_2D_SHADOW:
		return "Sampler2D Shadow";
	case GL_SAMPLER_1D_ARRAY:
		return "Sampler1DArray";
	case GL_SAMPLER_2D_ARRAY:
		return "Sampler2DArray";
	case GL_SAMPLER_1D_ARRAY_SHADOW:
		return "Sampler1DArrayShadow";
	case GL_SAMPLER_2D_ARRAY_SHADOW:
		return "Sampler2DArrayShadow";
	case GL_SAMPLER_2D_MULTISAMPLE:
		return "Sampler2DMS";
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "Sampler2DMSArray";
	case GL_SAMPLER_CUBE_SHADOW:
		return "SamplerCubeShadow";
	case GL_SAMPLER_BUFFER:
		return "SamplerBuffer";
	case GL_SAMPLER_2D_RECT:
		return "Sampler2DRect";
	case GL_SAMPLER_2D_RECT_SHADOW:
		return "sampelr2DRectShadow";
	case GL_INT_SAMPLER_1D:
		return "ISampler1D";
	case GL_INT_SAMPLER_2D:
		return "ISampler2D";
	case GL_INT_SAMPLER_3D:
		return "ISampler3D";
	case GL_INT_SAMPLER_CUBE:
		return "ISamplerCube";
	case GL_INT_SAMPLER_1D_ARRAY:
		return "ISampler1DArray";
	case GL_INT_SAMPLER_2D_ARRAY:
		return "ISampler2DArray";
	case GL_INT_SAMPLER_2D_MULTISAMPLE:
		return "ISampler2DMS";
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "ISampler2DMSArray";
	case GL_INT_SAMPLER_BUFFER:
		return "ISamplerBuffer";
	case GL_INT_SAMPLER_2D_RECT:
		return "ISampler2DRect";
	case GL_UNSIGNED_INT_SAMPLER_1D:
		return "USampler1D";
	case GL_UNSIGNED_INT_SAMPLER_2D:
		return "USampler2D";
	case GL_UNSIGNED_INT_SAMPLER_3D:
		return "USampler3D";
	case GL_UNSIGNED_INT_SAMPLER_CUBE:
		return "USamplerCube";
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
		return "USampler1DArray";
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
		return "USampler2DArray";
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
		return "USampler2DMS";
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "USampler2DMSArray";
	case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		return "USamplerBuffer";
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
		return "USampler2DRect";
	case GL_IMAGE_1D:
		return "Image1D";
	case GL_IMAGE_2D:
		return "Image2D";
	case GL_IMAGE_3D:
		return "Image3D";
	case GL_IMAGE_2D_RECT:
		return "Image2DRect";
	case GL_IMAGE_CUBE:
		return "ImageCube";
	case GL_IMAGE_BUFFER:
		return "ImageBuffer";
	case GL_IMAGE_1D_ARRAY:
		return "Image1DArray";
	case GL_IMAGE_2D_ARRAY:
		return "Image2DArray";
	case GL_IMAGE_2D_MULTISAMPLE:
		return "Image1DMS";
	case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
		return "Image2DMSArray";
	case GL_INT_IMAGE_1D:
		return "IImage1D";
	case GL_INT_IMAGE_2D:
		return "IImage2D";
	case GL_INT_IMAGE_3D:
		return "IImage3D";
	case GL_INT_IMAGE_2D_RECT:
		return "IImage2DRect";
	case GL_INT_IMAGE_CUBE:
		return "IImageCube";
	case GL_INT_IMAGE_BUFFER:
		return "IImageBuffer";
	case GL_INT_IMAGE_1D_ARRAY:
		return "IImage1DArray";
	case GL_INT_IMAGE_2D_ARRAY:
		return "IImage2DArray";
	case GL_INT_IMAGE_2D_MULTISAMPLE:
		return "IImage1DMS";
	case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
		return "IImage2DMSArray";
	case GL_UNSIGNED_INT_IMAGE_1D:
		return "UImage1D";
	case GL_UNSIGNED_INT_IMAGE_2D:
		return "UImage2D";
	case GL_UNSIGNED_INT_IMAGE_3D:
		return "UImage3D";
	case GL_UNSIGNED_INT_IMAGE_2D_RECT:
		return "UImage2DRect";
	case GL_UNSIGNED_INT_IMAGE_CUBE:
		return "UImageCube";
	case GL_UNSIGNED_INT_IMAGE_BUFFER:
		return "UImageBuffer";
	case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
		return "UImage1DArray";
	case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
		return "UImage2DArray";
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
		return "UImage1DMS";
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
		return "UImage2DMSArray";
	case GL_UNSIGNED_INT_ATOMIC_COUNTER:
		return "AtomicUInt";
	default:
		return "INVALID";
	}
}

UniformType GetUniformType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT: 
		return FLOAT;
	case GL_FLOAT_VEC2: 
		return FLOAT_VEC2;
	case GL_FLOAT_VEC3: 
		return FLOAT_VEC3;
	case GL_FLOAT_VEC4: 
		return FLOAT_VEC4;
	case GL_DOUBLE: 
		return DOUBLE;
	case GL_DOUBLE_VEC2: 
		return DOUBLE_VEC2;
	case GL_DOUBLE_VEC3: 
		return DOUBLE_VEC3;
	case GL_DOUBLE_VEC4: 
		return DOUBLE_VEC4;
	case GL_INT: 
		return INT;
	case GL_INT_VEC2: 
		return INT_VEC2;
	case GL_INT_VEC3: 
		return INT_VEC3;
	case GL_INT_VEC4: 
		return INT_VEC4;
	case GL_UNSIGNED_INT: 
		return UNSIGNED_INT;
	case GL_UNSIGNED_INT_VEC2: 
		return UNSIGNED_INT_VEC2;
	case GL_UNSIGNED_INT_VEC3: 
		return UNSIGNED_INT_VEC3;
	case GL_UNSIGNED_INT_VEC4: 
		return UNSIGNED_INT_VEC4;
	case GL_BOOL: 
		return BOOL;
	case GL_BOOL_VEC2: 
		return BOOL_VEC2;
	case GL_BOOL_VEC3: 
		return BOOL_VEC3;
	case GL_BOOL_VEC4: 
		return BOOL_VEC4;
	case GL_FLOAT_MAT2: 
		return FLOAT_MAT2;
	case GL_FLOAT_MAT3: 
		return FLOAT_MAT3;
	case GL_FLOAT_MAT4: 
		return FLOAT_MAT4;
	case GL_FLOAT_MAT2x3: 
		return FLOAT_MAT2X3;
	case GL_FLOAT_MAT2x4: 
		return FLOAT_MAT2X4;
	case GL_FLOAT_MAT3x2: 
		return FLOAT_MAT3X2;
	case GL_FLOAT_MAT3x4: 
		return FLOAT_MAT3X4;
	case GL_FLOAT_MAT4x2: 
		return FLOAT_MAT4X2;
	case GL_FLOAT_MAT4x3: 
		return FLOAT_MAT4X3;
	case GL_DOUBLE_MAT2: 
		return DOUBLE_MAT2;
	case GL_DOUBLE_MAT3: 
		return DOUBLE_MAT3;
	case GL_DOUBLE_MAT4: 
		return DOUBLE_MAT4;
	case GL_DOUBLE_MAT2x3: 
		return DOUBLE_MAT2X3;
	case GL_DOUBLE_MAT2x4: 
		return DOUBLE_MAT2X4;
	case GL_DOUBLE_MAT3x2: 
		return DOUBLE_MAT3X2;
	case GL_DOUBLE_MAT3x4: 
		return DOUBLE_MAT3X4;
	case GL_DOUBLE_MAT4x2: 
		return DOUBLE_MAT4X2;
	case GL_DOUBLE_MAT4x3:
		return DOUBLE_MAT4X3;
	case GL_SAMPLER_1D: 
		return SAMPLER_1D;
	case GL_SAMPLER_2D: 
		return SAMPLER_2D;
	case GL_SAMPLER_3D: 
		return SAMPLER_3D;
	case GL_SAMPLER_CUBE: 
		return SAMPLER_CUBE;
	case GL_SAMPLER_1D_SHADOW: 
		return SAMPLER_1D_SHADOW;
	case GL_SAMPLER_2D_SHADOW: 
		return SAMPLER_2D_SHADOW;
	case GL_SAMPLER_1D_ARRAY: 
		return SAMPLER_1D_ARRAY;
	case GL_SAMPLER_2D_ARRAY: 
		return SAMPLER_2D_ARRAY;
	case GL_SAMPLER_1D_ARRAY_SHADOW: 
		return SAMPLER_1D_ARRAY_SHADOW;
	case GL_SAMPLER_2D_ARRAY_SHADOW: 
		return SAMPLER_2D_ARRAY_SHADOW;
	case GL_SAMPLER_2D_MULTISAMPLE: 
		return SAMPLER_2D_MULTISAMPLE;
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: 
		return SAMPLER_2D_MULTISAMPLE_ARRAY;
	case GL_SAMPLER_CUBE_SHADOW: 
		return SAMPLER_CUBE_SHADOW;
	case GL_SAMPLER_BUFFER: 
		return SAMPLER_BUFFER;
	case GL_SAMPLER_2D_RECT: 
		return SAMPLER_2D_RECT;
	case GL_SAMPLER_2D_RECT_SHADOW: 
		return SAMPLER_2D_RECT_SHADOW;
	case GL_INT_SAMPLER_1D: 
		return INT_SAMPLER_1D;
	case GL_INT_SAMPLER_2D: 
		return INT_SAMPLER_2D;
	case GL_INT_SAMPLER_3D: 
		return INT_SAMPLER_3D;
	case GL_INT_SAMPLER_CUBE: 
		return INT_SAMPLER_CUBE;
	case GL_INT_SAMPLER_1D_ARRAY: 
		return INT_SAMPLER_1D_ARRAY;
	case GL_INT_SAMPLER_2D_ARRAY: 
		return INT_SAMPLER_2D_ARRAY;
	case GL_INT_SAMPLER_2D_MULTISAMPLE: 
		return INT_SAMPLER_2D_MULTISAMPLE;
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: 
		return INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
	case GL_INT_SAMPLER_BUFFER: 
		return INT_SAMPLER_BUFFER;
	case GL_INT_SAMPLER_2D_RECT: 
		return INT_SAMPLER_2D_RECT;
	case GL_UNSIGNED_INT_SAMPLER_1D: 
		return UNSIGNED_INT_SAMPLER_1D;
	case GL_UNSIGNED_INT_SAMPLER_2D: 
		return UNSIGNED_INT_SAMPLER_2D;
	case GL_UNSIGNED_INT_SAMPLER_3D: 
		return UNSIGNED_INT_SAMPLER_3D;
	case GL_UNSIGNED_INT_SAMPLER_CUBE: 
		return UNSIGNED_INT_SAMPLER_CUBE;
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: 
		return UNSIGNED_INT_SAMPLER_1D_ARRAY;
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: 
		return UNSIGNED_INT_SAMPLER_2D_ARRAY;
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: 
		return UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: 
		return UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
	case GL_UNSIGNED_INT_SAMPLER_BUFFER: 
		return UNSIGNED_INT_SAMPLER_BUFFER;
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT: 
		return UNSIGNED_INT_SAMPLER_2D_RECT;
	case GL_IMAGE_1D: 
		return IMAGE_1D;
	case GL_IMAGE_2D: 
		return IMAGE_2D;
	case GL_IMAGE_3D: 
		return IMAGE_3D;
	case GL_IMAGE_2D_RECT: 
		return IMAGE_2D_RECT;
	case GL_IMAGE_CUBE: 
		return IMAGE_CUBE;
	case GL_IMAGE_BUFFER: 
		return IMAGE_BUFFER;
	case GL_IMAGE_1D_ARRAY: 
		return IMAGE_1D_ARRAY;
	case GL_IMAGE_2D_ARRAY: 
		return IMAGE_2D_ARRAY;
	case GL_IMAGE_2D_MULTISAMPLE: 
		return IMAGE_2D_MULTISAMPLE;
	case GL_IMAGE_2D_MULTISAMPLE_ARRAY: 
		return IMAGE_2D_MULTISAMPLE_ARRAY;
	case GL_INT_IMAGE_1D: 
		return INT_IMAGE_1D;
	case GL_INT_IMAGE_2D: 
		return INT_IMAGE_2D;
	case GL_INT_IMAGE_3D: 
		return INT_IMAGE_3D;
	case GL_INT_IMAGE_2D_RECT: 
		return INT_IMAGE_2D_RECT;
	case GL_INT_IMAGE_CUBE: 
		return INT_IMAGE_CUBE;
	case GL_INT_IMAGE_BUFFER: 
		return INT_IMAGE_BUFFER;
	case GL_INT_IMAGE_1D_ARRAY: 
		return INT_IMAGE_1D_ARRAY;
	case GL_INT_IMAGE_2D_ARRAY: 
		return INT_IMAGE_2D_ARRAY;
	case GL_INT_IMAGE_2D_MULTISAMPLE: 
		return INT_IMAGE_2D_MULTISAMPLE;
	case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: 
		return INT_IMAGE_2D_MULTISAMPLE_ARRAY;
	case GL_UNSIGNED_INT_IMAGE_1D: 
		return UNSIGNED_INT_IMAGE_1D;
	case GL_UNSIGNED_INT_IMAGE_2D: 
		return UNSIGNED_INT_IMAGE_2D;
	case GL_UNSIGNED_INT_IMAGE_3D: 
		return UNSIGNED_INT_IMAGE_3D;
	case GL_UNSIGNED_INT_IMAGE_2D_RECT: 
		return UNSIGNED_INT_IMAGE_2D_RECT;
	case GL_UNSIGNED_INT_IMAGE_CUBE:
		return UNSIGNED_INT_IMAGE_CUBE;
	case GL_UNSIGNED_INT_IMAGE_BUFFER:
		return UNSIGNED_INT_IMAGE_BUFFER;
	case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: 
		return UNSIGNED_INT_IMAGE_1D_ARRAY;
	case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: 
		return UNSIGNED_INT_IMAGE_2D_ARRAY;
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: 
		return UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: 
		return UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
	case GL_UNSIGNED_INT_ATOMIC_COUNTER: 
		return UNSIGNED_INT_ATOMIC_COUNTER;
	default:
		return UNDEFINED;
	}
}
