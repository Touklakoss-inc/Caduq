#pragma once
#include "Vizir.h"

class SandboxFramebuffer : public Vizir::Layer
{
public:
  SandboxFramebuffer();
  ~SandboxFramebuffer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override {}
  virtual void OnUpdate(Vizir::Timestep ts) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Vizir::Event& e) override;
private:
  // Scene
  Vizir::OrthographicCameraController m_CameraController;
  Vizir::ShaderLibrary m_ShaderLibrary;
  Vizir::Ref<Vizir::Texture2D> m_GridTexture, m_LogoTexture;
  Vizir::Ref<Vizir::VertexArray> m_VertexArray;

  float m_GridTilingFactor{ 1.0f };
  glm::vec3 m_GridColor{ 1.0f, 1.0f, 0.5f };

  float m_LogoTilingFactor{ 1.0f };
  glm::vec3 m_LogoColor{ 1.0f, 1.0f, 0.5f };

  // Framebuffer
  Vizir::Ref<Vizir::Framebuffer> m_Framebuffer;
  Vizir::Ref<Vizir::Texture2D> m_ColorAttachment;
  float m_WindowWidth{ 400.0f }, m_WindowHeight{ 400.0f };
};
