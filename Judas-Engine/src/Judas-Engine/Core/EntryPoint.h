#pragma once

#include "Application.h"
#include "Judas-Engine/Debug/Instrumentor.h"

#if defined(JE_PLATFORM_WINDOWS) || defined(JE_PLATFORM_LINUX)

extern Judas_Engine::Application* Judas_Engine::CreateApplication();

int main()
{
	{
		JE_PROFILE_SESSION_BEGIN("Engine Profiler 1", "startup.json");
		Judas_Engine::Application* app = Judas_Engine::CreateApplication();
		JE_PROFILE_SESSION_END();

		JE_PROFILE_SESSION_BEGIN("Engine Profiler 2", "runtime.json");
		app->Run();
		JE_PROFILE_SESSION_END();

		JE_PROFILE_SESSION_BEGIN("Engine Profiler 2", "cleanup.json");
		delete app;
		JE_PROFILE_SESSION_END();
	}
}

#endif
