#include "SandboxGeo.h"

#include "Vizir/Renderer/Renderer2D.h"
#include "imgui/imgui.h"

#include "Geometry/Point.h"
#include "Geometry/Spline.h"
#include "Geometry/Patch.h"
#include "Tools/FileWriting.h"


SandboxGeo::SandboxGeo() : Layer("MyLayer"), m_CameraController(1280.0f / 720.0f, true)
{ 
}

SandboxGeo::~SandboxGeo()
{
}

void SandboxGeo::OnAttach()
{
    Geometry::Point p0{ 0.0, 0.0, 0.0 };
    Geometry::Point p1{ 3.0, 0.0, 1.0 };
    Geometry::Point p2{ 3.0, 4.0, 0.0 };
    Geometry::Point p3{ 0.0, 4.0, 1.0 };

    Geometry::SplinePoint sp0{ p0, { 1.0, 0.0, 0.0 }, 1.0 };
    Geometry::SplinePoint sp1{ p1, { 1.0, 0.0, 0.0 }, 1.0 };
    Geometry::Spline s0{ sp0, sp1 };

    Geometry::SplinePoint sp2{ p2, { 0.0, -1.0, 0.0 }, 1.0 };
    Geometry::SplinePoint sp3{ p1, { 0.0, -1.0, 0.0 }, 1.0 };
    Geometry::Spline s1{ sp2, sp3 };

    Geometry::SplinePoint sp4{ p3, { 1.0, 0.0, 0.0 }, 1.0 };
    Geometry::SplinePoint sp5{ p2, { 1.0, 0.0, 0.0 }, 1.0 };
    Geometry::Spline s2{ sp4, sp5 };

    Geometry::SplinePoint sp6{ p3, { 0.0, -1.0, 0.0 }, 1.0 };
    Geometry::SplinePoint sp7{ p0, { 0.0, -1.0, 0.0 }, 1.0 };
    Geometry::Spline s3{ sp6, sp7 };

    const int MESH_SIZE{ 10 };
    Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(MESH_SIZE, 0.0, 1.0) };
    s0.Mesh(u, MESH_SIZE);
    s1.Mesh(u, MESH_SIZE);
    s2.Mesh(u, MESH_SIZE);
    s3.Mesh(u, MESH_SIZE);

    Geometry::Tools::WriteMesh("mesh.k", { s0, s1, s2, s3 });

    Geometry::Patch c0{ s2, s0, s3, s1 };
    Eigen::MatrixXd m = c0.Mesh(u, u, MESH_SIZE);
    // std::cout << m;
    // std::cout << c0.GetGfxMesh().elts << '\n';
    Geometry::Tools::WriteNodes("nodes.k", m);
    Geometry::Tools::WriteSurface("surf.k", c0);
    Geometry::Tools::WriteGfxSurface("surfGfx.k", c0);
}

void SandboxGeo::OnDetach()
{
}

void SandboxGeo::OnUpdate(Vizir::Timestep ts)
{
	{
		VZ_PROFILE_SCOPE("Camera update");
		m_CameraController.OnUpdate(ts);
	}

	{
		VZ_PROFILE_SCOPE("Clear Background");
		Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vizir::RenderCommand::Clear();
	}

	Vizir::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Vizir::Renderer2D::EndScene();
}

void SandboxGeo::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::End();
}

void SandboxGeo::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
