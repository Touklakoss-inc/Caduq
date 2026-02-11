#include "Frame.h"

#include "EntityManager.h"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>

namespace Caduq
{
    Frame::Frame(Transform transform, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Frame " + std::to_string(++s_IdGenerator), type }
        , m_Id{ name != "" ? ++s_IdGenerator : s_IdGenerator }
        , m_Transform{ transform }
        , m_X{ transform * Eigen::Vector3d::UnitX() }
        , m_Y{ transform * Eigen::Vector3d::UnitY() }
        , m_Z{ transform * Eigen::Vector3d::UnitZ() }
    {
    }

    void Frame::Init()
    {
        m_Color = glm::vec3{ 1.0, 0.0, 1.0 };
        UpdateGFX();
    }

    void Frame::UpdateGFX()
    {
        // Cast to float
        Eigen::MatrixXd splineVertices(3, 4);
        splineVertices << m_Transform.translation(), m_X, m_Y, m_Z;
        Eigen::VectorX<uint32_t> splineIndices {{ 0, 1, 0, 2, 0, 3 }};

        UpdateGFXBuffer(splineVertices.cast<float>(), splineIndices, Vizir::LINE_STRIP);
    }

    void Frame::Delete()
    {
        Entity::Delete();
    }

    void Frame::Translate(Eigen::Vector3d translate)
    {
        m_Transform.pretranslate(translate);
        UpdateFrame();
    }
    void Frame::TranslateLocal(Eigen::Vector3d translate)
    {
        m_Transform.translate(translate);
        UpdateFrame();
    }
    void Frame::Rotate(Eigen::Quaterniond rotate)
    {
        m_Transform.prerotate(rotate);
        UpdateFrame();
    }
    void Frame::RotateLocal(Eigen::Quaterniond rotate)
    {
        m_Transform.rotate(rotate);
        UpdateFrame();
    }
    void Frame::SetPositionRotation(Eigen::Vector3d position, Eigen::Quaterniond rotation)
    {
        m_Transform.setIdentity();
        m_Transform.prerotate(rotation);
        m_Transform.pretranslate(position);
        UpdateFrame();
    }

    void Frame::UpdateFrame()
    {
        m_X = m_Transform * Eigen::Vector3d::UnitX();
        m_Y = m_Transform * Eigen::Vector3d::UnitY();
        m_Z = m_Transform * Eigen::Vector3d::UnitZ();

        UpdateGFX();
    }

    void Frame::RenderImGui(EntityManager& entityManager)
    {
        ImGuiID id = ImGui::GetID("create_frame_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            if (ImGui::Button("Modify")) 
            {
                auto quat = Eigen::Quaterniond(m_Transform.rotation());
                double angle = acos(quat.w())*2;

                Eigen::Vector4d rotEuler = Eigen::Vector4d::Zero();
                if (angle != 0.0)
                    rotEuler = Eigen::Vector4d(0.0, quat.x(), quat.y(), quat.z())/(sin(angle/2));
                rotEuler[0] = angle * 180.0/M_PI;

                SetPopupParam(m_Transform.translation(), rotEuler);

                entityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }
            ImGui::SameLine();
            Entity::RenderImGui(entityManager);

            ImGui::TreePop();
        }
    }

    void Frame::Popup(EntityManager& entityManager)
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
                const auto frame = std::make_shared<Caduq::Frame>();
                entityManager.CreateEntity(frame);

                frame->Translate(Eigen::Vector3d(m_GuiPopupPos[0], m_GuiPopupPos[1], m_GuiPopupPos[2]));

                const auto normal = Eigen::Vector3d(m_GuiPopupRot[1], m_GuiPopupRot[2], m_GuiPopupRot[3]).normalized();
                Eigen::Quaterniond rotQ = Eigen::Quaterniond(Eigen::AngleAxisd(m_GuiPopupRot[0] * (M_PI/180.0), normal));
                frame->RotateLocal(rotQ);
            }
            else
            {
                const auto frame = std::dynamic_pointer_cast<Caduq::Frame>(entityManager.GetCurEntity());

                auto pos = Eigen::Vector3d(m_GuiPopupPos[0], m_GuiPopupPos[1], m_GuiPopupPos[2]);

                const auto normal = Eigen::Vector3d(m_GuiPopupRot[1], m_GuiPopupRot[2], m_GuiPopupRot[3]).normalized();
                auto rotQ = Eigen::Quaterniond(Eigen::AngleAxisd(m_GuiPopupRot[0] * (M_PI/180.0), normal));
                frame->SetPositionRotation(pos, rotQ);

                ImGui::CloseCurrentPopup();
                entityManager.SetCurEntity(nullptr);
            }
        }
        ImGui::EndPopup();
    }

    void Frame::SetPopupParam(Eigen::Vector3d position, Eigen::Vector4d rotation)
    {
        for (int i = 0; i < 3; i++)
            m_GuiPopupPos[i] = position[i];

        for (int i = 0; i < 4; i++)
            m_GuiPopupRot[i] = rotation[i];
    }
}
