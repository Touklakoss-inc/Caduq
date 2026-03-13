#include "PhyXManager.h"

#include <Eigen/Core>
#include "XPBD/PhyXPart.h"
#include <imgui.h>
#include "tracy/Tracy.hpp"
#include <iostream>

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
        std::cout << "Clear entities..." << '\n';

        m_PtXPosition.clear();
        m_PtYPosition.clear();
        m_PtZPosition.clear();

        m_PtXLastPos.clear();
        m_PtYLastPos.clear();
        m_PtZLastPos.clear();

        m_PtXVelocity.clear();
        m_PtYVelocity.clear();
        m_PtZVelocity.clear();

        m_PtGrounded.clear();

        m_PtMasses.clear();

        std::cout << "Load entities..." << '\n';
        for (const auto& phyXPart : m_PhyXPartList)
        {
            Eigen::Vector3d pos = phyXPart->frame->GetPosition();
            m_PtXPosition.push_back(pos.x());
            m_PtYPosition.push_back(pos.y());
            m_PtZPosition.push_back(pos.z());

            Eigen::Vector3d lastPos = Eigen::Vector3d::Zero();
            m_PtXLastPos.push_back(lastPos.x());
            m_PtYLastPos.push_back(lastPos.y());
            m_PtZLastPos.push_back(lastPos.z());

            Eigen::Vector3d vel = Eigen::Vector3d::Zero();
            m_PtXVelocity.push_back(vel.x());
            m_PtYVelocity.push_back(vel.y());
            m_PtZVelocity.push_back(vel.z());

            m_PtGrounded.push_back(phyXPart->isGrounded);

            m_PtMasses.push_back(phyXPart->mass);
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
        ZoneScoped;
        if (!s_PhyXEnabled || !m_TimeEnabled)
            return;

        double dts = static_cast<double>(dt)/static_cast<double>(m_GuiSubSteps);

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

        for (int i = 0; i < m_PhyXPartList.size(); i++) 
        {
            auto& phyXPart = m_PhyXPartList[i];

            Eigen::Vector3d pos = {m_PtXPosition[i], m_PtYPosition[i], m_PtZPosition[i]};
            phyXPart->frame->SetPositionRotation(pos, Eigen::Quaterniond::Identity());
        }
    }

    void PhyXManager::CreateJoint(const std::shared_ptr<PhyXPart> p1, const std::shared_ptr<PhyXPart> p2, double dRest, double alpha)
    {
        auto it1 = std::find(m_PhyXPartList.begin(), m_PhyXPartList.end(), p1);
        auto it2 = std::find(m_PhyXPartList.begin(), m_PhyXPartList.end(), p2);
        if (it1 != m_PhyXPartList.end() && it2 != m_PhyXPartList.end())
        {
            int p1 = std::distance(m_PhyXPartList.begin(), it1);
            int p2 = std::distance(m_PhyXPartList.begin(), it2);
            m_Joints.push_back({p1, p2, dRest, alpha});
        }
        else
            std::cout << "point not found\n";
    }

    void PhyXManager::RenderImGui()
    {
        if (ImGui::Checkbox("Enable PhyX", &s_PhyXEnabled) && s_PhyXEnabled)
            ImportEntities();

        if (s_PhyXEnabled)
        {
            ImGui::SameLine();
            if (ImGui::Button("Import phyX points"))
                ImportEntities();

            ImGui::Checkbox("Enable Time", &m_TimeEnabled);

            ImGui::Separator();

            float framerate = ImGui::GetIO().Framerate;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / framerate, framerate);
            ImGui::InputInt("PhyX substeps", &m_GuiSubSteps);
        }
    }
}
