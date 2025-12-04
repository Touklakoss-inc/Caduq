#include <Vizir.h>

#pragma once
class LayerManager : public Vizir::Layer
{
public:
	LayerManager() : Vizir::Layer("Layer Manager")
	{	}

	virtual void OnAttach() override;
	virtual void OnDetach() override {}
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;

	void RegisterLayer(const Vizir::Ref<Layer>& layer);
	void RegisterOverlay(const Vizir::Ref<Layer>& overlay);
private:
	void PopLayer(const Vizir::Ref<Layer> layer);
	void PushLayer(const Vizir::Ref<Layer> layer);	
	void PopOverlay(const Vizir::Ref<Layer> overlay);
	void PushOverlay(const Vizir::Ref<Layer> overlay);
private:
	// Layers
	std::vector<Vizir::Ref<Vizir::Layer>> m_RegisteredLayers{};
	std::vector<const char*> m_RegisteredLayerNames{};
	int m_LayerIndex = 0;

	// Overlays
	struct OverlayEntry {
		Vizir::Ref<Vizir::Layer> layer;
		const char const* name;
		bool enabled;
	};
	std::vector<OverlayEntry> m_RegisteredOverlayEntries{};

	int m_PolygonModeIndex = (int)Vizir::RendererAPI::FILL;
	const char* m_PolygonModeStrings[3] = { "Point", "Line", "Fill" };	
};

