#include "Patch.h"

#include <Eigen/Core>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Geometry/Patch.h"

namespace Caduq
{
    Patch::Patch(const Caduq::Spline& s0, const Caduq::Spline& s1, const Caduq::Spline& s2, const Caduq::Spline& s3,
                 int mesh_size, const std::string& name)
        : Entity{ name != "" ? name : "Patch " + std::to_string(++s_IdGenerator) }
        , m_Id{ s_IdGenerator }, m_s0{ s0 }, m_s1{ s1 }, m_s2{ s2 }, m_s3{ s3 }, m_mesh_size{ mesh_size }
    {
    }

    void Patch::Init()
    {
        Geometry::Patch c0{ m_s0.GetGeoSpline(), m_s1.GetGeoSpline(),
                            m_s2.GetGeoSpline(), m_s3.GetGeoSpline() };

        // Create patch mesh
        Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(m_mesh_size, 0.0, 1.0) };
        Eigen::MatrixXd m = c0.Mesh(u, u, m_mesh_size);
        Geometry::Mesh mesh = c0.GetGfxMesh();

        // Cast points to float
        Eigen::MatrixXf patchVertices = mesh.nodes.cast<float>();
        Eigen::VectorX<uint32_t> patchIndices = mesh.elts.cast<uint32_t>();

        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> patchVertexBuffer;
        patchVertexBuffer.reset(Vizir::VertexBuffer::Create(patchVertices.data(), patchVertices.size() * sizeof(float)));

        Vizir::BufferLayout patchLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        patchVertexBuffer->SetLayout(patchLayout);

        // Index buffer
        Vizir::Ref<Vizir::IndexBuffer> patchIndexBuffer;
        patchIndexBuffer.reset(Vizir::IndexBuffer::Create(patchIndices.data(), patchIndices.size()));

        // Vertex array
        m_PatchVertexArray = Vizir::VertexArray::Create();
        m_PatchVertexArray->Bind();
        m_PatchVertexArray->SetVertexBuffer(patchVertexBuffer);
        m_PatchVertexArray->SetIndexBuffer(patchIndexBuffer);
        m_PatchVertexArray->SetPrimitiveType(Vizir::TRIANGLES);
        m_PatchVertexArray->Unbind();
    }

    void Patch::Visualize(Vizir::Ref<Vizir::Shader> m_Shader, glm::mat4 m_Transform)
    {
        Vizir::Renderer::Submit(m_Shader, m_PatchVertexArray, m_Transform);

        m_PatchVertexArray->Unbind();
    }
}
