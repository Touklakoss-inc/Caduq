#include "Point.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace Caduq
{
    Point::Point(double x, double y, double z, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Point " + std::to_string(++s_IdGenerator), type }
        , m_Id{ s_IdGenerator }, m_Point{ x, y, z }
    {
    }
    Point::Point(Eigen::Vector3d pos, Type type, const std::string& name)
        : Point{ pos(0), pos(1), pos(2), type, name }
    {
    }

    void Point::Init()
    {
        std::cout << m_Id << '\n';
        Geometry::Point p0 = m_Point;

        // Cast to float
        Eigen::Vector3f pointVertice = p0.GetPosition().cast<float>();
        Eigen::Vector<uint32_t, 1> pointIndice{ 0 };

        // Visualization buffer
        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> pointsVertexBuffer;
        pointsVertexBuffer.reset(Vizir::VertexBuffer::Create(pointVertice.data(), pointVertice.size() * sizeof(float)));

        Vizir::BufferLayout pointsLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        pointsVertexBuffer->SetLayout(pointsLayout);

        // Index buffer
        Vizir::Ref<Vizir::IndexBuffer> pointIndexBuffer;
        pointIndexBuffer.reset(Vizir::IndexBuffer::Create(pointIndice.data(), pointIndice.size()));

        // Vertex array
        m_VertexArray = Vizir::VertexArray::Create();        
        m_VertexArray->Bind();
        m_VertexArray->SetVertexBuffer(pointsVertexBuffer);
        m_VertexArray->SetIndexBuffer(pointIndexBuffer);
        m_VertexArray->SetPrimitiveType(Vizir::POINTS);
        m_VertexArray->Unbind();
    }

    Geometry::Point Point::GetGeoPoint() const
    {
        return m_Point;
    }

}
