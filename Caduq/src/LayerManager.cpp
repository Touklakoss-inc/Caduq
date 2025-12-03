#include "LayerManager.h"

#include "imgui/imgui.h"

// --- Layers ---
#include "SandboxCq.h"
#include "SandboxGeo.h"
#include "SandboxSplines.h"

void LayerManager::OnAttach()
{
  m_RegisteredLayers.push_back(new SandboxCq());
  m_RegisteredLayers.push_back(new SandboxGeo());
  m_RegisteredLayers.push_back(new SandboxSplines());

  Vizir::Application::Get().PushLayer(m_RegisteredLayers[0]);
}

void LayerManager::OnUpdate(Vizir::Timestep ts)
{
}

void LayerManager::OnImGuiRender()
{
  int newLayerIndex = m_LayerIndex;

	ImGui::Begin("Layer Manager");

  if (ImGui::Combo("Selected Layer", &newLayerIndex, m_RegisteredLayersName.data(), m_RegisteredLayersName.size())
      && newLayerIndex != m_LayerIndex)
  {
    Layer* lastLayer = m_RegisteredLayers[m_LayerIndex];
    Layer* newLayer = m_RegisteredLayers[newLayerIndex];

    Vizir::Application::Get().PopLayer(lastLayer);
    Vizir::Application::Get().PushLayer(newLayer);

    m_LayerIndex = newLayerIndex;
  }

  if (ImGui::Combo("Polygon Mode", &m_PolygonModeIndex, m_PolygonModeStrings, 3))
  {
    // New item was selected! Update your program state here.
    Vizir::RendererAPI::PolygonMode selected_enum_value = (Vizir::RendererAPI::PolygonMode)m_PolygonModeIndex;
    Vizir::RenderCommand::SetPolygonMode(selected_enum_value);
  }

	ImGui::End();
}
