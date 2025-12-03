#include <Vizir.h>

#pragma once
class LayerManager : public Vizir::Layer
{
public:
	LayerManager() : Layer("Base Paremeters")
	{

	}

	virtual void OnAttach() override;
	virtual void OnDetach() override {}
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	std::vector<Layer*> m_RegisteredLayers;
	std::vector<const char*> m_RegisteredLayersName = { "SandboxCq", "SandboxGeo", "SandboxSplines" };
	int m_LayerIndex = 0;

	int m_PolygonModeIndex = (int)Vizir::RendererAPI::FILL;
	const char* m_PolygonModeStrings[3] = { "Point", "Line", "Fill" };	
};

