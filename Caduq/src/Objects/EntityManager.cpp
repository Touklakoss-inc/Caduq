#include "EntityManager.h"

#include "BobIntegration.h"
#include "Entity.h"
#include "Point.h"
#include "Spline.h"
#include "Patch.h"
#include "Frame.h"
#include "Part.h"
#include "StlEntity.h"

#include <Eigen/Core>
#include <imgui.h>
#include <memory>

namespace Caduq 
{
    EntityManager::EntityManager(const std::shared_ptr<Frame>& frame)
        : m_MainFrame{ frame }
        , m_PhyXManager{ std::make_shared<XPBD::PhyXManager>() }
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
        ClearEntityToDelete();

        // Frame Creation
        if (ImGui::Button("Frame"))
        {
            Frame::SetPopupParam(Eigen::Vector3d::Zero(), Eigen::Vector4d::Zero());

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_frame_popup");
        }

        ImGui::SameLine();
        // Point Creation
        if (ImGui::Button("Point"))
        {
            Point::SetPopupParam(Eigen::Vector3f::Zero());

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_point_popup");
        }

        ImGui::SameLine();
        // Spline Creation
        if (ImGui::Button("Spline"))
        {
            if (!m_PointList.empty())
            {
                Spline::SetPopupParam(*this, Geometry::SplinePoint{}, m_PointList.at(0).lock()->GetID(), 
                                             Geometry::SplinePoint{}, m_PointList.at(0).lock()->GetID());

                m_CurEntity = nullptr;
                ImGui::OpenPopup("create_spline_popup");
            }
            else
                VZ_WARN("You need to create some points first");
        }

        ImGui::SameLine();
        // Patch4 Creation
        if (ImGui::Button("Patch4"))
        {
            if (!m_SplineList.empty())
            {
                Patch::SetPopupParam(*this, m_SplineList.at(0).lock()->GetID(),
                                            m_SplineList.at(0).lock()->GetID(),
                                            m_SplineList.at(0).lock()->GetID(),
                                            m_SplineList.at(0).lock()->GetID());
                m_CurEntity = nullptr;
                ImGui::OpenPopup("create_patch_popup");
            }
            else
                VZ_WARN("You need to create some splines first");
        }

        ImGui::SameLine();
        // Patch3 Creation
        if (ImGui::Button("Patch3"))
        {
            if (!m_SplineList.empty())
            {
                Patch::SetPopupParam(*this, m_SplineList.at(0).lock()->GetID(),
                                            m_SplineList.at(0).lock()->GetID(),
                                            m_SplineList.at(0).lock()->GetID(),
                                            -1);

                m_CurEntity = nullptr;
                ImGui::OpenPopup("create_patch_popup");
            }
            else
                VZ_WARN("You need to create some splines first");
        }

        ImGui::SameLine();
        // Part Creation
        if (ImGui::Button("Part"))
        {
            Part::SetPopupParam(Eigen::Vector3d::Zero(), Eigen::Vector4d::Zero());

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_part_popup");
        }

        ImGui::SameLine();
        // StlEntity Creation
        if (ImGui::Button("Stl"))
        {
            StlEntity::SetPopupParam("");

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_stlEntity_popup");
        }

        if (ImGui::BeginPopup("create_frame_popup"))
            Frame::Popup(*this);

        if (ImGui::BeginPopup("create_point_popup"))
            Point::Popup(*this); 

        if (ImGui::BeginPopup("create_spline_popup"))
            Spline::SplinePopup(*this); 

        if (ImGui::BeginPopup("create_patch_popup"))
            Patch::PatchPopup(*this);

        if (ImGui::BeginPopup("create_part_popup"))
            Part::Popup(*this);

        if (ImGui::BeginPopup("create_stlEntity_popup"))
            StlEntity::Popup(*this);
    }
}
