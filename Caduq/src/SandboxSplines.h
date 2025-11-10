#pragma once
#include <Vizir.h>

class SandboxSplines : public Vizir::Layer
{
public:
	SandboxSplines() : Layer("Sandbox - Splines"), m_CameraController(1280.0f / 720.0f, true)
	{ }

	virtual void OnAttach() override;
	virtual void OnDetach() override {}
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vizir::Event& e) override;
private:
	Vizir::OrthographicCameraController m_CameraController;

	std::vector<float> m_Points;
};

