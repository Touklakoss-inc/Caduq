#include "PhyXManager.h"

#include <Eigen/Core>
#include "Eigen/Geometry"
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

        m_PtXRotation.clear();
        m_PtYRotation.clear();
        m_PtZRotation.clear();
        m_PtWRotation.clear();

        m_PtXLastRot.clear();
        m_PtYLastRot.clear();
        m_PtZLastRot.clear();
        m_PtWLastRot.clear();

        m_PtXLinVelocity.clear();
        m_PtYLinVelocity.clear();
        m_PtZLinVelocity.clear();

        m_PtXAngVelocity.clear();
        m_PtYAngVelocity.clear();
        m_PtZAngVelocity.clear();

        m_PtMasses.clear();

        m_PtXInertia.clear();
        m_PtYInertia.clear();
        m_PtZInertia.clear();

        m_PtGrounded.clear();

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

            Eigen::Quaterniond rot = phyXPart->frame->GetRotation();
            m_PtXRotation.push_back(rot.x());
            m_PtYRotation.push_back(rot.y());
            m_PtZRotation.push_back(rot.z());
            m_PtWRotation.push_back(rot.w());

            Eigen::Quaterniond lastRot = Eigen::Quaterniond::Identity();
            m_PtXLastRot.push_back(lastRot.x());
            m_PtYLastRot.push_back(lastRot.y());
            m_PtZLastRot.push_back(lastRot.z());
            m_PtWLastRot.push_back(lastRot.w());

            Eigen::Vector3d linVel = Eigen::Vector3d::Zero();
            m_PtXLinVelocity.push_back(linVel.x());
            m_PtYLinVelocity.push_back(linVel.y());
            m_PtZLinVelocity.push_back(linVel.z());

            Eigen::Vector3d angVel = Eigen::Vector3d::Zero();
            m_PtXAngVelocity.push_back(angVel.x());
            m_PtYAngVelocity.push_back(angVel.y());
            m_PtZAngVelocity.push_back(angVel.z());

            m_PtMasses.push_back(phyXPart->mass);

            m_PtXInertia.push_back(phyXPart->inertiaTensor.x());
            m_PtYInertia.push_back(phyXPart->inertiaTensor.y());
            m_PtZInertia.push_back(phyXPart->inertiaTensor.z());

            m_PtGrounded.push_back(phyXPart->isGrounded);
        }
        std::cout << "Done" << '\n';
    }
    void PhyXManager::ApplyLinearCorrection(int prt1, Eigen::Vector3d r1, int prt2, Eigen::Vector3d r2, Eigen::Vector3d dp, double alpha, double dts)
    {
        double C = dp.norm();
        Eigen::Vector3d n = dp.normalized();

        Eigen::Matrix3d i1{ {1.0/m_PtXInertia[prt1], 0.0, 0.0},
                            {0.0, 1.0/m_PtYInertia[prt1], 0.0},
                            {0.0, 0.0, 1.0/m_PtZInertia[prt1]} };

        Eigen::Matrix3d i2{ {1.0/m_PtXInertia[prt2], 0.0, 0.0},
                            {0.0, 1.0/m_PtYInertia[prt2], 0.0},
                            {0.0, 0.0, 1.0/m_PtZInertia[prt2]} };

        Eigen::Vector3d rn1 = r1.cross(n);
        double w1 = 1.0/m_PtMasses[prt1] + rn1.transpose() * i1 * rn1;
        if (m_PtGrounded[prt1])
            w1 = 0.0;

        Eigen::Vector3d rn2 = r2.cross(n);
        double w2 = 1.0/m_PtMasses[prt2] + rn2.transpose() * i2 * rn2;
        if (m_PtGrounded[prt2])
            w2 = 0.0;

        double lambda = -C;
        if (dts != 0.0 && (w1 != 0.0 || w2 != 0.0 || alpha != 0.0))
            lambda /= w1 + w2 + alpha/(dts*dts);

        m_PtXPosition[prt1] += lambda*n.x()*w1;
        m_PtYPosition[prt1] += lambda*n.y()*w1;
        m_PtZPosition[prt1] += lambda*n.z()*w1;

        m_PtXPosition[prt2] -= lambda*n.x()*w2;
        m_PtYPosition[prt2] -= lambda*n.y()*w2;
        m_PtZPosition[prt2] -= lambda*n.z()*w2;

        Eigen::Quaterniond q1{ m_PtWRotation[prt1], m_PtXRotation[prt1], m_PtYRotation[prt1], m_PtZRotation[prt1] };
        Eigen::Quaterniond temp1 = Eigen::Quaterniond{ 0.0, i1 * rn1} * q1;

        q1.x() += 0.5 * lambda * temp1.x();
        q1.y() += 0.5 * lambda * temp1.y();
        q1.z() += 0.5 * lambda * temp1.z();
        q1.w() += 0.5 * lambda * temp1.w();

        q1.normalize();

        m_PtXRotation[prt1] = q1.x();
        m_PtYRotation[prt1] = q1.y();
        m_PtZRotation[prt1] = q1.z();
        m_PtWRotation[prt1] = q1.w();

        Eigen::Quaterniond q2{ m_PtWRotation[prt2], m_PtXRotation[prt2], m_PtYRotation[prt2], m_PtZRotation[prt2] };
        Eigen::Vector3d ptemp2 = i2 * rn2;
        Eigen::Quaterniond temp2 = Eigen::Quaterniond{ 0.0, ptemp2.x(), ptemp2.y(), ptemp2.z() } * q2;

        q2.x() -= 0.5 * lambda * temp2.x();
        q2.y() -= 0.5 * lambda * temp2.y();
        q2.z() -= 0.5 * lambda * temp2.z();
        q2.w() -= 0.5 * lambda * temp2.w();

        q2.normalize();

        m_PtXRotation[prt2] = q2.x();
        m_PtYRotation[prt2] = q2.y();
        m_PtZRotation[prt2] = q2.z();
        m_PtWRotation[prt2] = q2.w();
    }

    void PhyXManager::AttachJoint(int prt1, Eigen::Vector3d r1, int prt2, Eigen::Vector3d r2, double dRest, double alpha, double dts)
    {
        // Geometry::Transform t1 = m_PhyXPartList[prt1]->frame->GetTransform();
        // Geometry::Transform t2 = m_PhyXPartList[prt2]->frame->GetTransform();

        Eigen::Vector3d pos1{ m_PtXPosition[prt1], m_PtYPosition[prt1], m_PtZPosition[prt1] };
        Eigen::Vector3d pos2{ m_PtXPosition[prt2], m_PtYPosition[prt2], m_PtZPosition[prt2] };

        Eigen::Quaterniond rot1{ m_PtWRotation[prt1], m_PtXRotation[prt1], m_PtYRotation[prt1], m_PtZRotation[prt1] };
        Eigen::Quaterniond rot2{ m_PtWRotation[prt2], m_PtXRotation[prt2], m_PtYRotation[prt2], m_PtZRotation[prt2] };

        Eigen::Vector3d a1 = pos1 + (rot1 * r1);
        Eigen::Vector3d a2 = pos2 + (rot2 * r2);

        // Eigen::Vector3d a1 = t1 * r1;
        // Eigen::Vector3d a2 = t2 * r2;

        // Eigen::Vector3d n = {m_PtXPosition[prt2] - m_PtXPosition[prt1],
        //                      m_PtYPosition[prt2] - m_PtYPosition[prt1],
        //                      m_PtZPosition[prt2] - m_PtZPosition[prt1]};

        Eigen::Vector3d n = a2 - a1;
        double d = n.norm();
        n.normalize();

        ApplyLinearCorrection(prt1, r1, prt2, r2, -(d-dRest)*n, alpha, dts);
    }

    void PhyXManager::UpdatePhyX(float dt)
    {
        ZoneScoped;
        if (!s_PhyXEnabled || !m_TimeEnabled)
            return;

        double dts = static_cast<double>(dt)/static_cast<double>(m_GuiSubSteps);

        for (int n = 0; n < m_GuiSubSteps; n++)
        {
            // if (n > 3)
            //     break;
            for (int i = 0; i < m_PtXPosition.size(); i++)
            {
                if (!m_PtGrounded[i])
                {
                    m_PtXLastPos[i] = m_PtXPosition[i];
                    m_PtYLastPos[i] = m_PtYPosition[i];
                    m_PtZLastPos[i] = m_PtZPosition[i];

                    m_PtYLinVelocity[i] += dts*g.y();

                    m_PtXPosition[i] += dts*m_PtXLinVelocity[i];
                    m_PtYPosition[i] += dts*m_PtYLinVelocity[i];
                    m_PtZPosition[i] += dts*m_PtZLinVelocity[i];

                    m_PtXLastRot[i] = m_PtXRotation[i];
                    m_PtYLastRot[i] = m_PtYRotation[i];
                    m_PtZLastRot[i] = m_PtZRotation[i];
                    m_PtWLastRot[i] = m_PtWRotation[i];

                    // If want to add torque: omega += h*(I^-1)*torque_ext

                    Eigen::Quaterniond omega{ 0.0, m_PtXAngVelocity[i], m_PtYAngVelocity[i], m_PtZAngVelocity[i]};
                    Eigen::Quaterniond rot{ m_PtWRotation[i], m_PtXRotation[i], m_PtYRotation[i], m_PtZRotation[i]};

                    Eigen::Quaterniond dRot = omega * rot;
                    
                    rot.x() += 0.5 * dts * dRot.x();
                    rot.y() += 0.5 * dts * dRot.y();
                    rot.z() += 0.5 * dts * dRot.z();
                    rot.w() += 0.5 * dts * dRot.w();

                    rot.normalize();

                    m_PtXRotation[i] = rot.x();
                    m_PtYRotation[i] = rot.y();
                    m_PtZRotation[i] = rot.z();
                    m_PtWRotation[i] = rot.w();
                }
            }

            for (int i = 0; i < m_Joints.size(); i++)
            {
                auto& joint = m_Joints[i];
                Eigen::Vector3d r1{ 0.0, 0.0, 0.0 };
                Eigen::Vector3d r2{ 0.0, -1.0, 0.0 };
                AttachJoint(joint.pt1, r1, joint.pt2, r2, joint.m_DRest, joint.m_Alpha, dts);
            }

            for (int i = 0; i < m_PtXPosition.size(); i++)
            {
                m_PtXLinVelocity[i] = (m_PtXPosition[i] - m_PtXLastPos[i])/dts;
                m_PtYLinVelocity[i] = (m_PtYPosition[i] - m_PtYLastPos[i])/dts;
                m_PtZLinVelocity[i] = (m_PtZPosition[i] - m_PtZLastPos[i])/dts;

                Eigen::Quaterniond rot{ m_PtWRotation[i], m_PtXRotation[i], m_PtYRotation[i], m_PtZRotation[i]};
                Eigen::Quaterniond rot_prev{ m_PtWLastRot[i], m_PtXLastRot[i], m_PtYLastRot[i], m_PtZLastRot[i]};

                Eigen::Quaterniond drot = rot * rot_prev.inverse();

                double sign = 1.0;
                if (drot.w() < 0.0)
                    sign = -1.0;

                m_PtXAngVelocity[i] = sign * 2.0 * drot.x()/dts;
                m_PtYAngVelocity[i] = sign * 2.0 * drot.y()/dts;
                m_PtZAngVelocity[i] = sign * 2.0 * drot.z()/dts;
            }
        }

        for (int i = 0; i < m_PhyXPartList.size(); i++) 
        {
            auto& phyXPart = m_PhyXPartList[i];

            Eigen::Vector3d pos = {m_PtXPosition[i], m_PtYPosition[i], m_PtZPosition[i]};
            Eigen::Quaterniond rot = {m_PtWRotation[i], m_PtXRotation[i], m_PtYRotation[i], m_PtZRotation[i]};

            phyXPart->frame->SetPositionRotation(pos, rot);
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
