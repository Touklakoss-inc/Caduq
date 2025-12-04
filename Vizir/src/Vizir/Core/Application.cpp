#include "vzpch.h"
#include "Application.h"

#include "Core.h"

#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Vizir
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		VZ_PROFILE_FUNC();
		VZ_CORE_ASSERT(!s_Instance, "Another application has already be created !")
		s_Instance = this;

		Log::Init();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = std::make_shared<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& e)
	{
		VZ_PROFILE_FUNC()

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.begin(); it != m_LayerStack.end();)
		{
			(*it)->OnEvent(e);
			it++;
			if (e.handled)
				break;
		}
	}

	void Application::ProcessLayerOps()
	{
		VZ_PROFILE_FUNC()

		while (!m_LayerOpQueue.empty())
		{
			LayerOp layerOp = m_LayerOpQueue.front();
			m_LayerOpQueue.pop();

			switch (layerOp.type)
			{
			case LayerOp::PopLayer:
				m_LayerStack.PopLayer(layerOp.layer);
				break;
			case LayerOp::PushLayer:
				m_LayerStack.PushLayer(layerOp.layer);
				break;
			case LayerOp::PopOverlay:
				m_LayerStack.PopOverlay(layerOp.layer);
				break;
			case LayerOp::PushOverlay:
				m_LayerStack.PushOverlay(layerOp.layer);
				break;
			default :
				VZ_CORE_ERROR("Layer operation Type {} was not recognized", layerOp.type);
			}
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizedEvent& e)
	{
		VZ_PROFILE_FUNC()

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Vizir::Application::PushOverlay(const Ref<Layer>& overlay)
	{
		VZ_PROFILE_FUNC()

		EnqueueLayerOp({ LayerOp::PushOverlay, overlay });
	}

	void Application::PushLayer(const Ref<Layer>& layer)
	{
		VZ_PROFILE_FUNC()

		EnqueueLayerOp({ LayerOp::PushLayer, layer });
	}

	void Application::PopOverlay(const Ref<Layer>& overlay)
	{
		VZ_PROFILE_FUNC()

		EnqueueLayerOp({ LayerOp::PopOverlay, overlay });
	}

	void Application::PopLayer(const Ref<Layer>& layer)
	{
		VZ_PROFILE_FUNC()

		EnqueueLayerOp({ LayerOp::PopLayer, layer });
	}

	void Application::Run()
	{
		VZ_PROFILE_FUNC()

		while (m_Running)
		{
			float time = (float)glfwGetTime(); // Platform/Windows
			Timestep timestep = time - m_LastTime;
			m_LastTime = time;

			if (!m_LayerOpQueue.empty())
				ProcessLayerOps();

			if (!m_Minimized)
			{
				{
					VZ_PROFILE_SCOPE("LayerStack Update")
					for (auto layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);
					}
				}

				{
					VZ_PROFILE_SCOPE("ImGui LayerStack Update")

					m_ImGuiLayer->Begin();
					for (auto layer : m_LayerStack)
					{
						layer->OnImGuiRender();
					}
					m_ImGuiLayer->End();
				}
			}

			m_Window->OnUpdate();
		}
	}
}
