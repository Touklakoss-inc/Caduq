#include <Vizir.h>
#include "Vizir/Core/EntryPoint.h"

// ---- Layers ----
#include "SandboxSplines.h"

class GameApplication : public Vizir::Application
{
public:
	GameApplication()
	{
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