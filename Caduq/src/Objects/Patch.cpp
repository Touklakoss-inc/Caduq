#include "Patch.h"

#include "EntityManager.h"
#include "BobIntegration.h"

#include <Eigen/Core>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace Caduq
{
    Patch::Patch(const std::shared_ptr<Spline>& s0, const std::shared_ptr<Spline>& s1, 
                 const std::shared_ptr<Spline>& s2, const std::shared_ptr<Spline>& s3,
                 int mesh_size, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Patch " + std::to_string(++s_IdGenerator), type }
        , m_Id{ name != "" ? ++s_IdGenerator : s_IdGenerator }, m_mesh_size{ mesh_size }
        , m_s0{ s0 }, m_s1{ s1 }, m_s2{ s2 }, m_s3{ s3 }
    {
    }

    void Patch::Init()
    {
        // Add the child/parent relationship
        AddParent(m_s0);
        m_s0->AddChild(shared_from_this());

        AddParent(m_s1);
        m_s1->AddChild(shared_from_this());

        AddParent(m_s2);
        m_s2->AddChild(shared_from_this());

        if (m_s3 != nullptr)
        {
            AddParent(m_s3);
            m_s3->AddChild(shared_from_this());
        }

        UpdateGFX();
    }

    void Patch::UpdateGFX()
    {
        if (m_s3 != nullptr)
            m_c0 = { m_s0->GetGeoSpline(), m_s1->GetGeoSpline(),
                    m_s2->GetGeoSpline(), m_s3->GetGeoSpline() };
        else
            m_c0 = { m_s0->GetGeoSpline(), m_s1->GetGeoSpline(),
                    m_s2->GetGeoSpline() };

        // Create patch mesh
        Eigen::ArrayXd u{ Eigen::ArrayXd::LinSpaced(m_mesh_size, 0.0, 1.0) };
        Eigen::MatrixXd m = m_c0.Mesh(u, u, m_mesh_size);
        Geometry::Mesh mesh = m_c0.GetGfxMesh();

        // Cast points to float
        Eigen::MatrixXf patchVertices = mesh.nodes.cast<float>();
        Eigen::VectorX<uint32_t> patchIndices = mesh.elts.cast<uint32_t>();

        UpdateGFXBuffer(patchVertices, patchIndices, Vizir::TRIANGLES);

        for (const auto& child : m_Children)
        {
            child->UpdateGFX();
        }
    }

    void Patch::Update(const std::shared_ptr<Spline>& s0, const std::shared_ptr<Spline>& s1, 
                       const std::shared_ptr<Spline>& s2, const std::shared_ptr<Spline>& s3)
    {
        for (const auto& p : GetParents())
        {
            p->RemoveChild(shared_from_this());
        }

        ClearParents();

        m_s0 = s0;
        m_s1 = s1;
        m_s2 = s2;
        m_s3 = s3;

        Init();

        CQ_INFO("Patch modified");
    }

    void Patch::RenderImGui(EntityManager& entityManager)
    {
        ImGuiID id = ImGui::GetID("create_patch_popup");

        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::ColorEdit3("", glm::value_ptr(m_Color));
            ImGui::SameLine();
            if (ImGui::Button("Modify")) 
            {
                entityManager.SetPatchPopupParam(m_s0->GetID(), m_s1->GetID(), m_s2->GetID(), m_s3->GetID());

                entityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }

            Entity::RenderImGui(entityManager);

            ImGui::TreePop();
        }
    }
}
