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

		void PushOverlay(Layer* overlay);
		void PushLayer(Layer* layer);

		void Run();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowClosedEvent& e);
		bool OnWindowResize(WindowResizedEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

