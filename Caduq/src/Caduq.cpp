#include "Caduq.h"

#include "Vizir/Core/EntryPoint.h"

// ---- Layers ----
#include "LayerManager.h"


GameApplication::GameApplication()
{
	PushOverlay(std::make_shared<LayerManager>());
}

GameApplication::~GameApplication()
{

}

Vizir::Application* Vizir::CreateApplication()
{
	return new GameApplication();
}
