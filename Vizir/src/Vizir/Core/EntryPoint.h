#pragma once

#include "Application.h"
#include "Vizir/Debug/Instrumentor.h"

#if defined(VZ_PLATFORM_WINDOWS) || defined(VZ_PLATFORM_LINUX)

extern Vizir::Application* Vizir::CreateApplication();

int main()
{
	{
		VZ_PROFILE_SESSION_BEGIN("Engine Profiler 1", "startup.json");
		Vizir::Application* app = Vizir::CreateApplication();
		VZ_PROFILE_SESSION_END();

		VZ_PROFILE_SESSION_BEGIN("Engine Profiler 2", "runtime.json");
		app->Run();
		VZ_PROFILE_SESSION_END();

		VZ_PROFILE_SESSION_BEGIN("Engine Profiler 2", "cleanup.json");
		delete app;
		VZ_PROFILE_SESSION_END();
	}
}

#endif
