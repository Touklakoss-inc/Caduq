#include "Caduq.h"

#include "Vizir/Core/EntryPoint.h"

#include "BobIntegration.h"

// ---- Layers ----
#include "LayerManager.h"


GameApplication::GameApplication()
{
	Caduq::RegisterLogger();
	PushOverlay(std::make_shared<LayerManager>());
}

GameApplication::~GameApplication()
{

}

Vizir::Application* Vizir::CreateApplication()
{
	return new GameApplication();
}
