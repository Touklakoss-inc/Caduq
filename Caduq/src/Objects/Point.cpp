#include "Point.h"

#include "EntityManager.h"
#include "XPBD/PhyXManager.h"
#include "BobIntegration.h"

#include <Eigen/Core>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>

namespace Caduq
{
    Point::Point(Eigen::Vector3d pos, Type type, OptParam oP)
        : Entity{ oP.name != "" ? oP.name : "Point " + std::to_string(++s_IdGenerator), type }
        , m_Id{ oP.name != "" ? ++s_IdGenerator : s_IdGenerator }
        , m_GeoPoint{ std::make_shared<Geometry::Point>(pos) }
        , m_PhyXPoint{ std::make_shared<XPBD::Point>(m_GeoPoint, oP.mass, m_Name, m_Id, oP.grounded) }
    {
    }

    void Point::Init()
    {
        UpdateGFX();
    }

    void Point::UpdateGFX()
    {
        // Cast to float
        Eigen::Vector3f pointVertice = m_GeoPoint->GetPosition().cast<float>();
        Eigen::Vector<uint32_t, 1> pointIndice{ 0 };

        UpdateGFXBuffer(pointVertice, pointIndice, Vizir::POINTS);

        for (const auto& child : m_Children)
        {
            child->UpdateGFX();
        }
    }

    void Point::Update(double x, double y, double z)
    {
        for (const auto& p : GetParents())
        {
            p->RemoveChild(shared_from_this());
        }

        ClearParents();

        m_GeoPoint->SetPosition(x, y, z);

        Init();

        CQ_INFO("Point modified");
    }

    void Point::Delete()
    {
        for (const auto& pj : m_PhyXPoint->GetParentJoints())
        {
            XPBD::PhyXManager::AddJointToDelete(pj);
        }

        m_PhyXPoint->Delete();
        Entity::Delete();
    }

    void Point::RenderImGui(EntityManager& entityManager)
    {
        ImGuiID id = ImGui::GetID("create_point_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::ColorEdit3("", glm::value_ptr(m_Color));
            ImGui::SameLine();
            if (ImGui::Button("Modify")) 
            {
                Eigen::Vector3f vec = m_GeoPoint->GetPosition().cast<float>();
                SetPopupParam(vec);

                entityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }
            ImGui::SameLine();
            Entity::RenderImGui(entityManager);

            if (XPBD::PhyXManager::s_PhyXEnabled)
            {
                ImGui::Checkbox("Grounded", &m_PhyXPoint->IsGroundedRef());

                if (!m_PhyXPoint->IsGrounded()) 
                    ImGui::InputDouble("Mass [kg]", &m_PhyXPoint->GetMassRef());
            }

            ImGui::TreePop();
        }
    }

    void Point::Popup(EntityManager& entityManager)
    {
        ImGui::InputFloat2("", m_GuiPointPopupCoord);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            entityManager.SetCurEntity(nullptr);
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            if (entityManager.GetCurEntity() == nullptr)
                entityManager.CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{m_GuiPointPopupCoord[0], m_GuiPointPopupCoord[1], m_GuiPointPopupCoord[2]}, Type::point));
            else
            {
                std::dynamic_pointer_cast<Caduq::Point>(entityManager.GetCurEntity())->Update(m_GuiPointPopupCoord[0], m_GuiPointPopupCoord[1], m_GuiPointPopupCoord[2]);

                ImGui::CloseCurrentPopup();
                entityManager.SetCurEntity(nullptr);
            }
        }
        ImGui::EndPopup();
    }
}
