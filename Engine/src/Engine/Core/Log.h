#pragma once

#include "Engine/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace CitrusGL
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}

#define CGL_CORE_TRACE(...) ::CitrusGL::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CGL_CORE_INFO(...) ::CitrusGL::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CGL_CORE_WARN(...) ::CitrusGL::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CGL_CORE_ERROR(...) ::CitrusGL::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CGL_CORE_FATAL(...) ::CitrusGL::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define CGL_TRACE(...) ::CitrusGL::Log::GetCoreLogger()->traceL(__VA_ARGS__)
#define CGL_INFO(...) ::CitrusGL::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CGL_WARN(...) ::CitrusGL::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CGL_ERROR(...) ::CitrusGL::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CGL_FATAL(...) ::CitrusGL::Log::GetCoreLogger()->fatal(__VA_ARGS__)