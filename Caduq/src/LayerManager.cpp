#include "LayerManager.h"

#include <imgui/imgui.h>

// --- Layers ---
#include "SandboxCaduq.h"
#include "SandboxTexture.h"
#include "SandboxLayout.h"
#include "SandboxFramebuffer.h"

void LayerManager::OnAttach()
{
  RegisterLayer(std::make_shared<SandboxCaduq>());
  RegisterLayer(std::make_shared<SandboxTexture>());
  RegisterLayer(std::make_shared<SandboxLayout>());
  RegisterLayer(std::make_shared<SandboxFramebuffer>());


  PushLayer(m_RegisteredLayers[0]);
}

void LayerManager::OnUpdate(Vizir::Timestep ts)
{
}

void LayerManager::OnImGuiRender()
{
  ImGui::Begin("Main Parameters");

  if (ImGui::CollapsingHeader("Layer Manager"))
  {
    int newLayerIndex = m_LayerIndex;

    // Render every layer as a choice on the combo
    if (ImGui::Combo("Selected Layer", &newLayerIndex, m_RegisteredLayerNames.data(), static_cast<int>(m_RegisteredLayerNames.size()))
      && newLayerIndex != m_LayerIndex)
    {
      Vizir::Ref<Layer> lastLayer = m_RegisteredLayers[m_LayerIndex];
      Vizir::Ref<Layer> newLayer = m_RegisteredLayers[newLayerIndex];

      PopLayer(lastLayer);
      PushLayer(newLayer);

      m_LayerIndex = newLayerIndex;
    }

    ImGui::Separator();
    
    if (ImGui::CollapsingHeader("Overlays"))
    {
      // Render every overlay as a checkbox
      for (auto& entry : m_RegisteredOverlayEntries)
      {
        // On click on checkbox enable / disable overlay
        if (ImGui::Checkbox(entry.name, &entry.enabled))
        {
          if (!entry.enabled)
            PushOverlay(entry.layer);
          else // entry.enabled
            PopOverlay(entry.layer);
        }
      }
    }
  }

  if (ImGui::CollapsingHeader("Graphics Parameters"))
  {
    // Render every Polygon mode as a choice of the combo
    if (ImGui::Combo("Polygon Mode", &m_PolygonModeIndex, m_PolygonModeStrings, 3))
    {
      // New item was selected! Update your program state here.
      Vizir::RendererAPI::PolygonMode selected_enum_value = (Vizir::RendererAPI::PolygonMode)m_PolygonModeIndex;
      Vizir::RenderCommand::SetPolygonMode(selected_enum_value);
    }
  }

	ImGui::End();
}

void LayerManager::RegisterLayer(const Vizir::Ref<Layer>& layer)
{
  m_RegisteredLayers.push_back(layer);
  m_RegisteredLayerNames.push_back(layer->GetName().c_str());
}

void LayerManager::RegisterOverlay(const Vizir::Ref<Layer>& overlay)
{
  m_RegisteredOverlayEntries.push_back({
    overlay,
    overlay->GetName().c_str(),
    false
  });
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

void LayerManager::PopOverlay(const Vizir::Ref<Layer> overlay)
{
  VZ_TRACE("--- Pushing overlay {} ---", overlay->GetName().c_str());
  Vizir::Application::Get().PushOverlay(overlay);
}

void LayerManager::PushOverlay(const Vizir::Ref<Layer> overlay)
{
  VZ_TRACE("--- Popping overlay {} ---", overlay->GetName().c_str());
  Vizir::Application::Get().PopOverlay(overlay);
}
