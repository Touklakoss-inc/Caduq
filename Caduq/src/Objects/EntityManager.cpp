#include "EntityManager.h"

#include "BobIntegration.h"
#include "Entity.h"
#include "Point.h"
#include "Spline.h"
#include "Patch.h"

#include <Eigen/Core>
#include <imgui.h>
#include <set>

namespace Caduq 
{
    template<typename T> 
    void MyCombo(const char* name, std::vector<std::weak_ptr<T>> list, int& point_idx)
    {
        const auto combo_preview_value = list[point_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
        if (ImGui::BeginCombo(name, combo_preview_value.lock()->GetName().data()))
        {
            for (int n = 0; n < list.size(); n++)
            {
                const bool is_selected = (point_idx == n);
                if (ImGui::Selectable(list[n].lock()->GetName().data(), is_selected))
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
            if (!m_PointList.empty())
                ImGui::OpenPopup("create_spline_popup");
            else
                CQ_WARN("You need to create some points first");
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
            if (!m_SplineList.empty())
                ImGui::OpenPopup("create_patch_popup");
            else
                CQ_WARN("You need to create some splines first");
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
            if (!m_SplineList.empty())
                ImGui::OpenPopup("create_patch_popup");
            else
                CQ_WARN("You need to create some splines first");
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
                CreateEntity(std::make_shared<Caduq::Point>(Eigen::Vector3d{m_GuiPointPopupCoord[0], m_GuiPointPopupCoord[1], m_GuiPointPopupCoord[2]}, Type::point));
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
        MyCombo("Start Point", m_PointList, m_GuiStartPointID);
        ImGui::InputFloat2("Start Tangent Vector", m_GuiStartTangent);
        ImGui::InputFloat("Start Tension", m_GuiStartTension);

        ImGui::Separator();

        MyCombo("End Point", m_PointList, m_GuiEndPointID);
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
                    CreateEntity(std::make_shared<Caduq::Spline>(m_PointList.at(m_GuiStartPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiStartTangent[0], m_GuiStartTangent[1], 
                                                                                       m_GuiStartTangent[2]}, m_GuiStartTension[0]},
                                                                 m_PointList.at(m_GuiEndPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiEndTangent[0], m_GuiEndTangent[1],
                                                                                       m_GuiEndTangent[2]}, m_GuiEndTension[0]},
                                                                 100, Type::spline));              
                }
                else
                {
                    std::dynamic_pointer_cast<Caduq::Spline>(m_CurEntity)->Update(
                                                                 m_PointList.at(m_GuiStartPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiStartTangent[0], m_GuiStartTangent[1], 
                                                                                       m_GuiStartTangent[2]}, m_GuiStartTension[0]},
                                                                 m_PointList.at(m_GuiEndPointID).lock(),
                                                                 Caduq::PointTangency{{m_GuiEndTangent[0], m_GuiEndTangent[1],
                                                                                       m_GuiEndTangent[2]}, m_GuiEndTension[0]});

                }

                ImGui::CloseCurrentPopup();
                m_CurEntity = nullptr;
            }
            else
                CQ_WARN("Select two different points to create a spline");
        }
        ImGui::EndPopup();
    }

    void EntityManager::PatchPopup()
    {
        MyCombo("First Spline", m_SplineList, m_GuiSpline1ID);
        MyCombo("Second Spline", m_SplineList, m_GuiSpline2ID);

        ImGui::Separator();

        MyCombo("Third Spline", m_SplineList, m_GuiSpline3ID);
        if (m_GuiSpline4ID != -1)
            MyCombo("Fourth Spline", m_SplineList, m_GuiSpline4ID);

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
                    spline4 = m_SplineList.at(m_GuiSpline4ID).lock();
                if (m_CurEntity == nullptr)
                {
                    CreateEntity(std::make_shared<Caduq::Patch>(m_SplineList.at(m_GuiSpline1ID).lock(),
                                                               m_SplineList.at(m_GuiSpline2ID).lock(),
                                                               m_SplineList.at(m_GuiSpline3ID).lock(),
                                                               spline4,
                                                               10, Type::patch));
                }
                else
                {
                    CQ_TRACE(m_CurEntity->GetName());
                    std::dynamic_pointer_cast<Caduq::Patch>(m_CurEntity)->Update(m_SplineList.at(m_GuiSpline1ID).lock(),
                                                                                 m_SplineList.at(m_GuiSpline2ID).lock(),
                                                                                 m_SplineList.at(m_GuiSpline3ID).lock(),
                                                                                 spline4);
                }

                ImGui::CloseCurrentPopup();
                m_CurEntity = nullptr;
            }
            else
                CQ_WARN("Select four different splines to create a patch");
        }
        ImGui::EndPopup();
    }

    void EntityManager::CreateEntity(const std::shared_ptr<Entity>& entity)
    {
        m_EntityList.push_back(entity);
        switch (entity->GetType()) 
        {
            case Type::point :
                m_PhyXManager->AddPhyXPointToList(std::dynamic_pointer_cast<Point>(entity)->GetPhyXPoint());
                m_PointList.push_back(std::dynamic_pointer_cast<Point>(entity));
                break;
            case Type::spline :
                m_SplineList.push_back(std::dynamic_pointer_cast<Spline>(entity));
                break;
            case Type::patch :
                m_PatchList.push_back(std::dynamic_pointer_cast<Patch>(entity));
                break;
        }

        entity->Init();
        CQ_INFO(entity->GetName() + " created");
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

    template<typename T> 
    void EntityManager::CleanWeakPtrList(std::vector<std::weak_ptr<T>>& list)
    {
        list.erase(std::remove_if(list.begin(), list.end(), [](const std::weak_ptr<T>& l) { return l.expired(); }), list.end());
    }

    void EntityManager::SetSplinePopupParam(Geometry::SplinePoint startPoint, int startPointID, Geometry::SplinePoint endPoint, int endPointID)
    {
        for (int i = 0; i < 3; i++)
            m_GuiStartTangent[i] = startPoint.tangent[i];
        m_GuiStartTension[0] = static_cast<float>(startPoint.tension);

        for (int i = 0; i < 3; i++)
            m_GuiEndTangent[i] = endPoint.tangent[i];
        m_GuiEndTension[0] = static_cast<float>(endPoint.tension);

        for (int i = 0; i < m_PointList.size(); i++)
        {
            auto curPointID = m_PointList.at(i).lock()->GetID();
            if (curPointID == startPointID)
                m_GuiStartPointID = i;
            else if (curPointID == endPointID)
                m_GuiEndPointID = i;

        }
    }

    void EntityManager::SetPatchPopupParam(int spline1ID, int spline2ID, int spline3ID, int spline4ID)
    {
        for (int i = 0; i < m_SplineList.size(); i++)
        {
            auto curSplineID = m_SplineList.at(i).lock()->GetID();

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
