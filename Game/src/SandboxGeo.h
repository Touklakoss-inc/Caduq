#pragma once

#include <JudasEngine.h>

class SandboxGeo : public Judas_Engine::Layer
{
public:
	SandboxGeo();
	~SandboxGeo();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Judas_Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Judas_Engine::Event& e) override;
private:
	Judas_Engine::OrthographicCameraController m_CameraController;
};

