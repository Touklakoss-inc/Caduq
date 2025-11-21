#include <Vizir.h>
#include "Vizir/Core/EntryPoint.h"

// ---- Layers ----
#include "BaseParameters.h"
#include "SandboxSplines.h"
#include "SandboxCq.h"

class GameApplication : public Vizir::Application
{
public:
	GameApplication()
	{
		PushOverlay(new BaseParameters());
		// PushLayer(new SandboxSplines());
		PushLayer(new SandboxCq());
	}
	~GameApplication()
	{

	}
private:
};

Vizir::Application* Vizir::CreateApplication()
{
	return new GameApplication();
}
