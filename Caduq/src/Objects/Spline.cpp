#include "Spline.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Geometry/Spline.h"

namespace Caduq
{
    Spline::Spline(Caduq::Point startPoint, PointTangency startTangency, 
                   Caduq::Point endPoint, PointTangency endTangency,
                   int mesh_size)
        :m_spline{ 
            Geometry::SplinePoint{ startPoint.GetGeoPoint(), startTangency.tangent, startTangency.tension },
            Geometry::SplinePoint{ endPoint.GetGeoPoint(), endTangency.tangent, startTangency.tension }
        }
    {
    }

    void Spline::Init()
    {
        // Create spline mesh
        Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(m_mesh_size, 0.0, 1.0) };
        Eigen::MatrixXd U0 = m_spline.Mesh(u, m_mesh_size);
        Geometry::Mesh mesh = m_spline.GetGfxMesh();

        // Cast points to float
        Eigen::MatrixXf splineVertices = mesh.nodes.cast<float>();
        Eigen::VectorX<uint32_t> splineIndices = mesh.elts;

        // Copy data
        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> splinesVertexBuffer;
        splinesVertexBuffer.reset(Vizir::VertexBuffer::Create(splineVertices.data(), splineVertices.size() * sizeof(float)));

        Vizir::BufferLayout splinesLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        splinesVertexBuffer->SetLayout(splinesLayout);

        // Index buffer
        Vizir::Ref<Vizir::IndexBuffer> splinesIndexBuffer;
        splinesIndexBuffer.reset(Vizir::IndexBuffer::Create(splineIndices.data(), splineIndices.size()));

        // Vertex array
        m_SplineVertexArray = Vizir::VertexArray::Create();
        m_SplineVertexArray->Bind();
        m_SplineVertexArray->SetVertexBuffer(splinesVertexBuffer);
        m_SplineVertexArray->SetIndexBuffer(splinesIndexBuffer);
        m_SplineVertexArray->SetPrimitiveType(Vizir::LINE_STRIP);
        m_SplineVertexArray->Unbind();
    }

    void Spline::Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform)
    {
        Vizir::Renderer::Submit(m_Shader, m_SplineVertexArray, m_Transform);

        m_SplineVertexArray->Unbind();
    }
    Geometry::Spline Spline::GetGeoSpline()
    {
        return m_spline;
    }
}
