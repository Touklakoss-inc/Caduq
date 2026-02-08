#include "PhyXManager.h"

#include <Eigen/Core>
#include "XPBD/JAttach.h"
#include "XPBD/Joint.h"
#include "XPBD/Point.h"
#include <imgui.h>

#include <memory>
#include <string>
namespace XPBD
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

    void PhyXManager::UpdatePhyX(float dt)
    {
        if (!s_PhyXEnabled || !m_TimeEnabled)
            return;

        double dts = static_cast<double>(dt)/static_cast<double>(sub_steps);
        for (int n = 0; n < sub_steps; n++)
        {
            for (const auto& phyXPoint : m_PhyXPointList)
            {
                if (!phyXPoint->IsGrounded())
                {
                    auto& geoPoint = phyXPoint->GetGeoPoint();
                    phyXPoint->SetVelocity(phyXPoint->GetVelocity() + dts*g);

                    phyXPoint->SetLastPosition(geoPoint->GetPosition());

                    geoPoint->SetPosition(geoPoint->GetPosition() + dts*phyXPoint->GetVelocity());
                }
            }

            for (const auto& joint : m_JointList)
            {
                joint->ApplyConstraints(dts);
            }

            // Attach(*entityManager.GetPoint(0), *entityManager.GetPoint(1), std::sqrt(2.0), 0.0, dts);
            // Attach(*entityManager.GetPoint(1), *entityManager.GetPoint(2), std::sqrt(1.25), 0.0, dts);
            // Attach(*entityManager.GetPoint(2), *entityManager.GetPoint(3), std::sqrt(4.0), 0.0, dts);

            for (const auto& phyXPoint : m_PhyXPointList)
            {
                if (!phyXPoint->IsGrounded())
                {
                    auto& geoPoint = phyXPoint->GetGeoPoint();
                    phyXPoint->SetVelocity((geoPoint->GetPosition() - phyXPoint->GetLastPosition())/dts);
                }
            }
        }
    }

    void PhyXManager::CreateJoint(const std::shared_ptr<Joint>& joint)
    {
        joint->Init();
        m_JointList.push_back(joint); 
    }

    void PhyXManager::ClearJointsToDelete()
    {
        for (const auto& joint : m_JointsToDelete)
        {
            joint->Delete();

            auto it = std::find(m_JointList.begin(), m_JointList.end(), joint);
            if (it != m_JointList.end())
            {
                m_JointList.erase(it);
            }
        }

        m_JointsToDelete.clear();
    }

    void PhyXManager::RemovePhyXPointFromList(const std::shared_ptr<Point>& phyXPoint)
    {
        auto it = std::find(m_PhyXPointList.begin(), m_PhyXPointList.end(), phyXPoint);
        if (it != m_PhyXPointList.end())
        {
            m_PhyXPointList.erase(it);
        }
    }

    void PhyXManager::RenderImGui()
    {
        ImGui::Checkbox("Enable PhyX", &s_PhyXEnabled);

        if (s_PhyXEnabled)
        {
            ImGui::SameLine();
            ImGui::Checkbox("Enable Time", &m_TimeEnabled);

            ImGui::Separator();

            float framerate = ImGui::GetIO().Framerate;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / framerate, framerate);
            ImGui::InputInt("PhyX substeps", &sub_steps);

            ImGui::Separator();

            if (ImGui::Button("Attach"))
            {
                start_point_idx = 0;
                end_point_idx = 0;
                d_rest[0] = 0.0;

                ImGui::OpenPopup("create_attach_popup");
            }

            ImGui::Separator();

            if (ImGui::BeginPopup("create_attach_popup"))
                AttachPopup();

        }
    }

    void PhyXManager::AttachPopup()
    {
        MyCombo("Start Point", m_PhyXPointList, start_point_idx);
        MyCombo("End Point", m_PhyXPointList, end_point_idx);

        ImGui::InputDouble("Distance", &d_rest[0]);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            // Check if all 4 selected splines are different
            if (start_point_idx != end_point_idx)
            {
                if (m_CurJoint == nullptr)
                {
                    CreateJoint(std::make_shared<JAttach>(m_PhyXPointList.at(start_point_idx),
                                                          m_PhyXPointList.at(end_point_idx),
                                                          d_rest[0], 0.0));
                }
                else
                {
                    std::dynamic_pointer_cast<JAttach>(m_CurJoint)->Update(m_PhyXPointList.at(start_point_idx),
                                                       m_PhyXPointList.at(end_point_idx),
                                                       d_rest[0], 0.0);
                }
                m_CurJoint = nullptr;

                ImGui::CloseCurrentPopup();
            }
            else
                std::cout << "Select two different points to create an attach joint" << '\n';
        }
        ImGui::EndPopup();
    }

    void PhyXManager::SetAttachPopupParam(int startPointID, int endPointID, double dist)
    {
        d_rest[0] = dist;
        for (int i = 0; i < m_PhyXPointList.size(); i++)
        {
            auto curPointID = m_PhyXPointList.at(i)->GetID();
            if (curPointID == startPointID)
                start_point_idx = i;
            else if (curPointID == endPointID)
                end_point_idx = i;
        }
    }
}
