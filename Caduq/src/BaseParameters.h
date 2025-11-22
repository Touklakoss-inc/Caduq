#include <Vizir.h>

#pragma once
class BaseParameters : public Vizir::Layer
{
public:
	BaseParameters() : Layer("Base Paremeters")
	{
	}

	virtual void OnAttach() override;
	virtual void OnDetach() override {}
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	int m_PolygonModeIndex = (int)Vizir::RendererAPI::FILL;
	const char* m_PolygonModeStrings[3] = { "Point", "Line", "Fill" };	
};

