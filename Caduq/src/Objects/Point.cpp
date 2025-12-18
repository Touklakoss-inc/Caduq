#include "Point.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include <string>
#include "Geometry/Point.h"

namespace Caduq
{
    Point::Point(double x, double y, double z, const std::string& name)
        : Entity{ name != "" ? name : "Point " + std::to_string(++s_IdGenerator) }
        , m_Id{ s_IdGenerator }, m_Point{ x, y, z }
    {
        std::cout << "Create Point" << '\n';
    }
    Point::Point(Eigen::Vector3d pos, const std::string& name)
        : Point{ pos(0), pos(1), pos(2), name }
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
        m_PointVertexArray = Vizir::VertexArray::Create();        
        m_PointVertexArray->Bind();
        m_PointVertexArray->SetVertexBuffer(pointsVertexBuffer);
        m_PointVertexArray->SetIndexBuffer(pointIndexBuffer);
        m_PointVertexArray->SetPrimitiveType(Vizir::POINTS);
        m_PointVertexArray->Unbind();
    }

    void Point::Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform)
    {
        std::dynamic_pointer_cast<Vizir::OpenGLShader>(shader)->UploadUniformFloat3("u_Color", GetColor());

        Vizir::Renderer::Submit(shader, m_PointVertexArray, transform);

        m_PointVertexArray->Unbind();
    }

    Geometry::Point Point::GetGeoPoint() const
    {
        return m_Point;
    }

}
