#include "LayerManager.h"

#include "imgui/imgui.h"

// --- Layers ---
#include "SandboxCq.h"
#include "SandboxGeo.h"
#include "SandboxSplines.h"

void LayerManager::OnAttach()
{
  RegisterLayer(std::make_shared<SandboxCq>());
  RegisterLayer(std::make_shared<SandboxGeo>());
  RegisterLayer(std::make_shared<SandboxSplines>());

  PushLayer(m_RegisteredLayers[0]);
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
    Vizir::Ref<Layer> lastLayer = m_RegisteredLayers[m_LayerIndex];
    Vizir::Ref<Layer> newLayer = m_RegisteredLayers[newLayerIndex];

    PopLayer(lastLayer);
    PushLayer(newLayer);

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

void LayerManager::RegisterLayer(const Vizir::Ref<Layer>& layer)
{
  m_RegisteredLayers.push_back(layer);
  m_RegisteredLayersName.push_back(layer->GetName().c_str());
}

void LayerManager::PopLayer(const Vizir::Ref<Layer> layer)
{
  VZ_TRACE("--- Popping layer {} ---", layer->GetName().c_str());
  Vizir::Application::Get().PopLayer(layer);
}

void LayerManager::PushLayer(const Vizir::Ref<Layer> layer)
{
  VZ_TRACE("--- Pushing layer {} ---", layer->GetName().c_str());
  Vizir::Application::Get().PushLayer(layer);
}
