#pragma once

// Define logging

#include "Bob.h"

#define VZ_LOGGER_NAME "Vizir"

namespace Vizir
{
	static void RegisterLogger()
	{
		Bob::Log::RegisterLogger(VZ_LOGGER_NAME, Bob::Log::Level::TRACE);
	}
}

#define VZ_ERROR(...)		BOB_ERROR(VZ_LOGGER_NAME, __VA_ARGS__)
#define VZ_WARN(...)		BOB_WARN(VZ_LOGGER_NAME, __VA_ARGS__)
#define VZ_INFO(...)		BOB_INFO(VZ_LOGGER_NAME, __VA_ARGS__)
#define VZ_DEBUG(...)		BOB_DEBUG(VZ_LOGGER_NAME, __VA_ARGS__)
#define VZ_TRACE(...)		BOB_TRACE(VZ_LOGGER_NAME, __VA_ARGS__)

#define VZ_ASSERT(...)	BOB_ASSERT(VZ_LOGGER_NAME, __VA_ARGS__)