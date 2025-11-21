#include <Vizir.h>
#include "Vizir/Core/EntryPoint.h"

// ---- Layers ----
#include "BaseParameters.h"
#include "SandboxSplines.h"

class GameApplication : public Vizir::Application
{
public:
	GameApplication()
	{
		PushOverlay(new BaseParameters());
		PushLayer(new SandboxSplines());
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