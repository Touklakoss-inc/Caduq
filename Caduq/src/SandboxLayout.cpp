#include "SandboxLayout.h"

#include "Vizir/Renderer/Renderer2D.h"
#include "imgui/imgui.h"


SandboxLayout::SandboxLayout() : Layer("Sandbox - Layout"), m_CameraController(1280.0f / 720.0f, true)
{
}

SandboxLayout::~SandboxLayout()
{
}

void SandboxLayout::OnAttach()
{
  m_Texture = Vizir::Texture2D::Create("src/Assets/textures/texture.png");
}

void SandboxLayout::OnDetach()
{
}

void SandboxLayout::OnUpdate(Vizir::Timestep ts)
{
  {
    VZ_PROFILE_SCOPE("Camera update");
    m_CameraController.OnUpdate(ts);
  }

  {
    VZ_PROFILE_SCOPE("Clear Background");
    Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Vizir::RenderCommand::Clear();
  }

  Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

  Vizir::Renderer::EndScene();
}

void SandboxLayout::OnImGuiRender()
{
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  VZ_TRACE("Rendering");
  ImGui::Begin("Settings");
  ImGui::Image((void*)m_Texture->GetID(), ImVec2{ 200.0f, 200.0f });
  ImGui::End();
}

void SandboxLayout::OnEvent(Vizir::Event& e)
{
  m_CameraController.OnEvent(e);
}
