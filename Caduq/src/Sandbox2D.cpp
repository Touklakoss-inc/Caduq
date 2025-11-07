#include "Sandbox2D.h"

#include "Vizir/Platform/OpenGL/OpenGLShader.h"
#include "Vizir/Renderer/Renderer2D.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

Sandbox2D::Sandbox2D() : Layer("MyLayer"), m_CameraController(1280.0f / 720.0f, true)
{ 
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Vizir::Texture2D::Create("src/Assets/textures/texture.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Vizir::Timestep ts)
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

	Vizir::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Vizir::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, glm::vec2(0.5f, 1.0f), glm::radians(22.5f), { m_SquareColor, 1.0f });
	Vizir::Renderer2D::DrawQuad({ 0.0f, 1.0f }, glm::vec2(1.0f, 0.5f), { 0.5f, 0.1f, 0.8, 1.0f });
	Vizir::Renderer2D::DrawRotatedQuad({ -0.5f, 0.0f }, glm::vec2(1.0f, 1.0f), glm::radians(45.0f), m_Texture, 2.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	Vizir::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}