#include "StlEntity.h"

#include "BobIntegration.h"
#include "Objects/Entity.h"
#include "Objects/EntityManager.h"
#include "Vizir/Renderer/VertexArray.h"
#include "Tools/STL.h"
#include "MyImGui.h"

#include <filesystem>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Caduq
{
    StlEntity::StlEntity(const std::string& filename, const std::shared_ptr<Frame>& frame, Type type, const std::string& name)
        : Entity{ name != "" ? name : "StlEntity " + std::to_string(++s_IdGenerator), type, frame }
        , m_Mesh{ Tools::ImportSTL(filename) }
    {
    }

    void StlEntity::Init()
    {
        UpdateGFX();
    }

    void StlEntity::UpdateGFX()
    {
        Entity::UpdateGFXBuffer(m_Mesh.nodes.cast<float>(), m_Mesh.elts.cast<uint32_t>(), Vizir::TRIANGLES);
    }

    void StlEntity::Delete()
    {
        Entity::Delete();
    }

    void StlEntity::RenderImGui(EntityManager& entityManager)
    {
        if (ImGui::TreeNode(m_Name.data()))
        {
            ImGui::ColorEdit3("", glm::value_ptr(m_Color));
            ImGui::SameLine();
            Entity::RenderImGui(entityManager);

            ImGui::TreePop();
        }
    }

    void StlEntity::Popup(EntityManager& entityManager)
    {
        MyImGui::InputText("STL file name", &m_FileName);

        ImGui::Separator();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            entityManager.SetCurEntity(nullptr);
        }

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            if (entityManager.GetCurEntity() == nullptr)
            {
                if (std::filesystem::exists(m_FileName))
                    entityManager.CreateEntity(std::make_shared<Caduq::StlEntity>(m_FileName, entityManager.GetMainFrame()));
                else
                    CQ_ERROR("This stl file does not exist");
            }
            else
                CQ_ERROR("Cannot modify an stl object, current entity is not nullptr");
        }
        ImGui::EndPopup();
    }
}
