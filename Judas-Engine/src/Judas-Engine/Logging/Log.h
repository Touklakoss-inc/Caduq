#pragma once

#include "Judas-Engine/Core/Core.h"
#include "spdlog/spdlog.h"

namespace Judas_Engine
{
	class JE_API Log
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

#define JE_CORE_CRITICAL(...)	::Judas_Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define JE_CORE_ERROR(...)	::Judas_Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define JE_CORE_WARN(...)	::Judas_Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define JE_CORE_INFO(...)	::Judas_Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define JE_CORE_TRACE(...)	::Judas_Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define JE_CRITICAL(...)	::Judas_Engine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define JE_ERROR(...)	::Judas_Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define JE_WARN(...)	::Judas_Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define JE_INFO(...)	::Judas_Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define JE_TRACE(...)	::Judas_Engine::Log::GetClientLogger()->trace(__VA_ARGS__)

