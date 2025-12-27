#include "EntityManager.h"

#include "Vizir/Logging/Log.h"
#include "imgui/imgui.h"
#include <memory>

#include <string>
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
    
    void EntityManager::RenderImGui()
    {
        // Point Creation
        if (ImGui::Button("Create Point"))
            ImGui::OpenPopup("create_point_popup");

        if (ImGui::BeginPopup("create_point_popup"))
        {
            static float coord[3] = { 0.0f, 0.0f, 0.0f };
            ImGui::InputFloat2("", coord);

            ImGui::Separator();

            if (ImGui::Button("Create"))
            {
                CreatePoint(std::make_shared<Point>(coord[0], coord[1], coord[2], Type::point));
                VZ_INFO("Point created at " + std::to_string(coord[0]) + ", " + std::to_string(coord[1]) + ", " + std::to_string(coord[2]));
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        // Spline Creation
        if (ImGui::Button("Create Spline"))
            ImGui::OpenPopup("create_spline_popup");

        if (ImGui::BeginPopup("create_spline_popup"))
        {
            static int start_point_idx = 0; // Here we store our selection data as an index.
            MyCombo("Start Point", m_Point_List, start_point_idx);

            static float start_tangency[3] = { 0.0f, 0.0f, 0.0f };
            ImGui::InputFloat2("Start Tangent Vector", start_tangency);

            static float start_tension[1] = { 1.0f };
            ImGui::InputFloat("Start Tension", start_tension);

            ImGui::Separator();

            static int end_point_idx = 0; // Here we store our selection data as an index.
            MyCombo("End Point", m_Point_List, end_point_idx);

            static float end_tangency[3] = { 0.0f, 0.0f, 0.0f };
            ImGui::InputFloat2("End Tangent Vector", end_tangency);

            static float end_tension[1] = { 1.0f };
            ImGui::InputFloat("End Tension", end_tension);

            ImGui::Separator();

            if (ImGui::Button("Create"))
            {
                if (start_point_idx != end_point_idx)
                {
                    CreateSpline(std::make_shared<Caduq::Spline>(m_Point_List.at(start_point_idx), 
                                                                 Caduq::PointTangency{{start_tangency[0], start_tangency[1], 
                                                                                       start_tangency[2]}, start_tension[0]},
                                                                 m_Point_List.at(end_point_idx),
                                                                 Caduq::PointTangency{{end_tangency[0], end_tangency[1],
                                                                                       end_tangency[2]}, end_tension[0]},
                                                                 100, Type::spline));              
                    VZ_INFO("Spline created between " + m_Point_List.at(start_point_idx)->GetName()
                            + " and " + m_Point_List.at(end_point_idx)->GetName());
                }
                else
                    VZ_INFO("Select two different points to create a spline");
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        // Patch Creation
        if (ImGui::Button("Create Patch"))
            ImGui::OpenPopup("create_patch_popup");

        if (ImGui::BeginPopup("create_patch_popup"))
        {
            static int spline_1_idx = 0; // Here we store our selection data as an index.
            MyCombo("First Spline", m_Spline_List, spline_1_idx);

            static int spline_2_idx = 0; // Here we store our selection data as an index.
            MyCombo("Second Spline", m_Spline_List, spline_2_idx);

            ImGui::Separator();

            static int spline_3_idx = 0; // Here we store our selection data as an index.
            MyCombo("Third Spline", m_Spline_List, spline_3_idx);

            static int spline_4_idx = 0; // Here we store our selection data as an index.
            MyCombo("Fourth Spline", m_Spline_List, spline_4_idx);

            ImGui::Separator();

            if (ImGui::Button("Create"))
            {
                std::set<int> indexes = { spline_1_idx, spline_2_idx, spline_3_idx, spline_4_idx };
                if (indexes.size() == 4)
                {
                    CreatePatch(std::make_shared<Caduq::Patch>(m_Spline_List.at(spline_1_idx),
                                                               m_Spline_List.at(spline_2_idx),
                                                               m_Spline_List.at(spline_3_idx),
                                                               m_Spline_List.at(spline_4_idx),
                                                               10, Type::patch));
                        VZ_INFO("Patch created between ", 
                                m_Spline_List.at(spline_1_idx)->GetName() + " and ",
                                m_Spline_List.at(spline_2_idx)->GetName() + " and ",
                                m_Spline_List.at(spline_3_idx)->GetName() + " and ",
                                m_Spline_List.at(spline_4_idx)->GetName());
                }
                else
                    VZ_INFO("Select two different points to create a spline");
            }
            ImGui::EndPopup();
        }
    }

    void EntityManager::CreatePoint(const std::shared_ptr<Point>& point)
    {
        m_Point_List.push_back(point); // push_back make a copy of the shared pointer
        point->Init();
    }
    void EntityManager::CreateSpline(const std::shared_ptr<Spline>& spline)
    {
        m_Spline_List.push_back(spline);
        spline->Init();
    }
    void EntityManager::CreatePatch(const std::shared_ptr<Patch>& patch)
    {
        m_Patch_List.push_back(patch);
        patch->Init();
    }

    void EntityManager::DeleteEntity(const std::shared_ptr<Entity>& entity)
    {
        m_EntityToDelete.push_back(entity);
        for (const auto& c : entity->GetChildren())
        {
            m_EntityToDelete.push_back(c);
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
        }

        m_EntityToDelete.clear();
    }

    void EntityManager::DeletePoint(const std::shared_ptr<Point>& point)
    {
        point->Delete(*this);

        auto it = std::find(m_Point_List.begin(), m_Point_List.end(), point);
        if (it != m_Point_List.end())
        {
            m_Point_List.erase(it);
            VZ_INFO(point->GetName() + " removed from list");
        }
    }
    void EntityManager::DeleteSpline(const std::shared_ptr<Spline>& spline)
    {
        spline->Delete(*this);

        auto it = std::find(m_Spline_List.begin(), m_Spline_List.end(), spline);
        if (it != m_Spline_List.end())
        {
            m_Spline_List.erase(it);
            VZ_INFO(spline->GetName() + " removed from list");
        }
    }
    void EntityManager::DeletePatch(const std::shared_ptr<Patch>& patch)
    {
        patch->Delete(*this);

        auto it = std::find(m_Patch_List.begin(), m_Patch_List.end(), patch);
        if (it != m_Patch_List.end())
        {
            m_Patch_List.erase(it);
            VZ_INFO(patch->GetName() + " removed from list");
        }
    }

    std::shared_ptr<Point>& EntityManager::GetPoint(int index)
    {
        return m_Point_List.at(index);
    }

    std::shared_ptr<Spline>& EntityManager::GetSpline(int index)
    {
        return m_Spline_List.at(index);
    }

    std::shared_ptr<Patch>& EntityManager::GetPatch(int index)
    {
        return m_Patch_List.at(index);
    }
}
