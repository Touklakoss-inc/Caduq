#pragma once

#include "Bob/Core.h"

#define FMT_UNICODE 0
#define SPDLOG_USE_STD_FORMAT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Bob
{
	class BOB_API Log
	{
	public :
		enum Level
		{
			TRACE,
			DEBUG,
			INFO,
			WARN,
			ERR,
			CRITICAL,
			OFF,
		};

		static void RegisterLogger(const std::string& name, Level verbosityLevel);

		inline static const std::shared_ptr<spdlog::logger> GetLogger(const std::string& loggerName) { return s_LoggerMap[loggerName]; }
	private:
		static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_LoggerMap;
	};
}

#define BOB_ERROR(name, ...) Bob::Log::GetLogger(name)->error(__VA_ARGS__)
#define BOB_WARN(name, ...)  Bob::Log::GetLogger(name)->warn(__VA_ARGS__)
#define BOB_INFO(name, ...)  Bob::Log::GetLogger(name)->info(__VA_ARGS__)
#define BOB_DEBUG(name, ...) Bob::Log::GetLogger(name)->debug(__VA_ARGS__)
#define BOB_TRACE(name, ...) Bob::Log::GetLogger(name)->trace(__VA_ARGS__)

