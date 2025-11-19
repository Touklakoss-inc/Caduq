#pragma once
#include <Vizir.h>

#include "Geometry/Point.h"
#include "Geometry/Spline.h"
#include "Geometry/Patch.h"

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

	// Geometry
	Geometry::Point p0{ 0.0, 0.0 };
	Geometry::Point p1{ 1.1, 1.1 };

	Geometry::SplinePoint sp0{ p0, { 1.0, 0.0, 0.0 }, 1.0 };
	Geometry::SplinePoint sp1{ p1, { 1.0, 1.0, 0.0 }, 1.0 };

	Geometry::Spline s0{ sp0, sp1 };

	// Rendering 
	Vizir::OrthographicCameraController m_CameraController;

	Vizir::Ref<Vizir::Shader> m_Shader;
	Vizir::Ref<Vizir::VertexArray> m_PointVertexArray;
	Vizir::Ref<Vizir::VertexArray> m_SplineVertexArray;
	Vizir::Ref<Vizir::VertexArray> m_PatchVertexArray;

	// 'Uniforms'
	glm::vec3 m_PointColor = { 1.0f, 0.0f, 0.0f };
	float m_PointSize = 5.0f;

	glm::vec3 m_LineColor = { 1.0f, 1.0f, 1.0f };
	float m_LineWidth = 1.0f;

	glm::mat4 m_Transform;
};

