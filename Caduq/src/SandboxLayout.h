#pragma once

#include "Vizir.h"

class SandboxLayout : public Vizir::Layer
{
public:
	SandboxLayout();
	~SandboxLayout();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vizir::Event& e) override;
private:
	Vizir::OrthographicCameraController m_CameraController;

	Vizir::Ref<Vizir::Texture2D> m_Texture;
};

