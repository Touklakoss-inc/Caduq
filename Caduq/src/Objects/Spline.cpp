#include "Spline.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Vizir/Platform/OpenGL/OpenGLShader.h"
#include "Geometry/Point.h"

namespace Caduq
{
    Spline::Spline(Caduq::Point startPoint, PointTangency startTangency, 
                   Caduq::Point endPoint, PointTangency endTangency,
                   int mesh_size)
        :m_start{ startPoint }, m_startTangency{ startTangency }, 
         m_end{ endPoint }, m_endTangency{ endTangency },
         m_mesh_size{ mesh_size }
    {
    }

    void Spline::Init()
    {
        Geometry::SplinePoint startPoint{ m_start.GetGeoPoint(), 
                                          m_startTangency.tangent, 
                                          m_startTangency.tension };
        Geometry::SplinePoint endPoint{ m_end.GetGeoPoint(), 
                                        m_endTangency.tangent, 
                                        m_endTangency.tension };
        Geometry::Spline s0{ startPoint, endPoint };

        // Create spline mesh, todo: replace MESH_SIZE by m_mesh_size
        const int MESH_SIZE{ m_mesh_size };
        Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(MESH_SIZE, 0.0, 1.0) };
        Eigen::MatrixXd U0 = s0.Mesh(u, MESH_SIZE);

        // Gather points
        std::vector<float> splineVertices(1 * 3 * MESH_SIZE);

        // Cast points to float
        Eigen::MatrixXf splinePoints0 = U0.cast<float>();

        // Copy data
        memcpy(splineVertices.data() + 0 * 3 * MESH_SIZE, splinePoints0.data(), MESH_SIZE * 3 * sizeof(float));
        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> splinesVertexBuffer;
        splinesVertexBuffer.reset(Vizir::VertexBuffer::Create(splineVertices.data(), splineVertices.size() * sizeof(float)));

        Vizir::BufferLayout splinesLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        splinesVertexBuffer->SetLayout(splinesLayout);

        // Index buffer
        // vertex n * MESH_SIZE - 1 (n = 0, .., 3) is duplicated
        // needs to be addressed !
        std::vector<uint32_t> splinesIndices(1 * (MESH_SIZE + 1));
        {
            Eigen::ArrayX<uint32_t> spline0 = Eigen::ArrayX<uint32_t>::LinSpaced(MESH_SIZE + 1, 0 * MESH_SIZE, MESH_SIZE);
            spline0[MESH_SIZE] = UINT32_MAX;

            memcpy(splinesIndices.data() + 0 * (MESH_SIZE + 1), spline0.data(), (MESH_SIZE + 1) * sizeof(uint32_t));
        }

        Vizir::Ref<Vizir::IndexBuffer> splinesIndexBuffer;
        splinesIndexBuffer.reset(Vizir::IndexBuffer::Create(splinesIndices.data(), splinesIndices.size()));

        // Vertex array
        m_SplineVertexArray = Vizir::VertexArray::Create();
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
}
