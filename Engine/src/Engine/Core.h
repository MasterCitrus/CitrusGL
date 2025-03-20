#pragma once

#include <cstdint>

#ifdef CGL_DEBUG
	#define CGL_ENABLE_ASSERTS
#endif

#ifdef CGL_ENABLE_ASSERTS
#define CGL_ASSERT(x, ...) { if(!(x)) { CGL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define CGL_CORE_ASSERT(x, ...) { if(!(x)) { CGL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define CGL_ASSERT(x, ...)
#define CGL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (x << 1)

#define CGL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)