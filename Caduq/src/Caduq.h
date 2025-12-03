#pragma once

#include <Vizir.h>

class GameApplication : public Vizir::Application
{
public:
	GameApplication();
	~GameApplication();
private:
	Vizir::Layer* m_Layer;
};