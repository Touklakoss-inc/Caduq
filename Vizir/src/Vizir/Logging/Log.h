#pragma once

#include "Vizir/Core/Core.h"

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Vizir
{
	class VZ_API Log
	{
	public :
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define VZ_CORE_CRITICAL(...)	::Vizir::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define VZ_CORE_ERROR(...)	::Vizir::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VZ_CORE_WARN(...)	::Vizir::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VZ_CORE_INFO(...)	::Vizir::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VZ_CORE_TRACE(...)	::Vizir::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define VZ_CRITICAL(...)	::Vizir::Log::GetClientLogger()->critical(__VA_ARGS__)
#define VZ_ERROR(...)	::Vizir::Log::GetClientLogger()->error(__VA_ARGS__)
#define VZ_WARN(...)	::Vizir::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VZ_INFO(...)	::Vizir::Log::GetClientLogger()->info(__VA_ARGS__)
#define VZ_TRACE(...)	::Vizir::Log::GetClientLogger()->trace(__VA_ARGS__)
