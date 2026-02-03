#include "PhyXManager.h"

#include <Eigen/Core>
#include "EntityManager.h"
#include <imgui/imgui.h>

namespace Caduq
{
    void PhyXManager::RenderImGui()
    {
        ImGui::Checkbox("Enable PhyX", &s_PhyXEnabled);

        ImGui::Separator();

        if (s_PhyXEnabled)
        {
            ImGui::Checkbox("Enable Time", &m_TimeEnabled);
        }
    }

    void PhyXManager::UpdatePhyX(EntityManager& entityManager, float dt, ushort nSubStep)
    {
        if (!s_PhyXEnabled || !m_TimeEnabled)
            return;

        double dts = static_cast<double>(dt)/static_cast<double>(nSubStep);
        for (ushort n = 0; n < nSubStep; n++)
        {
            for (const auto& point : entityManager.GetPointList())
            {
                auto& phyXPoint = point->GetPhyXPoint(); 
                if (!phyXPoint->IsGrounded())
                {
                    auto& geoPoint = point->GetGeoPoint();
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

            for (const auto& point : entityManager.GetPointList())
            {
                auto& phyXPoint = point->GetPhyXPoint(); 
                if (!phyXPoint->IsGrounded())
                {
                    auto& geoPoint = point->GetGeoPoint();
                    phyXPoint->SetVelocity((geoPoint->GetPosition() - phyXPoint->GetLastPosition())/dts);
                }
            }
        }
    }

    void PhyXManager::CreateJoint(const std::shared_ptr<XPBD::Joint>& joint)
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
}
