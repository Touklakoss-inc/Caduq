#include "SandboxGeo.h"

#include "Judas-Engine/Renderer/Renderer2D.h"
#include "imgui/imgui.h"

#include "Geometry/Point.h"
#include "Geometry/Spline.h"
#include "Tools/FileWriting.h"


SandboxGeo::SandboxGeo() : Layer("MyLayer"), m_CameraController(1280.0f / 720.0f, true)
{ 
}

SandboxGeo::~SandboxGeo()
{
}

void SandboxGeo::OnAttach()
{
    Geometry::Point p0{ 0.0, 0.0 };
    Geometry::Point p1{ 1.1, 1.1 };

    Geometry::SplinePoint sp0{ p0, { 1.0, 0.0, 0.0 }, 1.0 };
    Geometry::SplinePoint sp1{ p1, { 1.0, 1.0, 0.0 }, 1.0 };

    Geometry::Spline s0{ sp0, sp1 };

    const int MESH_SIZE{ 100 };

    Eigen::MatrixXd U{ 4, MESH_SIZE };
    Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(MESH_SIZE, 0.0, 1.0) };

    U.row(0) = Eigen::VectorXd::Ones(MESH_SIZE);
    U.row(1) = u;
    U.row(2) = u*u;
    U.row(3) = u*u*u;

    Geometry::Tools::WriteMesh("mesh.k", s0.Mesh(U));
}

void SandboxGeo::OnDetach()
{
}

void SandboxGeo::OnUpdate(Judas_Engine::Timestep ts)
{
	{
		JE_PROFILE_SCOPE("Camera update");
		m_CameraController.OnUpdate(ts);
	}

	{
		JE_PROFILE_SCOPE("Clear Background");
		Judas_Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Judas_Engine::RenderCommand::Clear();
	}

	Judas_Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Judas_Engine::Renderer2D::EndScene();
}

void SandboxGeo::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::End();
}

void SandboxGeo::OnEvent(Judas_Engine::Event& e)
{
	m_CameraController.OnEvent(e);
}
