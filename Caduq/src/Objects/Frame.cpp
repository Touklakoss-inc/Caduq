#include "Frame.h"

#include "EntityManager.h"
#include "Vizir/Logging/Log.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>

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

    void Frame::Translate(Eigen::Vector3d translation)
    {
        m_Transform.pretranslate(translation);
        UpdateFrame();
    }
    void Frame::TranslateLocal(Eigen::Vector3d translation)
    {
        m_Transform.translate(translation);
        UpdateFrame();
    }
    void Frame::Rotate(Eigen::Quaterniond rotation)
    {
        m_Transform.prerotate(rotation);
        UpdateFrame();
    }
    void Frame::RotateLocal(Eigen::Quaterniond rotation)
    {
        m_Transform.rotate(rotation);
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
        if (ImGui::TreeNode(m_Name.data()))
        {
            if (ImGui::Button("Modify")) 
            {
            }
            Entity::RenderImGui(entityManager);

            ImGui::TreePop();
        }
    }
}
