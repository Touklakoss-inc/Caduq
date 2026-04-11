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

        m_PtInvMasses.clear();

        m_PtXInvInertia.clear();
        m_PtYInvInertia.clear();
        m_PtZInvInertia.clear();

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

            m_PtInvMasses.push_back(1.0 / phyXPart->mass);


            const double I = 1.0/6.0 * phyXPart->mass*0.5*0.5;
            m_PtXInvInertia.push_back(1.0/I);
            m_PtYInvInertia.push_back(1.0/I);
            m_PtZInvInertia.push_back(1.0/I);

            m_PtGrounded.push_back(phyXPart->isGrounded);

            m_PartCount++;
        }
        std::cout << "Done" << '\n';
    }

    void PhyXManager::UpdatePhyX(float dt)
    {
        ZoneScoped;
        if (!s_PhyXEnabled || !m_TimeEnabled)
            return;

        double dts = static_cast<double>(dt)/static_cast<double>(m_GuiSubSteps);

        for (int n = 0; n < m_GuiSubSteps; n++)
        {
            for (int i = 0; i < m_PartCount; i++)
            {
                if (!m_PtGrounded[i])
                    Integrate(i, dts);
            }

            SolveConstraint(dt, 0, -1, { 0.025, 0.05, 0.0 }, { 0.0, 2.5, 0.0 }, 0.25, 0.001);

            for (int i = 0; i < m_PartCount; i++)
            {
                if (!m_PtGrounded[i])
                    UpdateVelocities(i, dts, 5.0);
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

    void PhyXManager::Integrate(int p, double dt)
    {
        if (m_PtInvMasses[p] == 0.0)
            return;

        // linear motion
        m_PtXLastPos[p] = m_PtXPosition[p];
        m_PtYLastPos[p] = m_PtYPosition[p];
        m_PtZLastPos[p] = m_PtZPosition[p];

        m_PtXLinVelocity[p] += dt*g.x();
        m_PtYLinVelocity[p] += dt*g.y();
        m_PtZLinVelocity[p] += dt*g.z();

        m_PtXPosition[p] += dt*m_PtXLinVelocity[p];
        m_PtYPosition[p] += dt*m_PtYLinVelocity[p];
        m_PtZPosition[p] += dt*m_PtZLinVelocity[p];

        // angular motion
        m_PtXLastRot[p] = m_PtXRotation[p];
        m_PtYLastRot[p] = m_PtYRotation[p];
        m_PtZLastRot[p] = m_PtZRotation[p];
        m_PtWLastRot[p] = m_PtWRotation[p];

        // If want to add torque: omega += h*(I^-1)*torque_ext

        Eigen::Quaterniond dRot{ 0.0, 
                                 m_PtXAngVelocity[p], 
                                 m_PtYAngVelocity[p], 
                                 m_PtZAngVelocity[p]};

        Eigen::Quaterniond rot{ m_PtWRotation[p], 
                                m_PtXRotation[p], 
                                m_PtYRotation[p], 
                                m_PtZRotation[p]};

        dRot = dRot * rot;
        rot.x() += 0.5 * dt * dRot.x();
        rot.y() += 0.5 * dt * dRot.y();
        rot.z() += 0.5 * dt * dRot.z();
        rot.w() += 0.5 * dt * dRot.w();
        rot.normalize();

        m_PtXRotation[p] = rot.x();
        m_PtYRotation[p] = rot.y();
        m_PtZRotation[p] = rot.z();
        m_PtWRotation[p] = rot.w();
    }

    double PhyXManager::GetInverseMass(int p, Eigen::Vector3d normal, Eigen::Vector3d pos)
    {
        if (m_PtInvMasses[p] == 0.0)
            return 0.0;

        Eigen::Vector3d rn = normal;

        const Eigen::Vector3d pPos{ m_PtXPosition[p],
                                    m_PtYPosition[p],
                                    m_PtZPosition[p]};
        const Eigen::Quaterniond invRot{ m_PtWRotation[p],
                                        -m_PtXRotation[p],
                                        -m_PtYRotation[p],
                                        -m_PtZRotation[p]};
        rn = pos - pPos;
        rn = rn.cross(normal);
        rn = invRot * rn;

        double w = rn.x() * rn.x() * m_PtXInvInertia[p]
                 + rn.y() * rn.y() * m_PtYInvInertia[p]
                 + rn.z() * rn.z() * m_PtZInvInertia[p];

        w += m_PtInvMasses[p];

        return w;
    }

    void PhyXManager::_ApplyCorrection(int p, Eigen::Vector3d corr, Eigen::Vector3d pos)
    {
        if (m_PtInvMasses[p] == 0.0)
            return;

        // linear correction
        m_PtXPosition[p] += m_PtInvMasses[p] * corr.x();
        m_PtYPosition[p] += m_PtInvMasses[p] * corr.y();
        m_PtZPosition[p] += m_PtInvMasses[p] * corr.z();

        const Eigen::Vector3d pPos{ m_PtXPosition[p],
                                    m_PtYPosition[p],
                                    m_PtZPosition[p]};
        const Eigen::Quaterniond invRot{ m_PtWRotation[p],
                                         m_PtXRotation[p],
                                         m_PtYRotation[p],
                                         m_PtZRotation[p]};
        const Eigen::Vector3d invInertia{ m_PtXInvInertia[p],
                                          m_PtYInvInertia[p],
                                          m_PtZInvInertia[p]};
        Eigen::Quaterniond rot{ m_PtWRotation[p],
                                m_PtXRotation[p],
                                m_PtYRotation[p],
                                m_PtZRotation[p]};
        Eigen::Vector3d dOmega = pos - pPos;

        dOmega = dOmega.cross(corr);
        dOmega = invRot * dOmega;
        dOmega.x() *= invInertia.x();
        dOmega.y() *= invInertia.y();
        dOmega.z() *= invInertia.z();
        dOmega = rot * dOmega;

        Eigen::Quaterniond dRot{ 0.0,
                                 dOmega.x(),
                                 dOmega.y(),
                                 dOmega.z()};
        dRot = dRot * rot;
        rot.x() += 0.5 * dRot.x();
        rot.y() += 0.5 * dRot.y();
        rot.z() += 0.5 * dRot.z();
        rot.w() += 0.5 * dRot.w();
        rot.normalize();

        m_PtXRotation[p] = rot.x();
        m_PtYRotation[p] = rot.y();
        m_PtZRotation[p] = rot.z();
        m_PtWRotation[p] = rot.w();
    }

    double PhyXManager::ApplyCorrection(double dt, double compliance, Eigen::Vector3d corr, int p1, int p2, Eigen::Vector3d r1, Eigen::Vector3d r2)
    {
        if (corr.norm() == 0.0)
            return 0.0;

        const double C = corr.norm();
        Eigen::Vector3d normal = corr.normalized();

        double w = GetInverseMass(p1, normal, r1);
        if (p2 >= 0)
            w += GetInverseMass(p2, normal, r2); // normal should be < 0 no ?
        
        if (w == 0.0)
            return 0.0;

        const double alpha = compliance / (dt*dt);
        const double lambda = -C / (w + alpha);

        normal *= -lambda;

        _ApplyCorrection(p1, normal, r1);
        if (p2 >= 0)
        {
            normal *= -1.0;
            _ApplyCorrection(p2, normal, r2);
        }

        return lambda / (dt*dt);
    }

    void PhyXManager::SolveConstraint(double dt, int p1, int p2, Eigen::Vector3d r1, Eigen::Vector3d r2, double dRest, double alpha)
    {
        const Eigen::Quaterniond q1{ m_PtWRotation[p1],
                                     m_PtXRotation[p1],
                                     m_PtYRotation[p1],
                                     m_PtZRotation[p1]};
        const Eigen::Vector3d pos1{ m_PtXPosition[p1],
                                    m_PtYPosition[p1], 
                                    m_PtZPosition[p1]}; 
        const Eigen::Vector3d r1World = q1 * r1 + pos1;

        Eigen::Vector3d r2World = r2;
        if (p2 >= 0)
        {
            const Eigen::Quaterniond q2{ m_PtWRotation[p2],
                                         m_PtXRotation[p2],
                                         m_PtYRotation[p2],
                                         m_PtZRotation[p2]};
            const Eigen::Vector3d pos2{ m_PtXPosition[p2],
                                        m_PtYPosition[p2], 
                                        m_PtZPosition[p2]}; 
            r2World = q2 * r2 + pos2;
        }

        Eigen::Vector3d corr = r2World - r1World;

        const double distance = corr.norm();
        corr = corr.normalized() * (distance - dRest);

        const double force = ApplyCorrection(dt, alpha, corr, p1, p2, r1World, r2World);

        const double elongation = distance - dRest;
    }

    void PhyXManager::UpdateVelocities(int p, double dt, double damping)
    {
        if (m_PtInvMasses[p] == 0.0)
            return;

        // linear motion
        m_PtXLinVelocity[p] = (m_PtXPosition[p] - m_PtXLastPos[p])/dt;
        m_PtYLinVelocity[p] = (m_PtYPosition[p] - m_PtYLastPos[p])/dt;
        m_PtZLinVelocity[p] = (m_PtZPosition[p] - m_PtZLastPos[p])/dt;

        // angular motion
        Eigen::Quaterniond rot{ m_PtWRotation[p], 
                                m_PtXRotation[p], 
                                m_PtYRotation[p], 
                                m_PtZRotation[p]};

        Eigen::Quaterniond invLastRot{ m_PtWLastRot[p], 
                                      -m_PtXLastRot[p], 
                                      -m_PtYLastRot[p], 
                                      -m_PtZLastRot[p]};

        Eigen::Quaterniond drot = rot * invLastRot;
        Eigen::Vector3d omega{ drot.x() * 2.0 / dt,
                               drot.y() * 2.0 / dt,
                               drot.z() * 2.0 / dt};

        double sign = 1.0;
        if (drot.w() < 0.0)
            sign = -1.0;

        m_PtXAngVelocity[p] = omega.x();
        m_PtYAngVelocity[p] = omega.y();
        m_PtZAngVelocity[p] = omega.z();

        m_PtXLinVelocity[p] *= std::max(1.0 - damping*dt, 0.0);
        m_PtYLinVelocity[p] *= std::max(1.0 - damping*dt, 0.0);
        m_PtZLinVelocity[p] *= std::max(1.0 - damping*dt, 0.0);
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
