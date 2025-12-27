#include "Entity.h"

#include "Vizir/Logging/Log.h"
#include <memory>
#include <string>
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "EntityManager.h"

namespace Caduq 
{
    Entity::Entity(const std::string& name, Type type)
        : m_Name{ name }, m_Type{ type }
    {
    }
    Entity::~Entity()
    {
        VZ_INFO(m_Name + " object successfully deleted.");
    }

    void Entity::Init()
    {
    }

    void Entity::Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform)
    {
    }

    void Entity::RenderImGui(EntityManager& entityManager)
    {
        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::ColorEdit3("", glm::value_ptr(m_Color));
            ImGui::SameLine();
            if (ImGui::Button("Delete")) 
                ImGui::OpenPopup("Delete?");

            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("%s will NOT be deleted (cause the dev is an idiot).", m_Name.c_str());
                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) 
                { 
                    entityManager.DeleteEntity(shared_from_this());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            ImGui::TreePop();
        }
    }

    void Entity::AddParent(const std::shared_ptr<Entity>& parent)
    {
        m_Parents.insert(parent);
    }

    void Entity::RemoveParent(const std::shared_ptr<Entity>& parent)
    {
        m_Parents.erase(parent);
    }

    bool Entity::HasParent(const std::shared_ptr<Entity>& parent) const
    {
        return m_Parents.count(parent) == 1;
    }

    void Entity::AddChild(const std::shared_ptr<Entity>& child)
    {
        m_Children.insert(child);
    }

    void Entity::RemoveChild(const std::shared_ptr<Entity>& child)
    {
        m_Children.erase(child);
    }

    bool Entity::HasChild(const std::shared_ptr<Entity>& child) const
    {
        return m_Children.count(child) == 1;
    }

    bool Entity::Delete()
    {
        if (m_Children.size() > 0)
        {
            return false;
        }
        else
        {
            for (const auto& p : m_Parents)
            {
                p->RemoveChild(shared_from_this());
            }

            m_Parents.clear();

            return true;
        }
    }
}
