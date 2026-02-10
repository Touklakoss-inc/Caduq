#include "Patch.h"

#include "EntityManager.h"
#include "BobIntegration.h"
#include "MyImGui.h"

#include <Eigen/Core>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <set>

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
                if (m_s3 != nullptr)
                    SetPopupParam(entityManager, m_s0->GetID(), m_s1->GetID(), m_s2->GetID(), m_s3->GetID());
                else
                    SetPopupParam(entityManager, m_s0->GetID(), m_s1->GetID(), m_s2->GetID(), -1);

                entityManager.SetCurEntity(shared_from_this());
                ImGui::OpenPopup(id);
            }

            Entity::RenderImGui(entityManager);

            ImGui::TreePop();
        }
    }

    void Patch::PatchPopup(EntityManager& entityManager)
    {
        MyCombo("First Spline", entityManager.GetSplineList(), m_GuiSpline1ID);
        MyCombo("Second Spline", entityManager.GetSplineList(), m_GuiSpline2ID);

        ImGui::Separator();

        MyCombo("Third Spline", entityManager.GetSplineList(), m_GuiSpline3ID);
        if (m_GuiSpline4ID != -1)
            MyCombo("Fourth Spline", entityManager.GetSplineList(), m_GuiSpline4ID);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            entityManager.SetCurEntity(nullptr);
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            // Check if all 4 selected splines are different
            std::set<int> indexes = { m_GuiSpline1ID, m_GuiSpline2ID, m_GuiSpline3ID, m_GuiSpline4ID };
            if (indexes.size() == 4)
            {
                std::shared_ptr<Caduq::Spline> spline4 { nullptr };
                if (m_GuiSpline4ID != -1)
                    spline4 = entityManager.GetSpline(m_GuiSpline4ID).lock();
                if (entityManager.GetCurEntity() == nullptr)
                {
                    entityManager.CreateEntity(std::make_shared<Caduq::Patch>(entityManager.GetSpline(m_GuiSpline1ID).lock(),
                                                               entityManager.GetSpline(m_GuiSpline2ID).lock(),
                                                               entityManager.GetSpline(m_GuiSpline3ID).lock(),
                                                               spline4,
                                                               10, Type::patch));
                }
                else
                {
                    std::dynamic_pointer_cast<Caduq::Patch>(entityManager.GetCurEntity())->Update(entityManager.GetSpline(m_GuiSpline1ID).lock(),
                                                                                 entityManager.GetSpline(m_GuiSpline2ID).lock(),
                                                                                 entityManager.GetSpline(m_GuiSpline3ID).lock(),
                                                                                 spline4);
                }

                ImGui::CloseCurrentPopup();
                entityManager.SetCurEntity(nullptr);
            }
            else
                VZ_WARN("Select four different splines to create a patch");
        }
        ImGui::EndPopup();
    }

    void Patch::SetPopupParam(EntityManager& entityManager, int spline1ID, int spline2ID, int spline3ID, int spline4ID)
    {
        for (int i = 0; i < entityManager.GetSplineList().size(); i++)
        {
            auto curSplineID = entityManager.GetSpline(i).lock()->GetID();

            if (curSplineID == spline1ID)
                m_GuiSpline1ID = i;

            if (curSplineID == spline2ID)
                m_GuiSpline2ID = i;

            if (curSplineID == spline3ID)
                m_GuiSpline3ID = i;

            if (curSplineID == spline4ID)
                m_GuiSpline4ID = i;
        }

        if (spline4ID == -1)
            m_GuiSpline4ID = -1;
    }
}
