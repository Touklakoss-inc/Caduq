#include "Point.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Vizir/Platform/OpenGL/OpenGLShader.h"
#include "Geometry/Point.h"

namespace Caduq
{
    Point::Point(double x, double y, double z)
        :m_Position{ x, y, z }
    {
    }
    Point::Point(Eigen::Vector3d pos)
        :m_Position{ pos }
    {
    }

    void Point::Init()
    {
        Geometry::Point p0{ m_Position };

        // Gather points
        std::vector<float> pointsVertices(1 * 3);
        // Cast to float
        Eigen::Vector3f pf{ p0.GetPosition().cast<float>() };
        // copy data
        memcpy(pointsVertices.data() + 0 * 3, pf.data(), 3 * sizeof(float));
        // Visualization buffer
        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> pointsVertexBuffer;
        pointsVertexBuffer.reset(Vizir::VertexBuffer::Create(pointsVertices.data(), pointsVertices.size() * sizeof(float)));

        Vizir::BufferLayout pointsLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        pointsVertexBuffer->SetLayout(pointsLayout);
        // Index buffer
        uint32_t pointIndices[1] = {
            0
        };
        Vizir::Ref<Vizir::IndexBuffer> pointIndexBuffer;
        pointIndexBuffer.reset(Vizir::IndexBuffer::Create(pointIndices, sizeof(pointIndices) / sizeof(uint32_t)));

        // Vertex array
        m_PointVertexArray = Vizir::VertexArray::Create();
        m_PointVertexArray->SetVertexBuffer(pointsVertexBuffer);
        m_PointVertexArray->SetIndexBuffer(pointIndexBuffer);
        m_PointVertexArray->SetPrimitiveType(Vizir::POINTS);
        m_PointVertexArray->Unbind();
    }

    void Point::Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform)
    {
        Vizir::Renderer::Submit(m_Shader, m_PointVertexArray, m_Transform);

        m_PointVertexArray->Unbind();
    }
}
