#include "cqpch.h"
#include "Frame.h"

#include "EntityManager.h"
#include "Geometry/Geo.h"

namespace Caduq
{
    Frame::Frame(Geometry::Transform transform, const std::shared_ptr<Frame>& frame, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Frame " + std::to_string(++s_IdGenerator), type, frame }
        , m_Id{ name != "" ? ++s_IdGenerator : s_IdGenerator }
        , m_GeoFrame{ transform }
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
        splineVertices << m_GeoFrame.GetTransform().translation(), m_X, m_Y, m_Z;
        Eigen::VectorX<uint32_t> splineIndices {{ 0, 1, 0, 2, 0, 3 }};

        UpdateGFXBuffer(splineVertices.cast<float>(), splineIndices, Vizir::LINE_STRIP);
    }

    void Frame::Update(Eigen::Vector3d position, Eigen::Quaterniond rotation)
    {
        m_GeoFrame.SetPositionRotation(position, rotation);

        Update();
    }
    void Frame::Update()
    {
        m_X = m_GeoFrame.GetTransform() * Eigen::Vector3d::UnitX();
        m_Y = m_GeoFrame.GetTransform() * Eigen::Vector3d::UnitY();
        m_Z = m_GeoFrame.GetTransform() * Eigen::Vector3d::UnitZ();

        UpdateGFX();
    }

    void Frame::Delete()
    {
        Entity::Delete();
    }

    /* /!\ recursive function /!\ */
    const Geometry::Transform Frame::GetTransform()
    {
        if (m_RefFrame != nullptr)
            return m_RefFrame->GetTransform() * m_GeoFrame.GetTransform();
        else
            return Geometry::Transform::Identity();
    }

    void Frame::RenderImGui(EntityManager& entityManager)
    {
        ImGuiID id = ImGui::GetID("create_frame_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            if (ImGui::Button("Modify")) 
            {
                auto quat = Eigen::Quaterniond(m_GeoFrame.GetTransform().rotation());
                double angle = acos(quat.w())*2;

                Eigen::Vector4d rotEuler = Eigen::Vector4d::Zero();
                if (angle != 0.0)
                    rotEuler = Eigen::Vector4d(0.0, quat.x(), quat.y(), quat.z())/(sin(angle/2));
                rotEuler[0] = angle * 180.0/std::numbers::pi;

                SetPopupParam(m_GeoFrame.GetTransform().translation(), rotEuler);



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
                const auto pos = Eigen::Vector3d(m_GuiPopupPos[0], m_GuiPopupPos[1], m_GuiPopupPos[2]);
                const auto normal = Eigen::Vector3d(m_GuiPopupRot[1], m_GuiPopupRot[2], m_GuiPopupRot[3]).normalized();
                Eigen::Quaterniond rotQ = Eigen::Quaterniond(Eigen::AngleAxisd(m_GuiPopupRot[0] * (std::numbers::pi/180.0), normal));

                const auto frame = std::make_shared<Caduq::Frame>(Geometry::Transform::Identity(), entityManager.GetMainFrame());
                entityManager.CreateEntity(frame);
                frame->Update(pos, rotQ);
            }
            else
            {
                auto pos = Eigen::Vector3d(m_GuiPopupPos[0], m_GuiPopupPos[1], m_GuiPopupPos[2]);
                const auto normal = Eigen::Vector3d(m_GuiPopupRot[1], m_GuiPopupRot[2], m_GuiPopupRot[3]).normalized();
                auto rotQ = Eigen::Quaterniond(Eigen::AngleAxisd(m_GuiPopupRot[0] * (std::numbers::pi/180.0), normal));

                std::dynamic_pointer_cast<Caduq::Frame>(entityManager.GetCurEntity())->Update(pos, rotQ);

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
