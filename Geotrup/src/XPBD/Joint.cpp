#include "Joint.h"

#include "XPBD/PhyXManager.h"
#include <imgui.h>
#include <string>
namespace XPBD
{
    Joint::Joint()
        : m_Name{ "Joint" + std::to_string(++s_IdGenerator)}, m_Id{ s_IdGenerator }
    {
    }

    void Joint::ApplyLinearCorrection(Point& p1, Point& p2, Eigen::Vector3d dp, double alpha, double dts)
    {
        double C = dp.norm();
        Eigen::Vector3d n = dp.normalized();

        double w1 = 1.0/p1.GetMass();
        if (p1.IsGrounded())
            w1 = 0.0;

        double w2 = 1.0/p2.GetMass();
        if (p2.IsGrounded())
            w2 = 0.0;

        double lambda = -C;
        if (dts != 0.0 && (w1 != 0.0 || w2 != 0.0 || alpha != 0.0))
            lambda /= w1 + w2 + alpha/(dts*dts);

        p1.GetGeoPoint()->SetPosition(p1.GetGeoPoint()->GetPosition() + lambda*n*w1);
        p2.GetGeoPoint()->SetPosition(p2.GetGeoPoint()->GetPosition() - lambda*n*w2);
    }

    void Joint::Attach(Point& p1, Point& p2, double dRest, double alpha, double dts)
    {
        Eigen::Vector3d n = p2.GetGeoPoint()->GetPosition() - p1.GetGeoPoint()->GetPosition(); 
        double d = n.norm();
        n.normalize();

        ApplyLinearCorrection(p1, p2, -(d-dRest)*n, alpha, dts);
    }

    void Joint::RenderImGui(const PhyXManager& phyXManager)
    {
        // ImGui::SameLine();
        if (ImGui::Button("Delete")) 
            ImGui::OpenPopup("Delete?");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("This joint will be deleted.");
            ImGui::Separator();

            if (ImGui::Button("OK", ImVec2(120, 0))) 
            { 
                phyXManager.AddJointToDelete(shared_from_this());

                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }
}
