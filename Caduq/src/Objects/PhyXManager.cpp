#include "PhyXManager.h"

#include <Eigen/Core>
#include "EntityManager.h"
#include "XPBD/JAttach.h"
#include "XPBD/Joint.h"
#include "XPBD/Point.h"
#include <imgui/imgui.h>

#include <memory>
namespace Caduq
{
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

    void PhyXManager::RenderImGui(const EntityManager& entityManager)
    {
        ImGui::Checkbox("Enable PhyX", &s_PhyXEnabled);

        if (s_PhyXEnabled)
        {
            ImGui::SameLine();
            ImGui::Checkbox("Enable Time", &m_TimeEnabled);

            ImGui::Separator();

            if (ImGui::Button("Attach"))
            {
                ImGui::OpenPopup("create_attach_joint");
            }

            if (ImGui::BeginPopup("create_attach_joint"))
                AttachPopup(entityManager);

        }
    }

    void PhyXManager::AttachPopup(const EntityManager& entityManager)
    {
        static int start_point_idx = 0;
        MyCombo("Start Point", entityManager.GetPointList(), start_point_idx);

        ImGui::Separator();

        static int end_point_idx = 0;
        MyCombo("End Point", entityManager.GetPointList(), end_point_idx);

        ImGui::Separator();

        static double d_rest[1] = { 0.0 };
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
                CreateJoint(std::make_shared<XPBD::JAttach>(entityManager.GetPointList().at(start_point_idx)->GetPhyXPoint(),
                                                            entityManager.GetPointList().at(end_point_idx)->GetPhyXPoint(),
                                                            d_rest[0], 0.0));
                ImGui::CloseCurrentPopup();
            }
            else
                VZ_WARN("Select two different points to create an attach joint");
        }
        ImGui::EndPopup();
    }
}
