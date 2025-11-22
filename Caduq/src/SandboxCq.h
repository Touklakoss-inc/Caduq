#pragma once
#include <Vizir.h>

#include "Geometry/Point.h"
#include "Objects/Point.h"
#include "Objects/Spline.h"
#include "Geometry/Spline.h"
#include "Geometry/Patch.h"

class SandboxCq : public Vizir::Layer
{
public:
	SandboxCq() : Layer("Sandbox - Splines"), m_CameraController(1280.0f / 720.0f, true)
	{ }

	virtual void OnAttach() override;
	virtual void OnDetach() override {}
	virtual void OnUpdate(Vizir::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Vizir::Event& e) override;
private:

	// Geometry
    Caduq::Point cp0{ 0.0, 0.0, 0.0 };
    Caduq::Point cp1{ 1.0, 1.0, 0.0 };

    const int MESH_SIZE{ 100 };
    Caduq::Spline cs0{ cp0, Caduq::PointTangency{ { 1.0, 1.0, 0.0 }, 1.0 }, 
                       cp1, Caduq::PointTangency{ { 1.0, 0.0, 0.0 }, 1.0 },
                       MESH_SIZE };

	// Rendering 
	Vizir::OrthographicCameraController m_CameraController;
    Vizir::Ref<Vizir::Shader> m_Shader;

    // 'Uniforms'
    glm::vec3 m_PointColor = { 1.0f, 0.0f, 0.0f };
    float m_PointSize = 5.0f;
    glm::vec3 m_LineColor = { 1.0f, 1.0f, 1.0f };
    float m_LineSize = 1.0f;

    glm::mat4 m_Transform;
};

