#pragma once
#include <Vizir.h>

#include "Geometry/Point.h"
#include "Objects/Point.h"
#include "Objects/Spline.h"
#include "Objects/Patch.h"
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
    Caduq::Point cp1{ 3.0, 0.0, 1.0 };
    Caduq::Point cp2{ 3.0, 4.0, 0.0 };
    Caduq::Point cp3{ 0.0, 4.0, 1.0 };

    const int MESH_SIZE{ 10 };
    Caduq::Spline cs0{ cp0, Caduq::PointTangency{ { 1.0, 0.0, 0.0 }, 1.0 }, 
                       cp1, Caduq::PointTangency{ { 1.0, 0.0, 0.0 }, 1.0 },
                       MESH_SIZE };
    Caduq::Spline cs1{ cp2, Caduq::PointTangency{ { 0.0, -1.0, 0.0 }, 1.0 }, 
                       cp1, Caduq::PointTangency{ { 0.0, -1.0, 0.0 }, 1.0 },
                       MESH_SIZE };
    Caduq::Spline cs2{ cp3, Caduq::PointTangency{ { 1.0, 0.0, 0.0 }, 1.0 }, 
                       cp2, Caduq::PointTangency{ { 1.0, 0.0, 0.0 }, 1.0 },
                       MESH_SIZE };
    Caduq::Spline cs3{ cp3, Caduq::PointTangency{ { 0.0, -1.0, 0.0 }, 1.0 }, 
                       cp0, Caduq::PointTangency{ { 0.0, -1.0, 0.0 }, 1.0 },
                       MESH_SIZE };
    Caduq::Patch cc0{ cs2, cs0, cs3, cs1, MESH_SIZE };

	// Rendering 
	Vizir::OrthographicCameraController m_CameraController;
    Vizir::Ref<Vizir::Shader> m_Shader;

    // 'Uniforms'
    glm::vec3 m_PointColor = { 1.0f, 0.0f, 0.0f };
    float m_PointSize = 5.0f;

    glm::vec3 m_LineColor = { 1.0f, 1.0f, 1.0f };
    float m_LineSize = 1.0f;

	glm::vec3 m_TriangleColor = { 0.0f, 0.0f, 1.0f };

    glm::mat4 m_Transform;
};

