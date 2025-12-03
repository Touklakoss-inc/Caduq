#include "Caduq.h"

#include "Vizir/Core/EntryPoint.h"

// ---- Layers ----
#include "LayerManager.h"


GameApplication::GameApplication()
{
	PushOverlay(new LayerManager());
	// PushLayer(new SandboxSplines());
/*	PushLayer(new SandboxCq());*/
}

GameApplication::~GameApplication()
{

}

Vizir::Application* Vizir::CreateApplication()
{
	return new GameApplication();
}
