#include "Log.h"

namespace Bob
{
	std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Log::s_LoggerMap;

	spdlog::level::level_enum GetNativeLevel(Log::Level verbosityLevel)
	{
		switch (verbosityLevel)
		{
		case Log::Level::TRACE:
			return spdlog::level::trace;
		case Log::Level::DEBUG:
			return spdlog::level::debug;
		case Log::Level::INFO:
			return spdlog::level::info;
		case Log::Level::WARN:
			return spdlog::level::warn;
		case Log::Level::ERR:
			return spdlog::level::err;
		case Log::Level::CRITICAL:
			return spdlog::level::critical;
		case Log::Level::OFF:
			return spdlog::level::off;
		}
	}

	void Log::RegisterLogger(const std::string& name, Level verbosityLevel)
	{
		if (s_LoggerMap.contains(name))
		{
			throw std::runtime_error("Logger map already contains entry for given name.");
		}

		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_LoggerMap[name] = spdlog::stdout_color_mt(name);
		s_LoggerMap[name]->set_level(GetNativeLevel(verbosityLevel));
	}
}
