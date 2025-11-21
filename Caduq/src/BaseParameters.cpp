#include "BaseParameters.h"

#include "imgui/imgui.h"

void BaseParameters::OnAttach()
{
}

void BaseParameters::OnUpdate(Vizir::Timestep ts)
{
}

void BaseParameters::OnImGuiRender()
{
	ImGui::Begin("Base Parameters");

  if (ImGui::Combo("Polygon Mode", &m_PolygonModeIndex, m_PolygonModeStrings, 3))
  {
    // New item was selected! Update your program state here.
    Vizir::RendererAPI::PolygonMode selected_enum_value = (Vizir::RendererAPI::PolygonMode)m_PolygonModeIndex;
    Vizir::RenderCommand::SetPolygonMode(selected_enum_value);
  }

	ImGui::End();
}
