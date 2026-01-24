#include "Point.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace Caduq
{
    Point::Point(double x, double y, double z, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Point " + std::to_string(++s_IdGenerator), type }
        , m_Id{ name != "" ? ++s_IdGenerator : s_IdGenerator }, m_Point{ x, y, z }
    {
    }
    Point::Point(Eigen::Vector3d pos, Type type, const std::string& name)
        : Point{ pos(0), pos(1), pos(2), type, name }
    {
    }

    void Point::Init()
    {
        UpdateGFX();
    }

    void Point::UpdateGFX()
    {
        Geometry::Point p0 = m_Point;

        // Cast to float
        Eigen::Vector3f pointVertice = p0.GetPosition().cast<float>();
        Eigen::Vector<uint32_t, 1> pointIndice{ 0 };

        // Visualization buffer
        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> pointsVertexBuffer;
        pointsVertexBuffer.reset(Vizir::VertexBuffer::Create(pointVertice.data(), static_cast<uint32_t>(pointVertice.size()) * sizeof(float)));

        Vizir::BufferLayout pointsLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        pointsVertexBuffer->SetLayout(pointsLayout);

        // Index buffer
        Vizir::Ref<Vizir::IndexBuffer> pointIndexBuffer;
        pointIndexBuffer.reset(Vizir::IndexBuffer::Create(pointIndice.data(), static_cast<uint32_t>(pointIndice.size())));

        // Vertex array
        m_VertexArray = Vizir::VertexArray::Create();        
        m_VertexArray->Bind();
        m_VertexArray->SetVertexBuffer(pointsVertexBuffer);
        m_VertexArray->SetIndexBuffer(pointIndexBuffer);
        m_VertexArray->SetPrimitiveType(Vizir::POINTS);
        m_VertexArray->Unbind();

        for (const auto& child : m_Children)
        {
            child->UpdateGFX();
        }
    }

    void Point::Update(double x, double y, double z)
    {
        for (const auto& p : GetParents())
        {
            p->RemoveChild(shared_from_this());
        }

        ClearParents();

        m_Point = {x, y, z};

        Init();

        VZ_INFO("Point modified");
    }
}
