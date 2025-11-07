#pragma once

#include <Vizir.h>

class Sandbox2D : public Vizir::Layer
{
public:
	Sandbox2D();
	~Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vizir::Event& e) override;
private:
	Vizir::ShaderLibrary m_ShaderLibrary;

	Vizir::Ref<Vizir::Texture2D> m_Texture;

	Vizir::Ref<Vizir::Shader> m_FlatColorShader;
	Vizir::Ref<Vizir::VertexArray> m_VertexArray;

	Vizir::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.1f, 0.8f };
};

