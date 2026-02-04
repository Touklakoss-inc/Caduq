#include "Spline.h"

#include "Point.h"
#include "Geometry/Spline.h"
#include "EntityManager.h"

#include <Eigen/Core>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Caduq
{
    Spline::Spline(const std::shared_ptr<Point>& startPoint, PointTangency startTangency, 
                   const std::shared_ptr<Point>& endPoint, PointTangency endTangency,
                   int mesh_size, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Spline " + std::to_string(++s_IdGenerator), type }
        , m_Id{ name != "" ? ++s_IdGenerator : s_IdGenerator }, m_mesh_size{ mesh_size }
        , m_StartPoint{ startPoint }, m_StartTangency{ startTangency }
        , m_EndPoint{ endPoint }, m_EndTangency{ endTangency }
    {
    }

    Spline::~Spline()
    {
    }

    void Spline::Init()
    {
        // Add the child/parent relationship
        AddParent(m_StartPoint);
        m_StartPoint->AddChild(shared_from_this());

        AddParent(m_EndPoint);
        m_EndPoint->AddChild(shared_from_this());

        UpdateGFX();
    }

    void Spline::UpdateGFX()
    {
        m_Spline = {Geometry::SplinePoint{ *m_StartPoint->GetGeoPoint(), m_StartTangency.tangent, m_StartTangency.tension },
                    Geometry::SplinePoint{ *m_EndPoint->GetGeoPoint(), m_EndTangency.tangent, m_StartTangency.tension } };
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
        splinesVertexBuffer.reset(Vizir::VertexBuffer::Create(splineVertices.data(), static_cast<uint32_t>(splineVertices.size()) * sizeof(float)));

        Vizir::BufferLayout splinesLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        splinesVertexBuffer->SetLayout(splinesLayout);

        // Index buffer
        Vizir::Ref<Vizir::IndexBuffer> splinesIndexBuffer;
        splinesIndexBuffer.reset(Vizir::IndexBuffer::Create(splineIndices.data(), static_cast<uint32_t>(splineIndices.size())));

        // Vertex array
        m_VertexArray = Vizir::VertexArray::Create();
        m_VertexArray->Bind();
        m_VertexArray->SetVertexBuffer(splinesVertexBuffer);
        m_VertexArray->SetIndexBuffer(splinesIndexBuffer);
        m_VertexArray->SetPrimitiveType(Vizir::LINE_STRIP);
        m_VertexArray->Unbind();

        for (const auto& child : m_Children)
        {
            child->UpdateGFX();
        }
    }

    void Spline::Update(const std::shared_ptr<Point>& startPoint, PointTangency startTangency, 
                        const std::shared_ptr<Point>& endPoint, PointTangency endTangency)
    {
        for (const auto& p : GetParents())
        {
            p->RemoveChild(shared_from_this());
        }

        ClearParents();

        m_StartPoint = startPoint;
        m_StartTangency = startTangency;
        m_EndPoint = endPoint;
        m_EndTangency = endTangency;
        Init();

        VZ_INFO("Spline modified");
    }

    void Spline::RenderImGui(EntityManager& entityManager)
    {
        ImGuiID id = ImGui::GetID("create_spline_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::ColorEdit3("", glm::value_ptr(m_Color));
            ImGui::SameLine();
            if (ImGui::Button("Modify")) 
            {
                entityManager.SetSplinePopupParam(m_Spline.GetStartPoint(), m_StartPoint->GetID(), m_Spline.GetEndPoint(), m_EndPoint->GetID());

                entityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }

            Entity::RenderImGui(entityManager);

            ImGui::TreePop();
        }
    }
}
