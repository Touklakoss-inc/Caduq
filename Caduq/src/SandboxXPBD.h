#pragma once
#include "Vizir.h"

#include "Objects/EntityManager.h"

class SandboxXPBD : public Vizir::Layer
{
public:
	SandboxXPBD() : Layer("Sandbox - Caduq Geometry Test"), m_CameraController(1280.0f / 720.0f, true)
	{ }

	virtual void OnAttach() override;
	virtual void OnDetach() override {}
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vizir::Event& e) override;
private:


    Caduq::EntityManager m_Entity_Manager {};

	// Rendering 
	Vizir::OrthographicCameraController m_CameraController;
    Vizir::Ref<Vizir::Shader> m_Shader;

    // 'Uniforms'
    glm::vec3 m_PointColor = { 1.0f, 0.0f, 0.0f };
    float m_PointSize = 5.0f;

    glm::vec3 m_LineColor = { 1.0f, 1.0f, 1.0f };
    float m_LineSize = 1.0f;

	glm::vec3 m_TriangleColor = { 0.0f, 0.0f, 1.0f };

	glm::mat4 m_Transform = glm::mat4(1.0f);;
};

