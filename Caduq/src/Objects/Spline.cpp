#include "Spline.h"

#include "Point.h"
#include "Geometry/Spline.h"
#include "EntityManager.h"
#include "BobIntegration.h"
#include "MyImGui.h"

#include <Eigen/Core>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Caduq
{
    Spline::Spline(const std::shared_ptr<Point>& startPoint, PointTangency startTangency, 
                   const std::shared_ptr<Point>& endPoint, PointTangency endTangency,
                   int mesh_size, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Spline " + std::to_string(++s_IdGenerator), type }
        , m_Id{ name != "" ? ++s_IdGenerator : s_IdGenerator }, m_mesh_size{ mesh_size }
        , m_StartPoint{ startPoint }, m_StartTangency{ startTangency }
        , m_EndPoint{ endPoint }, m_EndTangency{ endTangency }
    {
    }

    void Spline::Init()
    {
        // Add the child/parent relationship
        AddParent(m_StartPoint);
        m_StartPoint->AddChild(shared_from_this());

        AddParent(m_EndPoint);
        m_EndPoint->AddChild(shared_from_this());

        UpdateGFX();
    }

    void Spline::UpdateGFX()
    {
        m_Spline = {Geometry::SplinePoint{ *m_StartPoint->GetGeoPoint(), m_StartTangency.tangent, m_StartTangency.tension },
                    Geometry::SplinePoint{ *m_EndPoint->GetGeoPoint(), m_EndTangency.tangent, m_StartTangency.tension } };
        // Create spline mesh
        Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(m_mesh_size, 0.0, 1.0) };
        Eigen::MatrixXd U0 = m_Spline.Mesh(u, m_mesh_size);
        Geometry::Mesh mesh = m_Spline.GetGfxMesh();

        // Cast points to float
        Eigen::MatrixXf splineVertices = mesh.nodes.cast<float>();
        Eigen::VectorX<uint32_t> splineIndices = mesh.elts;

        UpdateGFXBuffer(splineVertices, splineIndices, Vizir::LINE_STRIP);

        for (const auto& child : m_Children)
        {
            child->UpdateGFX();
        }
    }

    void Spline::Update(const std::shared_ptr<Point>& startPoint, PointTangency startTangency, 
                        const std::shared_ptr<Point>& endPoint, PointTangency endTangency)
    {
        for (const auto& p : GetParents())
        {
            p->RemoveChild(shared_from_this());
        }

        ClearParents();

        m_StartPoint = startPoint;
        m_StartTangency = startTangency;
        m_EndPoint = endPoint;
        m_EndTangency = endTangency;
        Init();

        CQ_INFO("Spline modified");
    }

    void Spline::RenderImGui(EntityManager& entityManager)
    {
        ImGuiID id = ImGui::GetID("create_spline_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::ColorEdit3("", glm::value_ptr(m_Color));
            ImGui::SameLine();
            if (ImGui::Button("Modify")) 
            {
                SetPopupParam(entityManager, m_Spline.GetStartPoint(), m_StartPoint->GetID(), m_Spline.GetEndPoint(), m_EndPoint->GetID());

                entityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }

            ImGui::SameLine();
            Entity::RenderImGui(entityManager);

            ImGui::TreePop();
        }
    }

    void Spline::SplinePopup(EntityManager& entityManager)
    {
        MyCombo("Start Point", entityManager.GetPointList(), m_GuiStartPointID);
        ImGui::InputFloat2("Start Tangent Vector", m_GuiStartTangent);
        ImGui::InputFloat("Start Tension", m_GuiStartTension);

        ImGui::Separator();

        MyCombo("End Point", entityManager.GetPointList(), m_GuiEndPointID);
        ImGui::InputFloat2("End Tangent Vector", m_GuiEndTangent);
        ImGui::InputFloat("End Tension", m_GuiEndTension);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            entityManager.SetCurEntity(nullptr);
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            // Check if all 4 selected splines are different
            if (m_GuiStartPointID != m_GuiEndPointID)
            {
                if (entityManager.GetCurEntity() == nullptr)
                {
                    entityManager.CreateEntity(std::make_shared<Caduq::Spline>(entityManager.GetPoint(m_GuiStartPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiStartTangent[0], m_GuiStartTangent[1], 
                                                                                       m_GuiStartTangent[2]}, m_GuiStartTension[0]},
                                                                 entityManager.GetPoint(m_GuiEndPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiEndTangent[0], m_GuiEndTangent[1],
                                                                                       m_GuiEndTangent[2]}, m_GuiEndTension[0]},
                                                                 100, Type::spline));              
                }
                else
                {
                    std::dynamic_pointer_cast<Caduq::Spline>(entityManager.GetCurEntity())->Update(
                                                                 entityManager.GetPoint(m_GuiStartPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiStartTangent[0], m_GuiStartTangent[1], 
                                                                                       m_GuiStartTangent[2]}, m_GuiStartTension[0]},
                                                                 entityManager.GetPoint(m_GuiEndPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiEndTangent[0], m_GuiEndTangent[1],
                                                                                       m_GuiEndTangent[2]}, m_GuiEndTension[0]});

                }

                ImGui::CloseCurrentPopup();
                entityManager.SetCurEntity(nullptr);
            }
            else
                VZ_WARN("Select two different points to create a spline");
        }
        ImGui::EndPopup();
    }

    void Spline::SetPopupParam(EntityManager& entityManager, Geometry::SplinePoint startPoint, int startPointID, Geometry::SplinePoint endPoint, int endPointID)
    {
        for (int i = 0; i < 3; i++)
            m_GuiStartTangent[i] = startPoint.tangent[i];
        m_GuiStartTension[0] = static_cast<float>(startPoint.tension);

        for (int i = 0; i < 3; i++)
            m_GuiEndTangent[i] = endPoint.tangent[i];
        m_GuiEndTension[0] = static_cast<float>(endPoint.tension);

        for (int i = 0; i < entityManager.GetPointList().size(); i++)
        {
            auto curPointID = entityManager.GetPoint(i).lock()->GetID();
            if (curPointID == startPointID)
                m_GuiStartPointID = i;

            if (curPointID == endPointID)
                m_GuiEndPointID = i;

        }
    }
}
