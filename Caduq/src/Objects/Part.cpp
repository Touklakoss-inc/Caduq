#include "Part.h"

#include "Geometry/Geo.h"
#include "Objects/EntityManager.h"

#include "Objects/Frame.h"
#include <imgui.h>

#include <memory>
#ifdef VZ_PLATFORM_WINDOWS
  #define _USE_MATH_DEFINES
#endif
#include <math.h>

namespace Caduq 
{
    Part::Part(Geometry::Transform transform, const std::shared_ptr<Frame>& frame, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Part " + std::to_string(++s_IdGenerator), type, frame }
        , m_MainFrame{ std::make_shared<Frame>(transform, frame) }
        , m_EntityManager{ m_MainFrame }
    {
    }

    void Part::Init()
    {
        m_MainFrame->Init();
        for (const auto& entity : m_EntityManager.GetEntityList())
            entity->Init();

        // UpdateGFX();
    }

    void Part::Update(Eigen::Vector3d position, Eigen::Quaterniond rotation)
    {
        GetMainFrame()->Update(position, rotation);
        UpdateGFX();
    }

    void Part::UpdateGFX()
    {
        m_MainFrame->UpdateGFX();
        for (const auto& entity : m_EntityManager.GetEntityList())
            entity->UpdateGFX();
    }

    void Part::Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform)
    {
        m_MainFrame->Visualize(shader, transform);
        for (const auto& entity : m_EntityManager.GetEntityList())
            entity->Visualize(shader, transform);
    }

    void Part::Delete()
    {
        for (const auto& entity : m_EntityManager.GetEntityList())
            m_EntityManager.DeleteEntity(entity);

        m_EntityManager.ClearEntityToDelete();
    }

    void Part::RenderImGui(EntityManager& globalEntityManager)
    {
        ImGuiID id = ImGui::GetID("create_part_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::Separator();

            m_EntityManager.RenderImGui();

            if (ImGui::Button("Modify")) 
            {
                auto quat = Eigen::Quaterniond(m_MainFrame->GetGeoFrame().GetTransform().rotation());
                double angle = acos(quat.w())*2;

                Eigen::Vector4d rotEuler = Eigen::Vector4d::Zero();
                if (angle != 0.0)
                    rotEuler = Eigen::Vector4d(0.0, quat.x(), quat.y(), quat.z())/(sin(angle/2));
                rotEuler[0] = angle * 180.0/M_PI;

                SetPopupParam(m_MainFrame->GetGeoFrame().GetTransform().translation(), rotEuler);

                globalEntityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }
            ImGui::SameLine();
            Entity::RenderImGui(globalEntityManager);

            ImGui::Separator();

            for (const auto& entity : m_EntityManager.GetEntityList())
                entity->RenderImGui(m_EntityManager);

            ImGui::TreePop();
        }
    }

    void Part::Popup(EntityManager& entityManager)
    {
        ImGui::InputFloat2("Position (xyz)", m_GuiPopupPos);
        ImGui::InputFloat4("Rotation (angle, axis)", m_GuiPopupRot);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            entityManager.SetCurEntity(nullptr);
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            if (entityManager.GetCurEntity() == nullptr)
            {
                const auto part = std::make_shared<Caduq::Part>(Geometry::Transform::Identity(), entityManager.GetMainFrame());
                entityManager.CreateEntity(part);

                const auto pos = Eigen::Vector3d(m_GuiPopupPos[0], m_GuiPopupPos[1], m_GuiPopupPos[2]);
                const auto normal = Eigen::Vector3d(m_GuiPopupRot[1], m_GuiPopupRot[2], m_GuiPopupRot[3]).normalized();
                Eigen::Quaterniond rotQ = Eigen::Quaterniond(Eigen::AngleAxisd(m_GuiPopupRot[0] * (M_PI/180.0), normal));

                part->GetMainFrame()->Update(pos, rotQ);
            }
            else
            {
                auto pos = Eigen::Vector3d(m_GuiPopupPos[0], m_GuiPopupPos[1], m_GuiPopupPos[2]);
                const auto normal = Eigen::Vector3d(m_GuiPopupRot[1], m_GuiPopupRot[2], m_GuiPopupRot[3]).normalized();
                auto rotQ = Eigen::Quaterniond(Eigen::AngleAxisd(m_GuiPopupRot[0] * (M_PI/180.0), normal));

                std::dynamic_pointer_cast<Caduq::Part>(entityManager.GetCurEntity())->Update(pos, rotQ);

                ImGui::CloseCurrentPopup();
                entityManager.SetCurEntity(nullptr);
            }
        }
        ImGui::EndPopup();
    }

    void Part::SetPopupParam(Eigen::Vector3d position, Eigen::Vector4d rotation)
    {
        for (int i = 0; i < 3; i++)
            m_GuiPopupPos[i] = position[i];

        for (int i = 0; i < 4; i++)
            m_GuiPopupRot[i] = rotation[i];
    }
}
