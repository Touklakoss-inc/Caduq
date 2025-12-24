#include "SandboxCq2.h"

#include "Objects/Patch.h"
#include "Objects/Point.h"
#include "Objects/Spline.h"
#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include <memory>
void SandboxCq2::OnAttach()
{
	Vizir::RenderCommand::EnablePrimitiveRestart();

    // Shader 
    std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 i_Position;
            
            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;
            uniform vec3 u_Color;

            out vec3 v_Color;

            void main()
            {
                gl_Position = u_ViewProjection * u_Transform * vec4(i_Position, 1.0);
                v_Color = u_Color;
            }
        )";

    std::string fragmentSrc = R"(
            #version 330 core
            
            in vec3 v_Color;

            layout(location = 0) out vec4 color;

            void main()
            {
                color = vec4(v_Color, 1.0);
            }
        )";

    m_Shader = Vizir::Shader::Create("PointShader", vertexSrc, fragmentSrc);


    // m_Entity_Manager.CreatePoint(std::make_shared<Caduq::Point>(0.0f, 0.0f, 0.0f));
    // m_Entity_Manager.CreatePoint(std::make_shared<Caduq::Point>(3.0f, 0.0f, 1.0f));
    // m_Entity_Manager.CreatePoint(std::make_shared<Caduq::Point>(3.0f, 4.0f, 0.0f));
    // m_Entity_Manager.CreatePoint(std::make_shared<Caduq::Point>(0.0f, 4.0f, 1.0f));

    // m_Entity_Manager.CreateSpline(std::make_shared<Caduq::Spline>(m_Entity_Manager.GetPoint(0), Caduq::PointTangency{{1, 0, 0}},
    //                                                               m_Entity_Manager.GetPoint(1), Caduq::PointTangency{{1, 0, 0}},
    //                                                               100));              
    // m_Entity_Manager.CreateSpline(std::make_shared<Caduq::Spline>(m_Entity_Manager.GetPoint(2), Caduq::PointTangency{{0, -1, 0}},
    //                                                               m_Entity_Manager.GetPoint(1), Caduq::PointTangency{{0, -1, 0}},
    //                                                               10));             
    // m_Entity_Manager.CreateSpline(std::make_shared<Caduq::Spline>(m_Entity_Manager.GetPoint(3), Caduq::PointTangency{{1, 0, 0}},
    //                                                               m_Entity_Manager.GetPoint(2), Caduq::PointTangency{{1, 0, 0}},
    //                                                               10));              
    // m_Entity_Manager.CreateSpline(std::make_shared<Caduq::Spline>(m_Entity_Manager.GetPoint(3), Caduq::PointTangency{{0, -1, 0}},
    //                                                               m_Entity_Manager.GetPoint(0), Caduq::PointTangency{{0, -1, 0}},
    //                                                               10));
    //
    // m_Entity_Manager.CreatePatch(std::make_shared<Caduq::Patch>(m_Entity_Manager.GetSpline(2),
    //                                                             m_Entity_Manager.GetSpline(0),
    //                                                             m_Entity_Manager.GetSpline(3),
    //                                                             m_Entity_Manager.GetSpline(1), 
    //                                                             10));

    // VZ_INFO(m_Entity_Manager.GetPoint(0)->GetChildren().size());
    // VZ_INFO(m_Entity_Manager.GetSpline(0)->GetParents().size());
    // VZ_INFO(m_Entity_Manager.GetPoint(0).use_count());
    // VZ_INFO(m_Entity_Manager.GetPointList().size());
    // VZ_INFO(m_Entity_Manager.GetSplineList().size());
    // m_Entity_Manager.DeleteSpline(m_Entity_Manager.GetSpline(0));
    // VZ_INFO(m_Entity_Manager.GetSplineList().size());

    // std::cout << m_Entity_Manager.GetPoint(0) << '\n';
    // std::cout << &*(m_Entity_Manager.GetPoint(0)) << '\n';

    Vizir::RenderCommand::SetPointSize(m_PointSize);
	Vizir::RenderCommand::SetLineWidth(m_LineSize);

    // Transform
    m_Transform = glm::mat4(1.0f);
}

void SandboxCq2::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Bind();

    // Render points
    for (const auto& point : m_Entity_Manager.GetPointList()) //GetEntities return by value, is it good ?
    {
        point->Visualize(m_Shader, m_Transform);
    }

    for (const auto& spline : m_Entity_Manager.GetSplineList()) //GetEntities return by value, is it good ?
    {
        spline->Visualize(m_Shader, m_Transform);
    }

    for (const auto& patch : m_Entity_Manager.GetPatchList()) //GetEntities return by value, is it good ?
    {
        patch->Visualize(m_Shader, m_Transform);
    }

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Unbind();

	Vizir::Renderer::EndScene();
}

void SandboxCq2::OnImGuiRender()
{
    float pointSize = m_PointSize;
    float lineSize = m_LineSize;

    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);


    ImGui::Begin("Settings");

    ImGui::DragFloat("Point Size", &pointSize, 1.0f, 1.0f, 25.0f);
    ImGui::DragFloat("Line Size", &lineSize, 1.0f, 1.0f, 10.0f);

    ImGui::End();

    if (pointSize != m_PointSize)
    {
        m_PointSize = pointSize;
        Vizir::RenderCommand::SetPointSize(m_PointSize);
    }
    if (lineSize != m_LineSize)
    {
        m_LineSize = lineSize;
        Vizir::RenderCommand::SetLineWidth(m_LineSize);
    }

    ImGui::Begin("Hierarchy");

    m_Entity_Manager.RenderImGui();

    ImGui::Separator();

    for (const auto& point : m_Entity_Manager.GetPointList()) 
    {
        point->RenderImGui(m_Entity_Manager);
    }

    for (const auto& spline : m_Entity_Manager.GetSplineList()) 
    {
        spline->RenderImGui(m_Entity_Manager);
    }

    for (const auto& patch : m_Entity_Manager.GetPatchList()) 
    {
        patch->RenderImGui(m_Entity_Manager);
    }

    ImGui::End();
    
    ImGui::ShowDemoWindow();
}

void SandboxCq2::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
