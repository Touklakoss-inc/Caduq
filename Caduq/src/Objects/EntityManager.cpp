#include "EntityManager.h"

#include "BobIntegration.h"
#include "Entity.h"
#include "Point.h"
#include "Spline.h"
#include "Patch.h"

#include <Eigen/Core>
#include <imgui.h>

namespace Caduq 
{
    EntityManager::EntityManager()
        : m_PhyXManager{ std::make_shared<XPBD::PhyXManager>() }
    {
    }

    // /!\ recursive function, maybe change ?
    void EntityManager::DeleteEntity(const std::shared_ptr<Entity>& entity)
    {
        m_EntityToDelete.push_back(entity);
        for (const auto& c : entity->GetChildren())
        {
            DeleteEntity(c);
        }
    }

    void EntityManager::ClearEntityToDelete()
    {
        for (const auto& entity : m_EntityToDelete)
        {
            if (entity->GetType() == Type::point)
                m_PhyXManager->RemovePhyXPointFromList(std::dynamic_pointer_cast<Point>(entity)->GetPhyXPoint());

            entity->Delete();

            auto it = std::find(m_EntityList.begin(), m_EntityList.end(), entity);
            if (it != m_EntityList.end())
            {
                m_EntityList.erase(it);
            }

            CQ_WARN(entity->GetName() + " removed from list");
        }

        m_EntityToDelete.clear();

        CleanWeakPtrList(m_PointList);
        CleanWeakPtrList(m_SplineList);
        CleanWeakPtrList(m_PatchList);
    }

    void EntityManager::RenderImGui()
    {
        // Point Creation
        if (ImGui::Button("Create Point"))
        {
            Point::SetPopupParam(Eigen::Vector3f::Zero());

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_point_popup");
        }

        ImGui::SameLine();
        // Spline Creation
        if (ImGui::Button("Create Spline"))
        {
            Spline::SetPopupParam(*this, Geometry::SplinePoint{}, m_PointList.at(0).lock()->GetID(), Geometry::SplinePoint{}, m_PointList.at(0).lock()->GetID());

            m_CurEntity = nullptr;
            if (!m_PointList.empty())
                ImGui::OpenPopup("create_spline_popup");
            else
                VZ_WARN("You need to create some points first");
        }

        ImGui::SameLine();
        // Patch4 Creation
        if (ImGui::Button("Create Patch4"))
        {
            Patch::SetPopupParam(*this, m_PointList.at(0).lock()->GetID(),
                                             m_PointList.at(0).lock()->GetID(),
                                             m_PointList.at(0).lock()->GetID(),
                                             m_PointList.at(0).lock()->GetID());
            m_CurEntity = nullptr;
            if (!m_SplineList.empty())
                ImGui::OpenPopup("create_patch_popup");
            else
                VZ_WARN("You need to create some splines first");
        }

        ImGui::SameLine();
        // Patch3 Creation
        if (ImGui::Button("Create Patch3"))
        {
            Patch::SetPopupParam(*this, m_PointList.at(0).lock()->GetID(),
                                             m_PointList.at(0).lock()->GetID(),
                                             m_PointList.at(0).lock()->GetID(),
                                             -1);

            m_CurEntity = nullptr;
            if (!m_SplineList.empty())
                ImGui::OpenPopup("create_patch_popup");
            else
                VZ_WARN("You need to create some splines first");
        }

        if (ImGui::BeginPopup("create_point_popup"))
            Point::Popup(*this); 

        if (ImGui::BeginPopup("create_spline_popup"))
            Spline::SplinePopup(*this); 

        if (ImGui::BeginPopup("create_patch_popup"))
            Patch::PatchPopup(*this);
    }
}
