#include "PhyXManager.h"

#include <Eigen/Core>
#include "XPBD/JAttach.h"
#include "XPBD/Joint.h"
#include "XPBD/Point.h"
#include <imgui.h>

#include <memory>
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

    void PhyXManager::ImportEntities()
    {
        std::cout << "Load entities..." << '\n';
        for (const auto& phyXPoint : m_PhyXPointList)
        {
            Eigen::Vector3d pos = phyXPoint->GetGeoPoint()->GetPosition();
            m_PtXPosition.push_back(pos.x());
            m_PtYPosition.push_back(pos.y());
            m_PtZPosition.push_back(pos.z());

            Eigen::Vector3d lastPos = phyXPoint->GetLastPosition();
            m_PtXLastPos.push_back(lastPos.x());
            m_PtYLastPos.push_back(lastPos.y());
            m_PtZLastPos.push_back(lastPos.z());

            Eigen::Vector3d vel = phyXPoint->GetVelocity();
            m_PtXVelocity.push_back(vel.x());
            m_PtYVelocity.push_back(vel.y());
            m_PtZVelocity.push_back(vel.z());

            m_PtGrounded.push_back(phyXPoint->IsGrounded());

            m_PtMasses.push_back(phyXPoint->GetMass());
        }
        std::cout << "Done" << '\n';
    }
    void PhyXManager::ApplyLinearCorrection(int p1, int p2, Eigen::Vector3d dp, double alpha, double dts)
    {
        double C = dp.norm();
        Eigen::Vector3d n = dp.normalized();

        double w1 = 1.0/m_PtMasses[p1];
        if (m_PtGrounded[p1])
            w1 = 0.0;

        double w2 = 1.0/m_PtMasses[p2];
        if (m_PtGrounded[p2])
            w2 = 0.0;

        double lambda = -C;
        if (dts != 0.0 && (w1 != 0.0 || w2 != 0.0 || alpha != 0.0))
            lambda /= w1 + w2 + alpha/(dts*dts);

        m_PtXPosition[p1] += lambda*n.x()*w1;
        m_PtYPosition[p1] += lambda*n.y()*w1;
        m_PtZPosition[p1] += lambda*n.z()*w1;

        m_PtXPosition[p2] -= lambda*n.x()*w2;
        m_PtYPosition[p2] -= lambda*n.y()*w2;
        m_PtZPosition[p2] -= lambda*n.z()*w2;
    }

    void PhyXManager::AttachJoint(int p1, int p2, double dRest, double alpha, double dts)
    {
        Eigen::Vector3d n = {m_PtXPosition[p2] - m_PtXPosition[p1],
                             m_PtYPosition[p2] - m_PtYPosition[p1],
                             m_PtZPosition[p2] - m_PtZPosition[p1]}; 
        double d = n.norm();
        n.normalize();

        ApplyLinearCorrection(p1, p2, -(d-dRest)*n, alpha, dts);
    }

    void PhyXManager::UpdatePhyX(float dt)
    {
        if (!s_PhyXEnabled || !m_TimeEnabled)
            return;

        double dts = static_cast<double>(dt)/static_cast<double>(m_GuiSubSteps);
        /*
        for (int n = 0; n < m_GuiSubSteps; n++)
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

            for (const auto& phyXPoint : m_PhyXPointList)
            {
                if (!phyXPoint->IsGrounded())
                {
                    auto& geoPoint = phyXPoint->GetGeoPoint();
                    phyXPoint->SetVelocity((geoPoint->GetPosition() - phyXPoint->GetLastPosition())/dts);
                }
            }
        }
        */

        for (int n = 0; n < m_GuiSubSteps; n++)
        {
            for (int i = 0; i < m_PtXPosition.size(); i++)
            {
                if (!m_PtGrounded[i])
                {
                    m_PtYVelocity[i] += dts*g.y();

                    m_PtXLastPos[i] = m_PtXPosition[i];
                    m_PtYLastPos[i] = m_PtYPosition[i];
                    m_PtZLastPos[i] = m_PtZPosition[i];

                    m_PtXPosition[i] += dts*m_PtXVelocity[i];
                    m_PtYPosition[i] += dts*m_PtYVelocity[i];
                    m_PtZPosition[i] += dts*m_PtZVelocity[i];
                }
            }

            for (int i = 0; i < m_Joints.size(); i++)
            {
                auto& joint = m_Joints[i];
                AttachJoint(joint.pt1, joint.pt2, joint.m_DRest, joint.m_Alpha, dts);
            }

            for (int i = 0; i < m_PtXPosition.size(); i++)
            {
                m_PtXVelocity[i] = (m_PtXPosition[i] - m_PtXLastPos[i])/dts;
                m_PtYVelocity[i] = (m_PtYPosition[i] - m_PtYLastPos[i])/dts;
                m_PtZVelocity[i] = (m_PtZPosition[i] - m_PtZLastPos[i])/dts;
            }
        }

        for (int i = 0; i < m_PhyXPointList.size(); i++) 
        {
            auto& phyXPoint = m_PhyXPointList[i];
            auto& geoPoint = phyXPoint->GetGeoPoint();

            Eigen::Vector3d vel = {m_PtXVelocity[i], m_PtYVelocity[i], m_PtZVelocity[i]};
            phyXPoint->SetVelocity(vel);

            Eigen::Vector3d lastPos = {m_PtXLastPos[i], m_PtYLastPos[i], m_PtZLastPos[i]};
            phyXPoint->SetLastPosition(lastPos);

            Eigen::Vector3d pos = {m_PtXPosition[i], m_PtYPosition[i], m_PtZPosition[i]};
            geoPoint->SetPosition(pos);
        }
    }

    void PhyXManager::CreateJoint(const std::shared_ptr<Joint>& joint)
    {
        joint->Init();
        m_JointList.push_back(joint);
        auto jAttach = std::dynamic_pointer_cast<JAttach>(joint);

        auto it1 = std::find(m_PhyXPointList.begin(), m_PhyXPointList.end(), jAttach->m_P1);
        auto it2 = std::find(m_PhyXPointList.begin(), m_PhyXPointList.end(), jAttach->m_P2);
        if (it1 != m_PhyXPointList.end() && it2 != m_PhyXPointList.end())
        {
            int p1 = std::distance(m_PhyXPointList.begin(), it1);
            int p2 = std::distance(m_PhyXPointList.begin(), it2);
            m_Joints.push_back({p1, p2, jAttach->m_DRest, jAttach->m_Alpha});
        }
        else
            std::cout << "point not found\n";
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
        if (ImGui::Checkbox("Enable PhyX", &s_PhyXEnabled) && s_PhyXEnabled)
        {
            ImportEntities();
        }

        if (s_PhyXEnabled)
        {
            ImGui::SameLine();
            ImGui::Checkbox("Enable Time", &m_TimeEnabled);

            ImGui::Separator();

            float framerate = ImGui::GetIO().Framerate;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / framerate, framerate);
            ImGui::InputInt("PhyX substeps", &m_GuiSubSteps);

            ImGui::Separator();

            if (ImGui::Button("Attach"))
            {
                m_GuiStartPointID = 0;
                m_GuiEndPointID = 0;
                m_GuiDistRest[0] = 0.0;

                ImGui::OpenPopup("create_attach_popup");
            }

            ImGui::Separator();

            if (ImGui::BeginPopup("create_attach_popup"))
                AttachPopup();

        }
    }

    void PhyXManager::AttachPopup()
    {
        MyCombo("Start Point", m_PhyXPointList, m_GuiStartPointID);
        MyCombo("End Point", m_PhyXPointList, m_GuiEndPointID);

        ImGui::InputDouble("Distance", &m_GuiDistRest[0]);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            // Check if all 4 selected splines are different
            if (m_GuiStartPointID != m_GuiEndPointID)
            {
                if (m_CurJoint == nullptr)
                {
                    CreateJoint(std::make_shared<JAttach>(m_PhyXPointList.at(m_GuiStartPointID),
                                                          m_PhyXPointList.at(m_GuiEndPointID),
                                                          m_GuiDistRest[0], 0.0));
                }
                else
                {
                    std::dynamic_pointer_cast<JAttach>(m_CurJoint)->Update(m_PhyXPointList.at(m_GuiStartPointID),
                                                       m_PhyXPointList.at(m_GuiEndPointID),
                                                       m_GuiDistRest[0], 0.0);
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
        m_GuiDistRest[0] = dist;
        for (int i = 0; i < m_PhyXPointList.size(); i++)
        {
            auto curPointID = m_PhyXPointList.at(i)->GetID();
            if (curPointID == startPointID)
                m_GuiStartPointID = i;
            else if (curPointID == endPointID)
                m_GuiEndPointID = i;
        }
    }
}
