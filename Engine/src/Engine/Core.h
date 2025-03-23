#pragma once

#include <cstdint>
#include <memory>

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

namespace CitrusGL
{
	template<typename T>
	using Ptr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ptr<T> CreatePtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}