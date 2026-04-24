#include "Eigen/Core"
#include "cqpch.h"
#include "SandboxXPBD.h"
#include "XPBD/JointsBuildingBlocks.h"

#include "Caduq/Objects/Entity.h"
#include "Objects/Point.h"
#include "Objects/Part.h"
#include "Objects/Spline.h"
#include "Objects/StlEntity.h"
#include "Vizir/Platform/OpenGL/OpenGLShader.h"
#include "tracy/Tracy.hpp"

#include <memory>
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


    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{0.0, 0.0, 0.0}, m_EntityManager.GetMainFrame(), Caduq::Type::point, Caduq::Point::OptParam{.grounded=true}));
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{1.0, 0.0, 0.0}, m_EntityManager.GetMainFrame()));
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{2.0, 0.0, 0.0}, m_EntityManager.GetMainFrame()));
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{3.5, 0.0, 0.0}, m_EntityManager.GetMainFrame(), Caduq::Type::point, Caduq::Point::OptParam{.grounded=true}));

    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(0).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              m_EntityManager.GetPoint(1).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              100, m_EntityManager.GetMainFrame()));              
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(1).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              m_EntityManager.GetPoint(2).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              100, m_EntityManager.GetMainFrame()));              
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(2).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              m_EntityManager.GetPoint(1).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              100, m_EntityManager.GetMainFrame()));              
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(3).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              m_EntityManager.GetPoint(2).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              100, m_EntityManager.GetMainFrame()));              
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(m_EntityManager.GetPoint(3).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              m_EntityManager.GetPoint(0).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              100));              
    //
    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Patch>(m_EntityManager.GetSpline(2).lock(),
    //                                                             m_EntityManager.GetSpline(0).lock(),
    //                                                             m_EntityManager.GetSpline(3).lock(),
    //                                                             m_EntityManager.GetSpline(1).lock(), 
    //                                                             10));

    m_EntityManager.CreateEntity(std::make_shared<Caduq::Part>(Geometry::Transform::Identity(), m_EntityManager.GetMainFrame(), Caduq::Part::OptParam{.mass = 1.0, .inertiaTensor = {2.0/3.0, 2.0/3.0, 2.0/3.0}, .grounded=false}));

    const auto pt1 = m_EntityManager.GetPart(0).lock();

    pt1->GetMainFrame()->Update(Eigen::Vector3d(-0.025, 2.2, 0.0), Eigen::Quaterniond::Identity());
    pt1->GetEntityManager().CreateEntity(std::make_shared<Caduq::StlEntity>("littlecube.stl", pt1->GetMainFrame()));
    pt1->GetEntityManager().CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d(0.025, 0.05, 0.0), pt1->GetMainFrame()));
    pt1->GetEntityManager().CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d(0.05, -0.05, 0.0), pt1->GetMainFrame()));

    m_EntityManager.CreateEntity(std::make_shared<Caduq::Part>(Geometry::Transform::Identity(), m_EntityManager.GetMainFrame(), Caduq::Part::OptParam{.mass = 1.0, .inertiaTensor = {2.0/3.0, 2.0/3.0, 2.0/3.0}, .grounded=true}));

    m_EntityManager.GetPart(1).lock()->GetMainFrame()->Update(Eigen::Vector3d(0.0, 2.5, 0.0), Eigen::Quaterniond::Identity());
    m_PhyXManager->CreateJoint(m_EntityManager.GetPart(0).lock()->GetPhyXPart(),
                               nullptr,
                               XPBD::JAttach{ .dRest = 0.25, .alpha = 0.001, 
                               .pos1 = m_EntityManager.GetPart(0).lock()->GetEntityManager().GetPoint(0).lock()->GetGeoPoint()->GetPosition(), 
                               .pos2 = { 0.0, 2.5, 0.0 }});
    m_EntityManager.CreateEntity(std::make_shared<Caduq::Part>(Geometry::Transform::Identity(), m_EntityManager.GetMainFrame(), Caduq::Part::OptParam{.mass = 1.0, .inertiaTensor = {2.0/3.0, 2.0/3.0, 2.0/3.0}, .grounded=false}));

    const auto pt2 = m_EntityManager.GetPart(2).lock();
    pt2->GetEntityManager().CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d(0.05, 0.05, 0.0), pt2->GetMainFrame()));
    pt2->GetMainFrame()->Update(Eigen::Vector3d(-0.05, 2.0, 0.0), Eigen::Quaterniond::Identity());
    m_PhyXManager->CreateJoint(pt1->GetPhyXPart(),
                               pt2->GetPhyXPart(),
                               XPBD::JAttach{ .dRest = 0.1, .alpha = 0.001, 
                               .pos1 = pt1->GetEntityManager().GetPoint(1).lock()->GetGeoPoint()->GetPosition(), 
                               .pos2 = pt2->GetEntityManager().GetPoint(0).lock()->GetGeoPoint()->GetPosition()});

    pt2->GetEntityManager().CreateEntity(std::make_shared<Caduq::StlEntity>("littlecube.stl", pt2->GetMainFrame()));

    // m_PhyXManager->CreateJoint(pt2->GetPhyXPart(),
    //                            nullptr,
    //                            XPBD::JRestrictAxis{ .axis = { 1.0, 1.0, 0.0 },
    //                            .pos1 = { 0.0, 0.0, 0.0 },
    //                            .pos2 = { 0.0, 2.5, 0.0 }, 
    //                            .posMin = -10,
    //                            .posMax =  10,
    //                            .alpha = 0.001});
    m_PhyXManager->CreateJoint(pt2->GetPhyXPart(),
                               nullptr,
                               XPBD::JLimitAngle{ 
                               .n = { 0.0, 0.0, 1.0 },
                               .a1 = { 0.0, 1.0, 0.0 },
                               .a2 = { 0.0, 1.0, 0.0 }, 
                               .phiMin = -10.0,
                               .phiMax =  10.0,
                               .alpha = 1.0});

    m_EntityManager.CreateEntity(std::make_shared<Caduq::Part>(Geometry::Transform::Identity(), m_EntityManager.GetMainFrame(), Caduq::Part::OptParam{.mass = 1.0, .inertiaTensor = {2.0/3.0, 2.0/3.0, 2.0/3.0}, .grounded=false}));

    const auto pt3 = m_EntityManager.GetPart(3).lock();

    pt3->GetMainFrame()->Update(Eigen::Vector3d(0.5, 2.2, 0.0), Eigen::Quaterniond::Identity());
    pt3->GetEntityManager().CreateEntity(std::make_shared<Caduq::StlEntity>("littlecube.stl", pt3->GetMainFrame()));

    m_PhyXManager->CreateJoint(pt3->GetPhyXPart(),
                               nullptr,
                               XPBD::JAlignTwoAxes{ 
                               .a1 = { 1.0, 1.0, 0.0 },
                               .a2 = { 1.0, 0.0, 0.0 },
                               .alpha = 0.001});

    // m_EntityManager.CreateEntity(std::make_shared<Caduq::Spline>(pt1->GetEntityManager().GetPoint(1).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              pt2->GetEntityManager().GetPoint(0).lock(), Caduq::PointTangency{{0, 0, 0}},
    //                                                              100, m_EntityManager.GetMainFrame()));              
    // m_PhyXManager->CreateJoint(std::make_shared<XPBD::JAttach>(m_EntityManager.GetPoint(1).lock()->GetPhyXPoint(), 
    //                                                            m_EntityManager.GetPoint(2).lock()->GetPhyXPoint(), 
    //                                                            std::sqrt(1.25), 0.0));
    // m_PhyXManager->CreateJoint(std::make_shared<XPBD::JAttach>(m_EntityManager.GetPoint(2).lock()->GetPhyXPoint(), 
    //                                                            m_EntityManager.GetPoint(3).lock()->GetPhyXPoint(), 
                                                               // std::sqrt(4.0), 0.0));

    Vizir::RenderCommand::SetPointSize(m_PointSize);
    Vizir::RenderCommand::SetLineWidth(m_LineSize);
}

void SandboxXPBD::OnUpdate(Vizir::Timestep ts)
{
    ZoneScoped;
	m_CameraController.OnUpdate(ts);

    m_PhyXManager->UpdatePhyX(ts.GetSeconds());

	Vizir::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Vizir::RenderCommand::Clear();

	Vizir::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Bind();

    // Render points
    for (const auto& entity : m_EntityManager.GetEntityList()) //GetEntities return by value, is it good ?
    {
        if (entity->GetType() == Caduq::Type::part && m_PhyXManager->s_PhyXEnabled)
            entity->UpdateGFX();
        entity->Visualize(m_Shader, m_Transform);
    }

    std::dynamic_pointer_cast<Vizir::OpenGLShader>(m_Shader)->Unbind();

	Vizir::Renderer::EndScene();
    FrameMark;
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

    ImGui::End();

    m_EntityManager.ClearEntityToDelete();
    
    ImGui::ShowDemoWindow();
}

void SandboxXPBD::OnEvent(Vizir::Event& e)
{
	m_CameraController.OnEvent(e);
}
