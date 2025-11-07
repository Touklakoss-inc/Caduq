#pragma once

#include <Vizir.h>

class SandboxGeo : public Vizir::Layer
{
public:
	SandboxGeo();
	~SandboxGeo();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vizir::Event& e) override;
private:
	Vizir::OrthographicCameraController m_CameraController;
};

