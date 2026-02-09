#pragma once

#include "Application.h"

#if defined(VZ_PLATFORM_WINDOWS) || defined(VZ_PLATFORM_LINUX)

extern Vizir::Application* Vizir::CreateApplication();

int main()
{
	{
		BOB_PROFILE_SESSION_BEGIN("Engine Profiler 1", "startup.json");
		Vizir::Application* app = Vizir::CreateApplication();
		BOB_PROFILE_SESSION_END();

		BOB_PROFILE_SESSION_BEGIN("Engine Profiler 2", "runtime.json");
		app->Run();
		BOB_PROFILE_SESSION_END();

		BOB_PROFILE_SESSION_BEGIN("Engine Profiler 2", "cleanup.json");
		delete app;
		BOB_PROFILE_SESSION_END();
	}
}

#endif
