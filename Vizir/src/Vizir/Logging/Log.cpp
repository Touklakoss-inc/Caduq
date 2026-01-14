#include "vzpch.h"
#include "Logging/Log.h"

namespace Vizir
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Judas Engine");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("Game");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}