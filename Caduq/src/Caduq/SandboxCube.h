#pragma once
#include "Vizir.h"

class SandboxCube : public Vizir::Layer
{
public:
  SandboxCube();
  ~SandboxCube() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override {}
  virtual void OnUpdate(Vizir::Timestep ts) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Vizir::Event& e) override;
private:
  Vizir::ArcballCameraController m_CameraController;
  Vizir::ShaderLibrary m_ShaderLibrary;
  Vizir::Ref<Vizir::VertexArray> m_VertexArray;
};
