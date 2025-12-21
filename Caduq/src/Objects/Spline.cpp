#include "Spline.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Point.h"
#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include "Entity.h"
#include "Geometry/Spline.h"

namespace Caduq
{
    Spline::Spline(const std::shared_ptr<Point>& startPoint, PointTangency startTangency, 
                   const std::shared_ptr<Point>& endPoint, PointTangency endTangency,
                   int mesh_size, const std::string& name)
        : Entity{ name != "" ? name : "Spline " + std::to_string(++s_IdGenerator) }
        , m_Id{ s_IdGenerator }, m_mesh_size{ mesh_size }
        , m_StartPoint{ startPoint }, m_StartTangency{ startTangency }
        , m_EndPoint{ endPoint }, m_EndTangency{ endTangency }
        , m_Spline{  // Move this to the init section ?
            Geometry::SplinePoint{ startPoint->GetGeoPoint(), startTangency.tangent, startTangency.tension },
            Geometry::SplinePoint{ endPoint->GetGeoPoint(), endTangency.tangent, startTangency.tension } 
        }
    {
    }

    void Spline::Init()
    {
        AddParent(m_StartPoint);
        m_StartPoint->AddChild(shared_from_this());

        AddParent(m_EndPoint);
        m_EndPoint->AddChild(shared_from_this());

        // Create spline mesh
        Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(m_mesh_size, 0.0, 1.0) };
        Eigen::MatrixXd U0 = m_Spline.Mesh(u, m_mesh_size);
        Geometry::Mesh mesh = m_Spline.GetGfxMesh();

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

    void Spline::Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform)
    {
        std::dynamic_pointer_cast<Vizir::OpenGLShader>(shader)->UploadUniformFloat3("u_Color", GetColor());

        Vizir::Renderer::Submit(shader, m_SplineVertexArray, transform);

        m_SplineVertexArray->Unbind();
    }

    Geometry::Spline Spline::GetGeoSpline() const
    {
        return m_Spline;
    }
}
