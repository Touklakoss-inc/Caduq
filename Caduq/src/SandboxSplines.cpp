#include "SandboxSplines.h"

#include "imgui/imgui.h"

void SandboxSplines::OnAttach()
{

}

void SandboxSplines::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

	Vizir::Renderer::EndScene();
}

void SandboxSplines::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::End();
}

void SandboxSplines::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}