#include "Entity.h"

#include "EntityManager.h"
#include "BobIntegration.h"
#include "Vizir/Platform/OpenGL/OpenGLShader.h"

#include <memory>
#include <string>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Caduq 
{
    Entity::Entity(const std::string& name, Type type)
        : m_Name{ name }, m_Type{ type }
    {
    }
    Entity::~Entity()
    {
        CQ_INFO(m_Name + " object successfully deleted.");
    }

    void Entity::UpdateGFXBuffer(Eigen::MatrixXf vertices, Eigen::VectorX<uint32_t> indices, Vizir::PrimitiveType primitiveType)
    {
        // Visualization buffer
        // Vertex Buffer
        Vizir::Ref<Vizir::VertexBuffer> pointsVertexBuffer;
        pointsVertexBuffer.reset(Vizir::VertexBuffer::Create(vertices.data(), static_cast<uint32_t>(vertices.size()) * sizeof(float)));

        Vizir::BufferLayout pointsLayout = {
            { Vizir::ShaderDataType::Float3, "v_position"},
        };
        pointsVertexBuffer->SetLayout(pointsLayout);

        // Index buffer
        Vizir::Ref<Vizir::IndexBuffer> pointIndexBuffer;
        pointIndexBuffer.reset(Vizir::IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size())));

        // Vertex array
        m_VertexArray = Vizir::VertexArray::Create();        
        m_VertexArray->Bind();
        m_VertexArray->SetVertexBuffer(pointsVertexBuffer);
        m_VertexArray->SetIndexBuffer(pointIndexBuffer);
        m_VertexArray->SetPrimitiveType(primitiveType);
        m_VertexArray->Unbind();
    }

    void Entity::Visualize(Vizir::Ref<Vizir::Shader> shader, glm::mat4 transform)
    {
        std::dynamic_pointer_cast<Vizir::OpenGLShader>(shader)->UploadUniformFloat3("u_Color", m_Color);

        Vizir::Renderer::Submit(shader, m_VertexArray, transform);

        m_VertexArray->Unbind();
    }

    void Entity::RenderImGui(EntityManager& entityManager)
    {
        if (ImGui::Button("Delete")) 
            ImGui::OpenPopup("Delete?");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("%s will be deleted.", m_Name.c_str());
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
    }

    void Entity::AddParent(const std::shared_ptr<Entity>& parent)
    {
        auto status = m_Parents.insert(parent);
        if (!status.second)
        {
            CQ_WARN(parent->GetName() + " is already the parent of " + m_Name);
        }
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
        auto status = m_Children.insert(child);
        if (!status.second)
        {
            CQ_WARN(child->GetName() + " has already the child " + m_Name);
        }
    }

    void Entity::RemoveChild(const std::shared_ptr<Entity>& child)
    {
        m_Children.erase(child);
    }

    bool Entity::HasChild(const std::shared_ptr<Entity>& child) const
    {
        return m_Children.count(child) == 1;
    }

    void Entity::Delete()
    {
        m_deleting = true;

        for (const auto& p : m_Parents)
        {
            if (!p->GetDeletingStatus())
            {
                p->RemoveChild(shared_from_this());
                CQ_WARN(m_Name + " removed from " + p->m_Name);
            }
        }

        m_Parents.clear();
        m_Children.clear();
    }
}
