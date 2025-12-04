#pragma once

#include "Window.h"
#include "Vizir/Layers/Layer.h"
#include "Vizir/Layers/LayerStack.h"
#include "Vizir/EventSystem/ApplicationEvent.h"

#include "Vizir/Core/Timestep.h"

#include "Vizir/ImGui/ImGuiLayer.h"

namespace Vizir
{
	class Application
	{
	public:
		Application();
		~Application() = default;

		void OnEvent(Event& e);

		void PushOverlay(const Ref<Layer>& overlay);
		void PushLayer(const Ref<Layer>& layer);

		void PopOverlay(const Ref<Layer>& layer);
		void PopLayer(const Ref<Layer>& layer);

		void Run();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		struct LayerOp {
			enum Type { PushOverlay, PopOverlay, PushLayer, PopLayer };
			Type type;
			Ref<Layer> layer;
		};

		void EnqueueLayerOp(LayerOp layerOp) { m_LayerOpQueue.push(layerOp); }
		void ProcessLayerOps();

		bool OnWindowClose(WindowClosedEvent& e);
		bool OnWindowResize(WindowResizedEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		Ref<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		std::queue<LayerOp> m_LayerOpQueue;
		float m_LastTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

