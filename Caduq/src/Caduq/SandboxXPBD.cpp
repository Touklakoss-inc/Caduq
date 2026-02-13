#include "cqpch.h"
#include "SandboxXPBD.h"

#include "XPBD/JAttach.h"
#include "Caduq/Objects/Entity.h"
#include "Objects/Point.h"
#include "Objects/Spline.h"
#include "Vizir/Platform/OpenGL/OpenGLShader.h"

void SandboxXPBD::OnAttach()
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

    m_PhyXManager = m_EntityManager.GetPhyXManager();


    m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{0.0, 0.0, 0.0}, m_EntityManager.GetMainFrame(), Caduq::Type::point, Caduq::Point::OptParam{.grounded=true}));
    m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{1.0, 1.0, 0.0}, m_EntityManager.GetMainFrame()));
    m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{1.5, 0.0, 0.0}, m_EntityManager.GetMainFrame()));
    m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{3.5, 0.0, 0.0}, m_EntityManager.GetMainFrame(), Caduq::Type::point, Caduq::Point::OptParam{.grounded=true}));

    m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(0).lock(), Caduq::PointTangency{{0, 0, 0}},
                                                                 m_EntityManager.GetPoint(1).lock(), Caduq::PointTangency{{0, 0, 0}},
                                                                 100, m_EntityManager.GetMainFrame()));              
    m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(2).lock(), Caduq::PointTangency{{0, 0, 0}},
                                                                 m_EntityManager.GetPoint(1).lock(), Caduq::PointTangency{{0, 0, 0}},
                                                                 100, m_EntityManager.GetMainFrame()));              
    m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(3).lock(), Caduq::PointTangency{{0, 0, 0}},
                                                                 m_EntityManager.GetPoint(2).lock(), Caduq::PointTangency{{0, 0, 0}},
                                                                 100, m_EntityManager.GetMainFrame()));              
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(3).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              m_EntityManager.GetPoint(0).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              100));              
    //
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Patch>(m_EntityManager.GetSpline(2).lock(),
    //                                                             m_EntityManager.GetSpline(0).lock(),
    //                                                             m_EntityManager.GetSpline(3).lock(),
    //                                                             m_EntityManager.GetSpline(1).lock(), 
    //                                                             10));

    m_PhyXManager->CreateJoint(std::make_shared<XPBD::JAttach>(m_EntityManager.GetPoint(0).lock()->GetPhyXPoint(), 
                                                               m_EntityManager.GetPoint(1).lock()->GetPhyXPoint(), 
                                                               std::sqrt(2.0), 0.0));
    m_PhyXManager->CreateJoint(std::make_shared<XPBD::JAttach>(m_EntityManager.GetPoint(1).lock()->GetPhyXPoint(), 
                                                               m_EntityManager.GetPoint(2).lock()->GetPhyXPoint(), 
                                                               std::sqrt(1.25), 0.0));
    m_PhyXManager->CreateJoint(std::make_shared<XPBD::JAttach>(m_EntityManager.GetPoint(2).lock()->GetPhyXPoint(), 
                                                               m_EntityManager.GetPoint(3).lock()->GetPhyXPoint(), 
                                                               std::sqrt(4.0), 0.0));

    Vizir::RenderCommand::SetPointSize(m_PointSize);
    Vizir::RenderCommand::SetLineWidth(m_LineSize);
}

void SandboxXPBD::OnUpdate(Vizir::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

    m_PhyXManager->UpdatePhyX(ts.GetSeconds());

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Bind();

    // Render points
    for (const auto& entity : m_EntityManager.GetEntityList()) //GetEntities return by value, is it good ?
    {
        if (entity->GetType() == Caduq::Type::point)
            entity->UpdateGFX();
        entity->Visualize(m_Shader, m_Transform);
    }

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Unbind();

	Vizir::Renderer::EndScene();
}

void SandboxXPBD::OnImGuiRender()
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

    m_EntityManager.RenderImGui();

    ImGui::Separator();

    for (const auto& entity : m_EntityManager.GetEntityList()) 
    {
        entity->RenderImGui(m_EntityManager);
    }

    ImGui::End();

    ImGui::Begin("PhyX");

    m_PhyXManager->RenderImGui();

    if (m_PhyXManager->s_PhyXEnabled)
    {
        for (const auto& joint : m_PhyXManager->GetJointList()) 
        {
            joint->RenderImGui(*m_PhyXManager);
        }
    }

    ImGui::End();

    m_EntityManager.ClearEntityToDelete();
    m_PhyXManager->ClearJointsToDelete();
    
    ImGui::ShowDemoWindow();
}

void SandboxXPBD::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
