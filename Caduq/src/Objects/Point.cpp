#include "Point.h"

#include "EntityManager.h"
#include "Vizir/Renderer/VertexArray.h"
#include "XPBD/Point.h"
#include "PhyXManager.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>

namespace Caduq
{
    Point::Point(Eigen::Vector3d pos, Type type, OptParam oP)
        : Entity{ oP.name != "" ? oP.name : "Point " + std::to_string(++s_IdGenerator), type }
        , m_Id{ oP.name != "" ? ++s_IdGenerator : s_IdGenerator }
        , m_GeoPoint{ std::make_shared<Geometry::Point>(pos) }
        , m_PhyXPoint{ std::make_shared<XPBD::Point>(m_GeoPoint, oP.mass, oP.grounded) }
    {
    }

    void Point::Init()
    {
        UpdateGFX();
    }

    void Point::UpdateGFX()
    {
        // Cast to float
        Eigen::Vector3f pointVertice = m_GeoPoint->GetPosition().cast<float>();
        Eigen::Vector<uint32_t, 1> pointIndice{ 0 };

        UpdateGFXBuffer(pointVertice, pointIndice);

        for (const auto& child : m_Children)
        {
            child->UpdateGFX();
        }
    }

    void Point::UpdateGFXBuffer(Eigen::MatrixXf vertices, Eigen::VectorX<uint32_t> indices, Vizir::PrimitiveType primitiveType)
    {
        // Visualization buffer
        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> pointsVertexBuffer;
        pointsVertexBuffer.reset(Vizir::VertexBuffer::Create(vertices.data(), static_cast<uint32_t>(vertices.size()) * sizeof(float)));

        Vizir::BufferLayout pointsLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        pointsVertexBuffer->SetLayout(pointsLayout);

        // Index buffer
        Vizir::Ref<Vizir::IndexBuffer> pointIndexBuffer;
        pointIndexBuffer.reset(Vizir::IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size())));

        // Vertex array
        m_VertexArray = Vizir::VertexArray::Create();        
        m_VertexArray->Bind();
        m_VertexArray->SetVertexBuffer(pointsVertexBuffer);
        m_VertexArray->SetIndexBuffer(pointIndexBuffer);
        m_VertexArray->SetPrimitiveType(primitiveType);
        m_VertexArray->Unbind();
    }

    void Point::Update(double x, double y, double z)
    {
        for (const auto& p : GetParents())
        {
            p->RemoveChild(shared_from_this());
        }

        ClearParents();

        m_GeoPoint->SetPosition(x, y, z);

        Init();

        VZ_INFO("Point modified");
    }

    void Point::RenderImGui(EntityManager& entityManager)
    {
        ImGuiID id = ImGui::GetID("create_point_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::ColorEdit3("", glm::value_ptr(m_Color));
            ImGui::SameLine();
            if (ImGui::Button("Modify")) 
            {
                Eigen::Vector3f vec = m_GeoPoint->GetPosition().cast<float>();
                entityManager.SetPointPopupParam(vec);

                entityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }
            Entity::RenderImGui(entityManager);

            if (PhyXManager::s_PhyXEnabled)
            {
                ImGui::Checkbox("Grounded", &m_PhyXPoint->IsGroundedRef());

                if (!m_PhyXPoint->IsGrounded()) 
                    ImGui::InputDouble("Mass [kg]", &m_PhyXPoint->GetMassRef());
            }

            ImGui::TreePop();
        }
    }
}
