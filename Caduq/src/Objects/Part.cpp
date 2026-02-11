#include "Part.h"

#include "Objects/EntityManager.h"

#include <imgui.h>

namespace Caduq 
{
    Part::Part(Transform transform, Type type, const std::string& name)
        : Entity{ name != "" ? name : "Part " + std::to_string(++s_IdGenerator), type }
        , m_MainFrame{ transform }
    {
    }

    void Part::Init()
    {
        m_MainFrame.Init();
        for (const auto& entity : m_EntityManager.GetEntityList())
            entity->Init();

        UpdateGFX();
    }

    void Part::UpdateGFX()
    {
        m_MainFrame.UpdateGFX();
        for (const auto& entity : m_EntityManager.GetEntityList())
            entity->UpdateGFX();
    }

    void Part::Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform)
    {
        m_MainFrame.Visualize(shader, transform);
        for (const auto& entity : m_EntityManager.GetEntityList())
            entity->Visualize(shader, transform);
    }

    void Part::Delete()
    {
        for (const auto& entity : m_EntityManager.GetEntityList())
            m_EntityManager.DeleteEntity(entity);

        m_EntityManager.ClearEntityToDelete();
    }

    void Part::RenderImGui(EntityManager& _)
    {
        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::Separator();

            m_EntityManager.RenderImGui();
            Entity::RenderImGui(_);

            ImGui::Separator();

            for (const auto& entity : m_EntityManager.GetEntityList())
                entity->RenderImGui(m_EntityManager);

            ImGui::TreePop();
        }
    }
}
