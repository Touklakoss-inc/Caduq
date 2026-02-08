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
            m_GuiPointPopupCoord[0] = 0.0f;
            m_GuiPointPopupCoord[1] = 0.0f;
            m_GuiPointPopupCoord[2] = 0.0f;

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_point_popup");
        }

        ImGui::SameLine();
        // Spline Creation
        if (ImGui::Button("Create Spline"))
        {
            m_GuiStartPointID = 0; // Here we store our selection data as an index.
            m_GuiStartTangent[0] = 0.0f;
            m_GuiStartTangent[1] = 0.0f;
            m_GuiStartTangent[2] = 0.0f;
            m_GuiStartTension[0] = 1.0f;

            m_GuiEndPointID = 0; // Here we store our selection data as an index.
            m_GuiEndTangent[0] = 0.0f;
            m_GuiEndTangent[1] = 0.0f;
            m_GuiEndTangent[2] = 0.0f;
            m_GuiEndTension[0] = 1.0f;

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_spline_popup");
        }

        ImGui::SameLine();
        // Patch4 Creation
        if (ImGui::Button("Create Patch4"))
        {
            m_GuiSpline1ID = 0;
            m_GuiSpline2ID = 0;
            m_GuiSpline3ID = 0;
            m_GuiSpline4ID = 0;

            m_CurEntity = nullptr;
            ImGui::OpenPopup("create_patch_popup");
        }

        ImGui::SameLine();
        // Patch3 Creation
        if (ImGui::Button("Create Patch3"))
        {
            m_GuiSpline1ID = 0;
            m_GuiSpline2ID = 0;
            m_GuiSpline3ID = 0;
            m_GuiSpline4ID = -1;

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
        ImGui::InputFloat2("", m_GuiPointPopupCoord);

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
                CreatePoint(std::make_shared<Caduq::Point>(Eigen::Vector3d{m_GuiPointPopupCoord[0], m_GuiPointPopupCoord[1], m_GuiPointPopupCoord[2]}, Type::point));
            else
            {
                std::dynamic_pointer_cast<Caduq::Point>(m_CurEntity)->Update(m_GuiPointPopupCoord[0], m_GuiPointPopupCoord[1], m_GuiPointPopupCoord[2]);

                ImGui::CloseCurrentPopup();
                m_CurEntity = nullptr;
            }

        }
        ImGui::EndPopup();
    }

    void EntityManager::SplinePopup()
    {
        MyCombo("Start Point", m_Point_List, m_GuiStartPointID);
        ImGui::InputFloat2("Start Tangent Vector", m_GuiStartTangent);
        ImGui::InputFloat("Start Tension", m_GuiStartTension);

        ImGui::Separator();

        MyCombo("End Point", m_Point_List, m_GuiEndPointID);
        ImGui::InputFloat2("End Tangent Vector", m_GuiEndTangent);
        ImGui::InputFloat("End Tension", m_GuiEndTension);

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
            if (m_GuiStartPointID != m_GuiEndPointID)
            {
                if (m_CurEntity == nullptr)
                {
                    CreateSpline(std::make_shared<Caduq::Spline>(m_Point_List.at(m_GuiStartPointID), 
                                                                 Caduq::PointTangency{{m_GuiStartTangent[0], m_GuiStartTangent[1], 
                                                                                       m_GuiStartTangent[2]}, m_GuiStartTension[0]},
                                                                 m_Point_List.at(m_GuiEndPointID),
                                                                 Caduq::PointTangency{{m_GuiEndTangent[0], m_GuiEndTangent[1],
                                                                                       m_GuiEndTangent[2]}, m_GuiEndTension[0]},
                                                                 100, Type::spline));              
                }
                else
                {
                    std::dynamic_pointer_cast<Caduq::Spline>(m_CurEntity)->Update(
                                                                 m_Point_List.at(m_GuiStartPointID), 
                                                                 Caduq::PointTangency{{m_GuiStartTangent[0], m_GuiStartTangent[1], 
                                                                                       m_GuiStartTangent[2]}, m_GuiStartTension[0]},
                                                                 m_Point_List.at(m_GuiEndPointID),
                                                                 Caduq::PointTangency{{m_GuiEndTangent[0], m_GuiEndTangent[1],
                                                                                       m_GuiEndTangent[2]}, m_GuiEndTension[0]});

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
        MyCombo("First Spline", m_Spline_List, m_GuiSpline1ID);
        MyCombo("Second Spline", m_Spline_List, m_GuiSpline2ID);

        ImGui::Separator();

        MyCombo("Third Spline", m_Spline_List, m_GuiSpline3ID);
        if (m_GuiSpline4ID != -1)
            MyCombo("Fourth Spline", m_Spline_List, m_GuiSpline4ID);

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
            std::set<int> indexes = { m_GuiSpline1ID, m_GuiSpline2ID, m_GuiSpline3ID, m_GuiSpline4ID };
            if (indexes.size() == 4)
            {
                std::shared_ptr<Caduq::Spline> spline4 { nullptr };
                if (m_GuiSpline4ID != -1)
                    spline4 = m_Spline_List.at(m_GuiSpline4ID);
                if (m_CurEntity == nullptr)
                {
                    CreatePatch(std::make_shared<Caduq::Patch>(m_Spline_List.at(m_GuiSpline1ID),
                                                               m_Spline_List.at(m_GuiSpline2ID),
                                                               m_Spline_List.at(m_GuiSpline3ID),
                                                               spline4,
                                                               10, Type::patch));
                }
                else
                {
                    VZ_TRACE(m_CurEntity->GetName());
                    std::dynamic_pointer_cast<Caduq::Patch>(m_CurEntity)->Update(m_Spline_List.at(m_GuiSpline1ID),
                                                                                 m_Spline_List.at(m_GuiSpline2ID),
                                                                                 m_Spline_List.at(m_GuiSpline3ID),
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
            m_GuiStartTangent[i] = startPoint.tangent[i];
        m_GuiStartTension[0] = static_cast<float>(startPoint.tension);

        for (int i = 0; i < 3; i++)
            m_GuiEndTangent[i] = endPoint.tangent[i];
        m_GuiEndTension[0] = static_cast<float>(endPoint.tension);


        for (int i = 0; i < m_Point_List.size(); i++)
        {
            auto curPointID = m_Point_List.at(i)->GetID();
            if (curPointID == startPointID)
                m_GuiStartPointID = i;
            else if (curPointID == endPointID)
                m_GuiEndPointID = i;
        }
    }

    void EntityManager::SetPatchPopupParam(int spline1ID, int spline2ID, int spline3ID, int spline4ID)
    {
        for (int i = 0; i < m_Spline_List.size(); i++)
        {
            auto curSplineID = m_Spline_List.at(i)->GetID();

            if (curSplineID == spline1ID)
                m_GuiSpline1ID = i;
            else if (curSplineID == spline2ID)
                m_GuiSpline2ID = i;
            else if (curSplineID == spline3ID)
                m_GuiSpline3ID = i;
            else if (curSplineID == spline4ID)
                m_GuiSpline4ID = i;
        }
    }
}
