#pragma once

// Define logging

#include "Bob.h"

#define CQ_LOGGER_NAME "Caduq"

namespace Caduq 
{
	static void RegisterLogger()
	{
		Bob::Log::RegisterLogger(CQ_LOGGER_NAME, Bob::Log::Level::TRACE);
	}
}

#define CQ_ERROR(...)		BOB_ERROR(CQ_LOGGER_NAME, __VA_ARGS__)
#define CQ_WARN(...)		BOB_WARN(CQ_LOGGER_NAME, __VA_ARGS__)
#define CQ_INFO(...)		BOB_INFO(CQ_LOGGER_NAME, __VA_ARGS__)
#define CQ_DEBUG(...)		BOB_DEBUG(CQ_LOGGER_NAME, __VA_ARGS__)
#define CQ_TRACE(...)		BOB_TRACE(CQ_LOGGER_NAME, __VA_ARGS__)

#define CQ_ASSERT(...)	BOB_ASSERT(CQ_LOGGER_NAME, __VA_ARGS__)