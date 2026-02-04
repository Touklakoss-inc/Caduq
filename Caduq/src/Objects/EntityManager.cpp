#include "EntityManager.h"

#include <Eigen/Core>
#include "XPBD/PhyXManager.h"
#include "Vizir/Logging/Log.h"
#include <imgui/imgui.h>
#include <memory>

#include <vector>
#include <set>

namespace Caduq 
{
    template<typename T> 
    void MyCombo(const char* name, std::vector<std::shared_ptr<T>> list, int& point_idx)
    {
        const auto combo_preview_value = list[point_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
        if (ImGui::BeginCombo(name, combo_preview_value->GetName().data()))
        {
            for (int n = 0; n < list.size(); n++)
            {
                const bool is_selected = (point_idx == n);
                if (ImGui::Selectable(list[n]->GetName().data(), is_selected))
                    point_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    EntityManager::EntityManager()
        : m_PhyXManager{ std::make_shared<XPBD::PhyXManager>() }
    {
    }
    void EntityManager::RenderImGui()
    {
        // Point Creation
        if (ImGui::Button("Create Point"))
        {
            m_PointPopupCoord[0] = 0.0f;
            m_PointPopupCoord[1] = 0.0f;
            m_PointPopupCoord[2] = 0.0f;

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_point_popup");
        }

        ImGui::SameLine();
        // Spline Creation
        if (ImGui::Button("Create Spline"))
        {
            start_point_idx = 0; // Here we store our selection data as an index.
            start_tangency[0] = 0.0f;
            start_tangency[1] = 0.0f;
            start_tangency[2] = 0.0f;
            start_tension[0] = 1.0f;

            end_point_idx = 0; // Here we store our selection data as an index.
            end_tangency[0] = 0.0f;
            end_tangency[1] = 0.0f;
            end_tangency[2] = 0.0f;
            end_tension[0] = 1.0f;

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_spline_popup");
        }

        ImGui::SameLine();
        // Patch4 Creation
        if (ImGui::Button("Create Patch4"))
        {
            spline_1_idx = 0;
            spline_2_idx = 0;
            spline_3_idx = 0;
            spline_4_idx = 0;

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_patch_popup");
        }

        ImGui::SameLine();
        // Patch3 Creation
        if (ImGui::Button("Create Patch3"))
        {
            spline_1_idx = 0;
            spline_2_idx = 0;
            spline_3_idx = 0;
            spline_4_idx = -1;

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_patch_popup");
        }

        if (ImGui::BeginPopup("create_point_popup"))
            PointPopup();

        if (ImGui::BeginPopup("create_spline_popup"))
            SplinePopup(); 

        if (ImGui::BeginPopup("create_patch_popup"))
            PatchPopup();
    }

    void EntityManager::PointPopup()
    {
        ImGui::InputFloat2("", m_PointPopupCoord);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            m_CurEntity = nullptr;
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            if (m_CurEntity == nullptr)
                CreatePoint(std::make_shared<Caduq::Point>(Eigen::Vector3d{m_PointPopupCoord[0], m_PointPopupCoord[1], m_PointPopupCoord[2]}, Type::point));
            else
            {
                std::dynamic_pointer_cast<Caduq::Point>(m_CurEntity)->Update(m_PointPopupCoord[0], m_PointPopupCoord[1], m_PointPopupCoord[2]);

                ImGui::CloseCurrentPopup();
                m_CurEntity = nullptr;
            }

        }
        ImGui::EndPopup();
    }

    void EntityManager::SplinePopup()
    {
        MyCombo("Start Point", m_Point_List, start_point_idx);
        ImGui::InputFloat2("Start Tangent Vector", start_tangency);
        ImGui::InputFloat("Start Tension", start_tension);

        ImGui::Separator();

        MyCombo("End Point", m_Point_List, end_point_idx);
        ImGui::InputFloat2("End Tangent Vector", end_tangency);
        ImGui::InputFloat("End Tension", end_tension);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            m_CurEntity = nullptr;
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            // Check if all 4 selected splines are different
            if (start_point_idx != end_point_idx)
            {
                if (m_CurEntity == nullptr)
                {
                    CreateSpline(std::make_shared<Caduq::Spline>(m_Point_List.at(start_point_idx), 
                                                                 Caduq::PointTangency{{start_tangency[0], start_tangency[1], 
                                                                                       start_tangency[2]}, start_tension[0]},
                                                                 m_Point_List.at(end_point_idx),
                                                                 Caduq::PointTangency{{end_tangency[0], end_tangency[1],
                                                                                       end_tangency[2]}, end_tension[0]},
                                                                 100, Type::spline));              
                }
                else
                {
                    std::dynamic_pointer_cast<Caduq::Spline>(m_CurEntity)->Update(
                                                                 m_Point_List.at(start_point_idx), 
                                                                 Caduq::PointTangency{{start_tangency[0], start_tangency[1], 
                                                                                       start_tangency[2]}, start_tension[0]},
                                                                 m_Point_List.at(end_point_idx),
                                                                 Caduq::PointTangency{{end_tangency[0], end_tangency[1],
                                                                                       end_tangency[2]}, end_tension[0]});

                }

                ImGui::CloseCurrentPopup();
                m_CurEntity = nullptr;
            }
            else
                VZ_WARN("Select two different points to create a spline");
        }
        ImGui::EndPopup();
    }

    void EntityManager::PatchPopup()
    {
        MyCombo("First Spline", m_Spline_List, spline_1_idx);
        MyCombo("Second Spline", m_Spline_List, spline_2_idx);

        ImGui::Separator();

        MyCombo("Third Spline", m_Spline_List, spline_3_idx);
        if (spline_4_idx != -1)
            MyCombo("Fourth Spline", m_Spline_List, spline_4_idx);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            m_CurEntity = nullptr;
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            // Check if all 4 selected splines are different
            std::set<int> indexes = { spline_1_idx, spline_2_idx, spline_3_idx, spline_4_idx };
            if (indexes.size() == 4)
            {
                std::shared_ptr<Caduq::Spline> spline4 { nullptr };
                if (spline_4_idx != -1)
                    spline4 = m_Spline_List.at(spline_4_idx);
                if (m_CurEntity == nullptr)
                {
                    CreatePatch(std::make_shared<Caduq::Patch>(m_Spline_List.at(spline_1_idx),
                                                               m_Spline_List.at(spline_2_idx),
                                                               m_Spline_List.at(spline_3_idx),
                                                               spline4,
                                                               10, Type::patch));
                }
                else
                {
                    VZ_TRACE(m_CurEntity->GetName());
                    std::dynamic_pointer_cast<Caduq::Patch>(m_CurEntity)->Update(m_Spline_List.at(spline_1_idx),
                                                                                 m_Spline_List.at(spline_2_idx),
                                                                                 m_Spline_List.at(spline_3_idx),
                                                                                 spline4);
                }

                ImGui::CloseCurrentPopup();
                m_CurEntity = nullptr;
            }
            else
                VZ_WARN("Select four different splines to create a patch");
        }
        ImGui::EndPopup();
    }

    void EntityManager::CreatePoint(const std::shared_ptr<Point>& point)
    {
        m_Point_List.push_back(point); // push_back make a copy of the shared pointer
        m_PhyXManager->AddPhyXPointToList(point->GetPhyXPoint());
        point->Init();
        VZ_INFO("Point created");
    }
    void EntityManager::CreateSpline(const std::shared_ptr<Spline>& spline)
    {
        m_Spline_List.push_back(spline);
        spline->Init();
        VZ_INFO("Spline created");
    }
    void EntityManager::CreatePatch(const std::shared_ptr<Patch>& patch)
    {
        m_Patch_List.push_back(patch);
        patch->Init();
        VZ_INFO("Patch created");
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
            switch (entity->GetType())
            {
                case Type::point:
                    // /!\ shared pointer count increased by 2 because:
                    // - the entity pointer
                    // - dynamic_pointer_cast created an other one
                    DeletePoint(std::dynamic_pointer_cast<Point>(entity));
                    break;
                case Type::spline:
                    DeleteSpline(std::dynamic_pointer_cast<Spline>(entity));
                    break;
                case Type::patch:
                    DeletePatch(std::dynamic_pointer_cast<Patch>(entity));
                    break;
            }
            VZ_WARN(entity->GetName() + " removed from list");
        }

        m_EntityToDelete.clear();
    }

    void EntityManager::DeletePoint(const std::shared_ptr<Point>& point)
    {
        m_PhyXManager->RemovePhyXPointFromList(point->GetPhyXPoint());
        point->Delete();

        auto it = std::find(m_Point_List.begin(), m_Point_List.end(), point);
        if (it != m_Point_List.end())
        {
            m_Point_List.erase(it);
        }
    }
    void EntityManager::DeleteSpline(const std::shared_ptr<Spline>& spline)
    {
        spline->Delete();

        auto it = std::find(m_Spline_List.begin(), m_Spline_List.end(), spline);
        if (it != m_Spline_List.end())
        {
            m_Spline_List.erase(it);
        }
    }
    void EntityManager::DeletePatch(const std::shared_ptr<Patch>& patch)
    {
        patch->Delete();

        auto it = std::find(m_Patch_List.begin(), m_Patch_List.end(), patch);
        if (it != m_Patch_List.end())
        {
            m_Patch_List.erase(it);
        }
    }

    void EntityManager::SetSplinePopupParam(Geometry::SplinePoint startPoint, int startPointID, Geometry::SplinePoint endPoint, int endPointID)
    {
        for (int i = 0; i < 3; i++)
            start_tangency[i] = startPoint.tangent[i];
        start_tension[0] = static_cast<float>(startPoint.tension);

        for (int i = 0; i < 3; i++)
            end_tangency[i] = endPoint.tangent[i];
        end_tension[0] = static_cast<float>(endPoint.tension);


        for (int i = 0; i < m_Point_List.size(); i++)
        {
            auto curPointID = m_Point_List.at(i)->GetID();
            if (curPointID == startPointID)
                start_point_idx = i;
            else if (curPointID == endPointID)
                end_point_idx = i;
        }
    }

    void EntityManager::SetPatchPopupParam(int spline1ID, int spline2ID, int spline3ID, int spline4ID)
    {
        for (int i = 0; i < m_Spline_List.size(); i++)
        {
            auto curSplineID = m_Spline_List.at(i)->GetID();

            if (curSplineID == spline1ID)
                spline_1_idx = i;
            else if (curSplineID == spline2ID)
                spline_2_idx = i;
            else if (curSplineID == spline3ID)
                spline_3_idx = i;
            else if (curSplineID == spline4ID)
                spline_4_idx = i;
        }
    }
}
